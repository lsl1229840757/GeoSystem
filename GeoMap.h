#pragma once
#include "qrect.h"
#include "Layer.h"
#include <vector>
#include "MapPrjUtil.h"
#include "MapPrjLambert.h"
#include "MapPrjMercator.h"
#include "qdebug.h"
#include "SpatialIndex.h"
using namespace std;
class GeoMap
{
public:
	GeoMap(void);
	~GeoMap(void);
	vector<Layer*> layers;
	QRectF maxRange;
	void addLayer(Layer *layer);
	Layer* getLayerByName(string name);
	double whRatio;
	double hwRatio;
	bool isVisible; //控制是否可见
	double dx;
	double dy;
	string name;
	MapPrjUtil* mapPrj;  //地图投影
	void setMapPrj(MapPrjType type);
	SpatialIndex *index;
	// //判断是否投影，获取地图Range
	QRectF getMapRange();
	// //重置所有Features的投影状态
	void resetFeaturePrjStatus();
	// //判断是否存在投影
	bool mapPrjEmpty();
};