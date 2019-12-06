#include "GeoMap.h"

GeoMap::GeoMap(void)
{
	isVisible = true;//Ĭ�Ͽɼ�
}

GeoMap::~GeoMap(void)
{

}

void GeoMap::addLayer(Layer *layer){
	//���ͼ��
	layers.push_back(layer);
	//���·�Χ
	if(layers.size()==0){
		//��ʼ��range
		maxRange = layer->range;
	}else{
		//���±߽�
		maxRange = maxRange.united(layer->range);
	}
	//���±����ͳ���
	QPointF bottomLeft = this->maxRange.bottomLeft();
	QPointF topRight = this->maxRange.topRight();
	double max_x = topRight.x();
	double min_x = bottomLeft.x();
	double max_y = topRight.y();
	double min_y = bottomLeft.y();
	this->dx = abs(max_x-min_x);
	this->dy = abs(max_y-min_y);
	hwRatio = dy/dx;
	whRatio = dx/dy;
}

Layer* GeoMap::getLayerByName(string name){
	for(int i=0;i<layers.size();i++){
		if(layers[i]->name==name){
			return layers[i];	
		}	
	}
	return NULL;
}

QPointF GeoMap::NormalCd2worldCd(double x, double y)
{
	maxRange = maxRange.normalized();
	x = (this->dx * x) / 2 + maxRange.center().x();
	y = (this->dy * y) / 2 + maxRange.center().y();
	return QPointF(x, y);
}
