#include "MouseZoomAction.h"



MouseZoomAction::MouseZoomAction()
{
	mouseFlag = 0;
}


MouseZoomAction::~MouseZoomAction()
{
}

void MouseZoomAction::start()
{
	mouseFlag = 0;
}

void MouseZoomAction::end()
{
	//以后有必要可以添加清除操作
	mouseFlag = 0;
}

void MouseZoomAction::mousePress(QPointF startPoint)
{
	//开始按压
	if (mouseFlag == 0) {
		mouseFlag++;
		this->startPoint = startPoint;
	}
}

void MouseZoomAction::mouseRelease(QPointF endPoint)
{
	//开始释放
	if (mouseFlag == 1) {
		this->endPoint = endPoint;
		//开始计算矩形范围
		double maxX = startPoint.x() > endPoint.x() ? startPoint.x() : endPoint.x();
		double maxY = startPoint.y() > endPoint.y() ? startPoint.y() : endPoint.y();
		double minX = startPoint.x() < endPoint.x() ? startPoint.x() : endPoint.x();
		double minY = startPoint.y() < endPoint.y() ? startPoint.y() : endPoint.y();
		range = QRectF(QPointF(minX, maxY), QPointF(maxX, minY));
	}
}
