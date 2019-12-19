#include "GeoMap.h"

GeoMap::GeoMap(void)
{
	isVisible = true;//默认可见
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


// //判断是否投影，获取地图Range
QRectF GeoMap::getMapRange()
{
	// TODO: 在此处添加实现代码.
	QRectF normalRange;
	if (this->mapPrj != NULL) {
		QRectF prjRange = this->mapPrj->getPrjRange(this->maxRange.normalized());
		normalRange = prjRange.normalized();  //将地图范围规范化
	}
	else {
		normalRange = this->maxRange.normalized();  //先将地图范围规范化
	}
	return normalRange;
}


// //重置所有Features的投影状态
void GeoMap::resetFeaturePrjStatus()
{
	// TODO: 在此处添加实现代码.
	if (this->mapPrj != NULL)
	{
		for (int i = 0; i < this->layers.size(); i++)
		{
			Layer *layer = this->layers.at(i);
			for (int j = 0; j < layer->features.size(); j++)
			{
				//重置isFirstProjeted,回复第一次投影状态
				//layer->features.at(j)->isFirstProjeted = true;
				layer->features[i]->geometry->isProjeted = false;
			}
		}
	}
}


// //判断是否存在投影
bool GeoMap::mapPrjEmpty()
{
	// TODO: 在此处添加实现代码.
	if (this->mapPrj != NULL)
	{
		return false;
	}
	else 
	{
		return true;
	}
	
}
