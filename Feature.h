#pragma once
#include <vector>
#include "Geometry.h"
#include <QJsonObject>
using namespace std;
class Feature
{
public:
	Feature(void);
	~Feature(void);
	 //∂®“Â Ù–‘
    Geometry* geometry;
    QJsonObject properties;
};
