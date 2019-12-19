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
	int nodeCount;  //�ڵ���
	double **ODNetDistMtx;  //���е�֮��·���������
	// //���������㷨
	void calcuFloydDist();
	int **path;
private:
	double **costMatrix;   //�ڵ��ĳ�ʼ�������(��Ȩ�ڽӾ���)
};

