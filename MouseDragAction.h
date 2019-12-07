#pragma once
#include <qpoint.h>
#include <vector>
using namespace std;
class MouseDragAction
{
public:
	MouseDragAction();
	~MouseDragAction();
	int dragFlag;
	QPointF startPoint;
	QPointF endPoint;
	//������������
	void begin(QPointF startPoint);
	vector<double> end(QPointF endPoint);
	void finish();
};

