#pragma once
#include "Feature.h"
#include "qrect.h"
#include <vector>
using namespace std;
class Layer
{
public:
	Layer(void);
	~Layer(void);
	vector<Feature*> features;
	int layerID;
	QRectF range;
	string name;
	bool isVisble;
	QRectF getLayerRange();
	time_t t;
};