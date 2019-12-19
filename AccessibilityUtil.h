#pragma once
#include <vector>
#include "Layer.h"
#include "Feature.h"
#include "EuclideanDistanceUtil.h"
#include "GeoPoint.h"
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
};

