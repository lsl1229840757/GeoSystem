#include "MouseZoomAction.h"



MouseZoomAction::MouseZoomAction()
{

}

MouseZoomAction::MouseZoomAction(QRectF mapRange)
{
	//初始化
	this->mapRange = mapRange;
	zoomNum = 5;
}

MouseZoomAction::~MouseZoomAction()
{

}

QRectF MouseZoomAction::zoomIn(QPointF centerPos)
{
	//放大,最小到1
	if (zoomNum > minZoomNum)
		zoomNum--;
	//计算放大后实际矩形大小
	double ratio = (double)zoomNum / normalZoomNum;
	//得到放大后地图的宽和高
	double width = mapRange.width() * ratio;
	double height = mapRange.height() * ratio;
	QRectF zoomRange = QRectF(mapRange.left(), mapRange.top(), width, height);
	zoomRange.moveCenter(centerPos);
	return zoomRange;
}

QRectF MouseZoomAction::zoomOut(QPointF centerPos)
{
	//缩小
	if (zoomNum < maxZoomNum)
		zoomNum++;
	//计算放大后实际矩形大小
	double ratio = (double)zoomNum / normalZoomNum;
	//得到放大后地图的宽和高
	double width = mapRange.width() * ratio;
	double height = mapRange.height() * ratio;
	QRectF zoomRange = QRectF(mapRange.left(), mapRange.top(), width, height);
	zoomRange.moveCenter(centerPos);
	return zoomRange;
}

