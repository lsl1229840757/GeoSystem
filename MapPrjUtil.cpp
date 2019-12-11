#include "MapPrjUtil.h"



MapPrjUtil::MapPrjUtil()
{
	prjx = 0;
	prjy = 0;
	e = sqrt((a*a - b * b) / (a * a));
}


MapPrjUtil::~MapPrjUtil()
{
}

QRectF MapPrjUtil::getPrjRange(QRectF dataRange)
{
	double max_x = dataRange.right();
	double min_x = dataRange.left();
	double min_y = dataRange.top();
	double max_y = dataRange.bottom();
	double prjmax_x, prjmax_y, prjmin_x, prjmin_y;
	getXY(max_x, max_y, &prjmax_x, &prjmax_y);
	getXY(min_x, min_y, &prjmin_x, &prjmin_y);
	this->prjMapRange = QRectF(QPointF(prjmin_x, prjmin_y), QPointF(prjmax_x, prjmax_y));
	return this->prjMapRange;
}
