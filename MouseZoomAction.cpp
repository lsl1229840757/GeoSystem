#include "MouseZoomAction.h"



MouseZoomAction::MouseZoomAction()
{

}

MouseZoomAction::MouseZoomAction(QRectF mapRange)
{
	//��ʼ��
	this->mapRange = mapRange;
	zoomNum = 5;
}

MouseZoomAction::~MouseZoomAction()
{

}

QRectF MouseZoomAction::zoomIn(QPointF centerPos)
{
	//�Ŵ�,��С��1
	if (zoomNum > minZoomNum)
		zoomNum--;
	//����Ŵ��ʵ�ʾ��δ�С
	double ratio = (double)zoomNum / normalZoomNum;
	//�õ��Ŵ���ͼ�Ŀ�͸�
	double width = mapRange.width() * ratio;
	double height = mapRange.height() * ratio;
	QRectF zoomRange = QRectF(mapRange.left(), mapRange.top(), width, height);
	zoomRange.moveCenter(centerPos);
	return zoomRange;
}

QRectF MouseZoomAction::zoomOut(QPointF centerPos)
{
	//��С
	if (zoomNum < maxZoomNum)
		zoomNum++;
	//����Ŵ��ʵ�ʾ��δ�С
	double ratio = (double)zoomNum / normalZoomNum;
	//�õ��Ŵ���ͼ�Ŀ�͸�
	double width = mapRange.width() * ratio;
	double height = mapRange.height() * ratio;
	QRectF zoomRange = QRectF(mapRange.left(), mapRange.top(), width, height);
	zoomRange.moveCenter(centerPos);
	return zoomRange;
}

