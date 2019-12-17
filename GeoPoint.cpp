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

// //��ȡ������
pair<double,double> GeoPoint::getPtCoor(MapPrjUtil* mapPrj, bool isLastPt, bool &isFeaFirstPrj)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (mapPrj!=NULL) {
		//�ж�pt����feature�Ƿ�Ϊ��һ��ͶӰ��������ͶӰ����
		if (isFeaFirstPrj) {
			if (isLastPt) isFeaFirstPrj = false;  //feature�����һ����ı�ͶӰ״̬
			//����ͶӰ����
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
