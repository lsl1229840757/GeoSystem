#include "MapPrjUtil.h"



MapPrjUtil::MapPrjUtil()
{
	prjx = 0;
	prjy = 0;
	e = sqrt((a*a - b * b) / (a * a));
	mapPrjChanged = false;
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


QRectF MapPrjUtil::getPrimRange(QRectF prjRange)
{
	// TODO: 在此处添加实现代码.
	double max_x = prjRange.right();
	double min_x = prjRange.left();
	double min_y = prjRange.top();
	double max_y = prjRange.bottom();
	double primmax_x, primmax_y, primmin_x, primmin_y;
	getBL(max_x, max_y, &primmax_x, &primmax_y);
	getBL(min_x, min_y, &primmin_x, &primmin_y);
	return QRectF(QPointF(primmin_x, primmin_y), QPointF(primmax_x, primmax_y));
}
