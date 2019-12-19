#include "AccessibilityUtil.h"



AccessibilityUtil::AccessibilityUtil()
{
}


AccessibilityUtil::~AccessibilityUtil()
{
}

vector<double> AccessibilityUtil::accessAnalyse(Layer * demand, vector<Layer*> supplys, Layer * road)
{
	vector<double> result;
	vector<vector<vector<double>>> ODs;
	for (int i = 0; i < supplys.size(); i++) {
		Layer * supply = supplys[i];
		vector<vector<double>> OD = calculOD(demand, supply, road);
		ODs.push_back(OD);
	}
	//�õ����е�Ȩ�ؾ����ʼ����ɴ���ֵ
	vector<vector<double>> tempWeight;
	for (int i = 0; i < ODs.size(); i++) {
		vector<vector<double>> OD = ODs[i];
		//���������ϲ�
		vector<double> tempt;
		for (int j = 0; j < OD.size(); j++) {
			double wValueSum = 0;
			for (int k = 0; k < OD[j].size(); k++) {
				wValueSum += OD[j][k];
			}
			tempt.push_back(wValueSum);
		}
		tempWeight.push_back(tempt);
	}
	//�ٺϲ�tempWeight�õ����
	for (int j = 0; j < tempWeight[0].size(); j++) {
		double sum = 0;
		for (int i = 0; i < tempWeight.size(); i++) {
			sum+=tempWeight[i][j];
		}
		result.push_back(sum);
	}
	return result;
}

double AccessibilityUtil::getWeight(double distance)
{
	if (distance <= 400) {
		return 1;
	}
	else if (distance > 400 && distance <= 1600) {
		return -153.6558*pow(distance/1000, 3) + 419.4604*pow(distance / 1000, 2) - 395.9706*distance / 1000 + 201.1086;
	}
	else if (distance > 1600 && distance <= 2400) {
		return -92.8*pow(distance / 1000, 3) + 566.6*pow(distance / 1000, 2) - 1153.1*distance / 1000 + 786.6;
	}
	return 0.0;
}

double AccessibilityUtil::calculDistance(Feature * demand, Feature * supply, Layer * road)
{
	if (snapUtil.ODNetDistMtx == NULL)
	{
		//û��OD����ʱ��˵����һ������·��
		snapUtil.createNetwork(road);
		snapUtil.calcuFloydDist();  //���������㷨����OD����
	}
	//����·������
	EuclideanDistanceUtil distanceUtil;
	GeoPoint* demandPoint = (GeoPoint*)demand->geometry;
	GeoPoint* supplyPoint = (GeoPoint*)supply->geometry;
	int demandNodeID = getNearestNode(demandPoint);  //��ȡ���ڽ��Ľڵ�
	int supplyNodeID = getNearestNode(supplyPoint);
	double distance = snapUtil.ODNetDistMtx[demandNodeID][supplyNodeID];  //��ȡ·������
	//double distance = distanceUtil.computeDistanceByLatLng(demandPoint->y, demandPoint->x, supplyPoint->y, supplyPoint->x);
	return distance;
}

vector<vector<double>> AccessibilityUtil::calculOD(Layer* demand, Layer * supply, Layer * road)
{
	vector<vector<double>> OD;
	for (int i = 0; i < demand->features.size(); i++) {
		vector<double> ODRow;
		Feature * demandFeature = demand->features[i];
		for (int j = 0; j < supply->features.size(); j++) {
			Feature * supplyFeature = supply->features[j];
			double distance = calculDistance(demandFeature, supplyFeature, road);
			ODRow.push_back(getWeight(distance));
		}
		OD.push_back(ODRow);
	}
	return OD;
}

int AccessibilityUtil::getNearestNode(GeoPoint * pt)
{
	//�Ծ�γ�������پ�������������Ľڵ㣬�ӿ�����
	double manhatonDist = MAX_DIST;
	int nodeID=0; //�ڵ�ID��
	for (int i = 0; i < snapUtil.existPoints.size(); i++)
	{
		GeoPoint* nodept = snapUtil.existPoints.at(i);
		//����û��ͶӰ
		if ((fabs(nodept->getX() - pt->getX()) + fabs(nodept->getY() - pt->getY())) < manhatonDist)
		{
			manhatonDist = fabs(nodept->getX() - pt->getX()) + fabs(nodept->getY() - pt->getY());
			nodeID = i;
		}
	}
	pt->ptId = nodeID;  //������ڵ��ID�����õ�
	return nodeID;
}
