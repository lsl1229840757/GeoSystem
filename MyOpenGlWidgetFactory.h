#pragma once
#include "myopenglwidget.h"
#include "GeoMap.h"
#include <unordered_map>
using namespace std;
class MyOpenGlWidgetFactory
{
public:
	MyOpenGlWidgetFactory();
	~MyOpenGlWidgetFactory();
	MyOpenGLWidget* getMyOpenGlWidget(GeoMap *geoMap);
	void deleteMyOpenGlWidget(GeoMap *geoMap);
	//����ά�������
	unordered_map<GeoMap *, MyOpenGLWidget *> beanMap;
};

