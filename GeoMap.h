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
	bool isVisible; //�����Ƿ�ɼ�
	double dx;
	double dy;
};