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
	//开始
	this->startPoint = startPoint;
	dragFlag++;
}

vector<double> MouseDragAction::end(QPointF endPoint)
{
	if (dragFlag == 1) {
		//保证鼠标是点击之后才进行
		this->endPoint = endPoint;
		//计算移动向量
		double dx = endPoint.rx() - startPoint.rx();
		double dy = endPoint.ry() - startPoint.ry();
		//结束
		return vector<double>{dx ,dy};
	}
	return vector<double>{0, 0};
}

void MouseDragAction::finish()
{
	dragFlag = 0;
}