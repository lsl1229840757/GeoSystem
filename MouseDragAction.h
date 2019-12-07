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
	//控制生命周期
	void begin(QPointF startPoint);
	vector<double> end(QPointF endPoint);
	void finish();
};

