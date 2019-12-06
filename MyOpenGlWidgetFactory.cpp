#include "MyOpenGlWidgetFactory.h"

//工厂设计，将GeoMap和MyOpenGlWidget唯一绑定

MyOpenGlWidgetFactory::MyOpenGlWidgetFactory()
{

}


MyOpenGlWidgetFactory::~MyOpenGlWidgetFactory()
{

}

MyOpenGLWidget * MyOpenGlWidgetFactory::getMyOpenGlWidget(GeoMap * geoMap)
{
	MyOpenGLWidget *myOpenGLWidget = beanMap[geoMap];
	if (myOpenGLWidget == NULL) {
		//如果为空就new
		myOpenGLWidget = new MyOpenGLWidget(geoMap);
		//添加进beanMap
		beanMap[geoMap] = myOpenGLWidget;
		return myOpenGLWidget;
	}
	else {
		//如果有就直接返回
		return 	myOpenGLWidget;
	}
}

void MyOpenGlWidgetFactory::deleteMyOpenGlWidget(GeoMap * geoMap)
{
	//TODO 还没有写析构函数后面再处理

}
