#include "SnapUtil.h"



SnapUtil::SnapUtil()
{
	costMatrix = NULL;
	ODNetDistMtx = NULL;
	path = NULL;
}


SnapUtil::~SnapUtil()
{
	if (costMatrix != NULL)
	{
		for (int i = 0; i < nodeCount; i++)
		{
			delete[] costMatrix[i];
		}
		delete[] costMatrix;
		costMatrix = NULL;
	}
	if (ODNetDistMtx != NULL)
	{
		for (int i = 0; i < nodeCount; i++)
		{
			delete[] ODNetDistMtx[i];
		}
		delete[] ODNetDistMtx;
		ODNetDistMtx = NULL;
	}
}


void SnapUtil::createNetwork(Layer* layer)  //road
{
	// TODO: 在此处添加实现代码.
	int ptID = 0;
	for (int i = 0; i < layer->features.size(); i++)
	{
		if (layer->features.at(i)->geometry->getGeometryType() != GEOPOLYLINE)
			continue;
		GeoPolyline* mgeom = ((GeoPolyline*)(layer->features.at(i)->geometry));
		//对于线段首点
		GeoPoint *bgPt = mgeom->points.front();
		int flag = 0; //是否已经存储该点
		for(int j=0; j<existPoints.size();j++)
		{
			if (fabs((bgPt->getX())-(existPoints[j]->getX()))<0.0001 && fabs((bgPt->getY() -existPoints[j]->getY()))<0.0001) 
			{
				flag = 1;
				bgPt->ptId = j;  //记录点id
				mgeom->beginPtId = j;
				break; 
			}
		}
		if(0==flag)
		{
		existPoints.push_back(bgPt);
		bgPt->ptId = ptID;
		mgeom->beginPtId = ptID;
		ptID ++;
		}
		//对于线段末点
		GeoPoint *edPt = mgeom->points.back();
		flag = 0; //是否已经存储该点
		for (int j = 0; j < existPoints.size(); j++)
		{
			if (fabs((edPt->getX() - existPoints[j]->getX())) < 0.0001 && fabs((edPt->getY() - existPoints[j]->getY())) < 0.0001)
			{
				flag = 1;
				edPt->ptId = j;  //记录点id
				mgeom->endPtId = j;
				break;
			}
		}
		if (0 == flag) {
			existPoints.push_back(edPt);
			edPt->ptId = ptID;
			mgeom->endPtId = ptID;
			ptID++;
		}
	}
	qDebug() << ptID;
	nodeCount = ptID;
	EuclideanDistanceUtil distanceUtil;
	//创建节点间的距离矩阵
	costMatrix = new double*[nodeCount];
	for (int i = 0; i < nodeCount; i++)
	{
		costMatrix[i] = new double[nodeCount];
		for (int j = 0; j < nodeCount; j++)
		{
			if (i == j)costMatrix[i][j] = 0;
			else costMatrix[i][j] = MAX_DIST;
		}
	}
	for (int i = 0; i < layer->features.size(); i++)
	{
		GeoPolyline* mgeom = ((GeoPolyline*)(layer->features.at(i)->geometry));
		double lineLenth = 0;//迭代计算折线长度
		for (int j = 0; j < mgeom->points.size()-1; j++)
		{
			lineLenth += distanceUtil.computeDistanceByLatLng(mgeom->points[j]->getY(), mgeom->points[j]->getX(), mgeom->points[j + 1]->getY(), mgeom->points[j + 1]->getX());
		}
		mgeom->lineLenth = lineLenth; //添加线长(大地线距离)到线   
		if (lineLenth < costMatrix[mgeom->beginPtId][mgeom->endPtId])
		{ //记录更近的距离
			costMatrix[mgeom->beginPtId][mgeom->endPtId] = lineLenth;  //无向图，双向存储距离
			costMatrix[mgeom->endPtId][mgeom->beginPtId] = lineLenth;
		}
	}
	/*
	for (int i = 0; i < nodeCount; i++)
	{
		for (int j = 0; j < nodeCount; j++)
		{
			if (fabs(costMatrix[i][j]-MAX_DIST)< 0.0001)
				continue;
			cout<< costMatrix[i][j] <<",";
		}
	}
	*/
	/*
	Env = TEnv();
	Env.PrepArgs(TStr::Fmt("Network community detection. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
	TExeTm ExeTm;
	Try
		const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "graph.txt", "Input graph (undirected graph)");
	const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "communities.txt", "Output file");
	const int CmtyAlg = Env.GetIfArgPrefixInt("-a:", 2, "Algorithm: 1:Girvan-Newman, 2:Clauset-Newman-Moore, 3:Infomap");

	PUNGraph Graph = TSnap::LoadEdgeList<PUNGraph>(InFNm, false);
	//PUNGraph Graph = TSnap::LoadEdgeList<PUNGraph>("../as20graph.txt", false);
	//PUNGraph Graph = TSnap::GenRndGnm<PUNGraph>(5000, 10000); // generate a random graph

	TSnap::DelSelfEdges(Graph);
	TCnComV CmtyV;
	double Q = 0.0;
	TStr CmtyAlgStr;
	if (CmtyAlg == 1) {
		CmtyAlgStr = "Girvan-Newman";
		Q = TSnap::CommunityGirvanNewman(Graph, CmtyV);
	}
	else if (CmtyAlg == 2) {
		CmtyAlgStr = "Clauset-Newman-Moore";
		Q = TSnap::CommunityCNM(Graph, CmtyV);
	}
	else if (CmtyAlg == 3) {
		CmtyAlgStr = "Infomap";
		Q = TSnap::Infomap(Graph, CmtyV);
	}
	else { Fail; }

	FILE *F = fopen(OutFNm.CStr(), "wt");
	fprintf(F, "# Input: %s\n", InFNm.CStr());
	fprintf(F, "# Nodes: %d    Edges: %d\n", Graph->GetNodes(), Graph->GetEdges());
	fprintf(F, "# Algoritm: %s\n", CmtyAlgStr.CStr());
	if (CmtyAlg != 3) {
		fprintf(F, "# Modularity: %f\n", Q);
	}
	else {
		fprintf(F, "# Average code length: %f\n", Q);
	}
	fprintf(F, "# Communities: %d\n", CmtyV.Len());
	fprintf(F, "# NId\tCommunityId\n");
	for (int c = 0; c < CmtyV.Len(); c++) {
		for (int i = 0; i < CmtyV[c].Len(); i++) {
			fprintf(F, "%d\t%d\n", CmtyV[c][i].Val, c);
		}
	}
	fclose(F);

	Catch
		printf("\nrun time: %s (%s)\n", ExeTm.GetTmStr(), TSecTm::GetCurTm().GetTmStr().CStr());
		*/
}


