#include "MouseDragAction.h"



MouseDragAction::MouseDragAction()
{
	dragFlag = 0;
}


MouseDragAction::~MouseDragAction()
{
}

void MouseDragAction::begin(QPointF startPoint)
{
	//��ʼ
	this->startPoint = startPoint;
	dragFlag++;
}

vector<double> MouseDragAction::end(QPointF endPoint)
{
	if (dragFlag == 1) {
		//��֤����ǵ��֮��Ž���
		this->endPoint = endPoint;
		//�����ƶ�����
		double dx = endPoint.rx() - startPoint.rx();
		double dy = endPoint.ry() - startPoint.ry();
		//����
		return vector<double>{dx ,dy};
	}
	return vector<double>{0, 0};
}

void MouseDragAction::finish()
{
	dragFlag = 0;
}