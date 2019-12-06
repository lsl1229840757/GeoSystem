#pragma once
//鼠标放大操作类
#include <qpoint.h>
#include <qrect.h>
class MouseZoomAction
{
public:
	MouseZoomAction();
	~MouseZoomAction();
	QPointF startPoint;
	QPointF endPoint;
	int mouseFlag;
	//生命期中相关操作
	void mousePress(QPointF startPoint);
	void mouseRelease(QPointF endPoint);
	QRectF range;
	//控制生命周期
	void start();
	void end();
};

