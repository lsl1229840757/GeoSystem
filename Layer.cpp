#include "Layer.h"

Layer::Layer(void)
{
	isVisble = true; //Ĭ�Ͽɼ�
	t = time(NULL);
}

Layer::~Layer(void)
{
}



QRectF Layer::getLayerRange()
{
	
	return QRectF();
}
