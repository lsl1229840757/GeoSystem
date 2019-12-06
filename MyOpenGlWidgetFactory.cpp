#include "MyOpenGlWidgetFactory.h"

//������ƣ���GeoMap��MyOpenGlWidgetΨһ��

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
		//���Ϊ�վ�new
		myOpenGLWidget = new MyOpenGLWidget(geoMap);
		//��ӽ�beanMap
		beanMap[geoMap] = myOpenGLWidget;
		return myOpenGLWidget;
	}
	else {
		//����о�ֱ�ӷ���
		return 	myOpenGLWidget;
	}
}

void MyOpenGlWidgetFactory::deleteMyOpenGlWidget(GeoMap * geoMap)
{
	//TODO ��û��д�������������ٴ���

}
