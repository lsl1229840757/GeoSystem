#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QWidget>
#include "ui_myopenglwidget.h"
#include <qopenglwidget.h>
#include <qopenglfunctions.h>
#include "GdalUtil.h"
#include "GeoMap.h"
#include "SymbolStyle.h"
#include  <QMouseEvent>
#include <GL/glu.h>
#include "MouseZoomAction.h"
class MyOpenGLWidget : public QOpenGLWidget,protected QOpenGLFunctions
{
	Q_OBJECT

public:
	MyOpenGLWidget::MyOpenGLWidget(GeoMap *map, QWidget *parent = 0);
	~MyOpenGLWidget();
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int width, int height) Q_DECL_OVERRIDE;
	//绘图数据源
	GeoMap *geoMap;
	OGRDataSource *poDS;
	void drawLayer(Layer *layer);
	int width;
	int height;
	double whRatio;
	//缩放操作
	MouseZoomAction mouseZoom;
	double min_x;
	double min_y;
	double max_x;
	double max_y;
protected:
	//鼠标按下事件
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent *event);
private:
	Ui::MyOpenGLWidget ui;
};
#endif // MYOPENGLWIDGET_H