#pragma once
//鼠标放大操作类
#include <qpoint.h>
#include <qrect.h>
class MouseZoomAction
{
public:
	//这里使用的都是世界坐标
	MouseZoomAction();
	MouseZoomAction(QRectF mapRange);
	~MouseZoomAction();
	//控制常量
	static const int minZoomNum = 1;
	static const int maxZoomNum = 11;
	static const int normalZoomNum = 5;
	//鼠标缩放级数, 1-11级,5级为正常显示,小于5级为放大,大于5级为缩小
	int zoomNum;
	//真正地图范围
	QRectF mapRange;
	//滑轮向前滚一次放大一级
	QRectF zoomIn(QPointF centerPos);
	//滑轮向后滚一次缩小一级
	QRectF zoomOut(QPointF centerPos);
};
