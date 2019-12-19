#pragma once
#include <vector>
#include "Layer.h"
#include "Feature.h"
#include "EuclideanDistanceUtil.h"
#include "GeoPoint.h"
#include "SnapUtil.h"
using namespace std;
class AccessibilityUtil
{
public:
	AccessibilityUtil();
	~AccessibilityUtil();
	//返回一个和demand中feature长度一致的可达性向量
	vector<double> accessAnalyse(Layer *demand, vector<Layer*> supplys, Layer *road);
	double getWeight(double distance);
	double calculDistance(Feature *demandFeature, Feature *supplyFeature, Layer *road);
	vector<vector<double>> calculOD(Layer* demand, Layer * supply, Layer *road);
	SnapUtil snapUtil;   //路网分析工具
	int getNearestNode(GeoPoint* pt);  //获得供给或需求点最近节点ID

};

