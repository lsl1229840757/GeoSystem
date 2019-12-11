#include "GeoMap.h"

GeoMap::GeoMap(void)
{
	isVisible = true;//默认可见
	mapPrj = NULL;
}

GeoMap::~GeoMap(void)
{
	if (mapPrj != NULL)
	{
		delete mapPrj;
		mapPrj = NULL;
	}
}

void GeoMap::addLayer(Layer *layer){
	//添加图层
	layers.push_back(layer);
	//更新范围
	if(layers.size()==0){
		//初始化range
		maxRange = layer->range;
	}else{
		//跟新边界
		maxRange = maxRange.united(layer->range);
	}
	//更新比例和长宽
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
	// TODO: 在此处添加实现代码.
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
