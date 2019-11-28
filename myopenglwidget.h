#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QWidget>
#include "ui_myopenglwidget.h"
#include <qopenglwidget.h>
#include <qopenglfunctions.h>
#include "GdalUtil.h"
#include "GeoMap.h"
class MyOpenGLWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
	Q_OBJECT

public:
	MyOpenGLWidget::MyOpenGLWidget(GeoMap *map, QWidget *parent = 0);
	~MyOpenGLWidget();
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int width, int height) Q_DECL_OVERRIDE;
	//»æÍ¼Êý¾ÝÔ´
	GeoMap *geoMap;
	OGRDataSource *poDS;
	void drawLayer(Layer *layer);
	int width;
	int height;
private:
	Ui::MyOpenGLWidget ui;
};
#endif // MYOPENGLWIDGET_H