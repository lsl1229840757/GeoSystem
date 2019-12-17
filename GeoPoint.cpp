#include "GeoPoint.h"


GeoPoint::GeoPoint(void)
{
	isFirstProjeted = true;
}

GeoPoint::GeoPoint(double x, double y)
{
	isFirstProjeted = true;
	this->x = x;
	this->y = y;
}


GeoPoint::~GeoPoint(void)
{
}

GeometryType GeoPoint::getGeometryType(){

	return GeometryType::GEOPOINT;
}

// //获取点坐标
pair<double,double> GeoPoint::getPtCoor(MapPrjUtil* mapPrj, bool isLastPt, bool &isFeaFirstPrj)
{
	// TODO: 在此处添加实现代码.
	if (mapPrj!=NULL) {
		//判断pt所在feature是否为第一次投影，并储存投影坐标
		if (isFeaFirstPrj) {
			if (isLastPt) isFeaFirstPrj = false;  //feature的最后一个点改变投影状态
			//计算投影坐标
			double prjx, prjy;
			mapPrj->getXY(this->x, this->y, &prjx, &prjy);
			this->prjx = prjx;
			this->prjy = prjy;
			return pair<double, double>(this->prjx, this->prjy);
		}
		else {
			return pair<double, double>(this->prjx, this->prjy);
		}
	}
	else {
		return pair<double,double>(x,y);
	}
	
}

double GeoPoint::getX()
{
	if (!isProjeted) {
		return x;
	}
	else {
		return prjx;
	}
}

double GeoPoint::getY()
{
	if (!isProjeted) {
		return y;
	}
	else {
		return prjy;
	}
}
