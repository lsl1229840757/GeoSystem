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
	 //��������
    Geometry* geometry;
    QJsonObject properties;
};
