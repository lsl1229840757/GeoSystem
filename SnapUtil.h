#pragma once
//#include "Snap.h"
#include "GeoPoint.h"
#include "GeoPolyline.h"
#include <vector>
#include "Layer.h"
#include "qdebug.h"
#include "EuclideanDistanceUtil.h"

#define MAX_DIST DBL_MAX

using namespace std;
class SnapUtil
{
public:


	//static const double MAX_DIST;
	SnapUtil();
	~SnapUtil();
	void createNetwork(Layer* layer);
	vector<GeoPoint*> existPoints;
	int nodeCount;  //节点数
	double **ODNetDistMtx;  //所有点之间路网距离矩阵
	// //弗洛伊德算法
	void calcuFloydDist();
	int **path;
private:
	double **costMatrix;   //节点间的初始距离矩阵(带权邻接矩阵)
};