// //弗洛伊德算法
void SnapUtil::calcuFloydDist()
{
	// TODO: 在此处添加实现代码.
	int i, j, k;
	double tmp;  //临时距离
	//初始化path和OD矩阵
	ODNetDistMtx = new double*[nodeCount];
	path = new int*[nodeCount];
	for (i = 0; i < nodeCount; i++)
	{
		ODNetDistMtx[i] = new double[nodeCount];
		path[i] = new int[nodeCount];
		for (j = 0; j < nodeCount; j++)
		{
			ODNetDistMtx[i][j] = costMatrix[i][j];
			path[i][j] = j;
		}
	}
	//计算最短距离
	for (k = 0;k < nodeCount; k++)
	{
		for (i = 0; i < nodeCount; i++)
		{
			for (j = 0; j < nodeCount; j++)
			{
				//跳过无穷大，加快效率
				if (i == j)continue;
				//注意绝对值
				tmp = (fabs(ODNetDistMtx[i][k] - MAX_DIST) < 0.0001 || fabs(ODNetDistMtx[k][j] - MAX_DIST) < 0.0001) ? MAX_DIST : (ODNetDistMtx[i][k] + ODNetDistMtx[k][j]);
				if (ODNetDistMtx[i][j] > tmp)
				{
					//更新距离
					ODNetDistMtx[i][j] = tmp;
					//记录i到j经过点k
					path[i][j] = path[i][k];
				}
			}
		}
	}
	qDebug() << "Floyd succeed!";
	/*
	for (i = 0; i < nodeCount; i++)
	{
		for (j = 0; j < nodeCount; j++)
		{
			cout << ODNetDistMtx[i][j] << "  ";
		}
		cout << endl;
	}*/
}
