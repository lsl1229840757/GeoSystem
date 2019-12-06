#pragma once
#include "qrect.h"
#include "Layer.h"
#include <vector>
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
	//标准坐标转世界坐标
	QPointF NormalCd2worldCd(double x, double y);
};