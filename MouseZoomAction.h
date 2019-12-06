#pragma once
//���Ŵ������
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
	//����������ز���
	void mousePress(QPointF startPoint);
	void mouseRelease(QPointF endPoint);
	QRectF range;
	//������������
	void start();
	void end();
};

