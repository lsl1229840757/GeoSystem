#pragma once
//���Ŵ������
#include <qpoint.h>
#include <qrect.h>
class MouseZoomAction
{
public:
	//����ʹ�õĶ�����������
	MouseZoomAction();
	MouseZoomAction(QRectF mapRange);
	~MouseZoomAction();
	//���Ƴ���
	static const int minZoomNum = 1;
	static const int maxZoomNum = 11;
	static const int normalZoomNum = 5;
	//������ż���, 1-11��,5��Ϊ������ʾ,С��5��Ϊ�Ŵ�,����5��Ϊ��С
	int zoomNum;
	//������ͼ��Χ
	QRectF mapRange;
	//������ǰ��һ�ηŴ�һ��
	QRectF zoomIn(QPointF centerPos);
	//��������һ����Сһ��
	QRectF zoomOut(QPointF centerPos);
};
