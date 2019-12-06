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
	//�Ժ��б�Ҫ��������������
	mouseFlag = 0;
}

void MouseZoomAction::mousePress(QPointF startPoint)
{
	//��ʼ��ѹ
	if (mouseFlag == 0) {
		mouseFlag++;
		this->startPoint = startPoint;
	}
}

void MouseZoomAction::mouseRelease(QPointF endPoint)
{
	//��ʼ�ͷ�
	if (mouseFlag == 1) {
		this->endPoint = endPoint;
		//��ʼ������η�Χ
		double maxX = startPoint.x() > endPoint.x() ? startPoint.x() : endPoint.x();
		double maxY = startPoint.y() > endPoint.y() ? startPoint.y() : endPoint.y();
		double minX = startPoint.x() < endPoint.x() ? startPoint.x() : endPoint.x();
		double minY = startPoint.y() < endPoint.y() ? startPoint.y() : endPoint.y();
		range = QRectF(QPointF(minX, maxY), QPointF(maxX, minY));
	}
}
