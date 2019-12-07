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
#include "MouseDragAction.h"
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
	QRectF viewRange;
	//跟踪鼠标点, 记录鼠标世界坐标点
	QPointF centerPos;
	//移动操作
	MouseDragAction mouseDrag;
	//标准化坐标转世界坐标
	QPointF MyOpenGLWidget::normalCd2worldCd(double x, double y);
	//屏幕坐标转世界坐标
	QPointF MyOpenGLWidget::screenCd2worldCd(QPointF screenPoint);
protected:
	//鼠标点击释放事件
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent *event);
	//滚轮事件
	void wheelEvent(QWheelEvent*event);
	//鼠标移动操作
	void mouseMoveEvent(QMouseEvent *event);
signals:
	//向父类发送当前鼠标的世界坐标
	void sendCurrentPos(QPointF pos);
private:
	Ui::MyOpenGLWidget ui;
};
#endif // MYOPENGLWIDGET_H