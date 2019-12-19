#include "GeoMap.h"

GeoMap::GeoMap(void)
{
	isVisible = true;//Ĭ�Ͽɼ�
	mapPrj = NULL;
	index = NULL;
}

GeoMap::~GeoMap(void)
{
	if (mapPrj != NULL)
	{
		delete mapPrj;
		mapPrj = NULL;
	}
	if (index != NULL)
	{
		delete index;
		index = NULL;
	}
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



void GeoMap::setMapPrj(MapPrjType type)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (mapPrj != NULL)
	{
		qDebug() << "You have defined map projection";
		return;
	}
	if (MapPrjType::LAMBERT == type)
	{
		this->mapPrj = new MapPrjLambert;
	}
	else if (MapPrjType::MERCATOR == type)
	{
		this->mapPrj = new MapPrjMercator;
	}
}


// //�ж��Ƿ�ͶӰ����ȡ��ͼRange
QRectF GeoMap::getMapRange()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	QRectF normalRange;
	if (this->mapPrj != NULL) {
		QRectF prjRange = this->mapPrj->getPrjRange(this->maxRange.normalized());
		normalRange = prjRange.normalized();  //����ͼ��Χ�淶��
	}
	else {
		normalRange = this->maxRange.normalized();  //�Ƚ���ͼ��Χ�淶��
	}
	return normalRange;
}


// //��������Features��ͶӰ״̬
void GeoMap::resetFeaturePrjStatus()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (this->mapPrj != NULL)
	{
		for (int i = 0; i < this->layers.size(); i++)
		{
			Layer *layer = this->layers.at(i);
			for (int j = 0; j < layer->features.size(); j++)
			{
				//����isFirstProjeted,�ظ���һ��ͶӰ״̬
				//layer->features.at(j)->isFirstProjeted = true;
				layer->features[i]->geometry->isProjeted = false;
			}
		}
	}
}


// //�ж��Ƿ����ͶӰ
bool GeoMap::mapPrjEmpty()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (this->mapPrj != NULL)
	{
		return false;
	}
	else 
	{
		return true;
	}
	
}
