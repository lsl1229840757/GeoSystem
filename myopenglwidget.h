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
#include "GridIndex.h"
#include "QuadNode.h"
#include "QuadTreeIndex.h"
#include "geos.h"
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
	//屏幕点转标准坐标系
	QPointF screenCd2normalCd(double x, double y);
	//标准化坐标转世界坐标
	QPointF normalCd2worldCd(double x, double y);
	//屏幕坐标转世界坐标
	QPointF screenCd2worldCd(QPointF screenPoint);
	vector<Feature *> selectedFeature;
	bool searchByClick(QPoint screenPoint);
	//分层设色
	void setStyleByProperties(Layer *layer, QString propertyName);
protected:
	//鼠标点击释放事件
	void mousePressEvent(QMouseEvent * event);
	void mouseReleaseEvent(QMouseEvent *event);
	//滚轮事件
	void wheelEvent(QWheelEvent*event);
	//鼠标移动操作
	void mouseMoveEvent(QMouseEvent *event);
	//画feature, 如果有特殊情况就可以传入symbol为准画图
	void drawFeature(Feature *feature, SymbolStyle* symbolStyle=0);
	//画Polygon
	void drawPolygon(mgeo::Geometry *geometry, SymbolStyle symbolStyle);
	//画点
	void drawPoint(mgeo::Geometry *geometry, SymbolStyle symbolStyle);
	//画线
	void drawPolyline(mgeo::Geometry *geometry, SymbolStyle symbolStyle);
	//画多面
	void drawMultiPolygon(mgeo::Geometry *geometry, SymbolStyle symbolStyle);

	vector<double> normalizeSymbol(SymbolStyle symbolStyle);

	void mouseDoubleClickEvent(QMouseEvent *event);//鼠标双击事件

	bool isMouseMovement;
signals:
	//向父类发送当前鼠标的世界坐标
	void sendCurrentPos(QPointF pos);
private:
	Ui::MyOpenGLWidget ui;
public:
	void drawIndex();
	void drawQuadTreeIndexNode(QuadNode* node);
};
#endif // MYOPENGLWIDGET_H