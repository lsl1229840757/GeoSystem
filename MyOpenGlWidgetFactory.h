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
	//用来维护对象池
	unordered_map<GeoMap *, MyOpenGLWidget *> beanMap;
};

