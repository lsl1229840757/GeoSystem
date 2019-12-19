#include "Layer.h"

Layer::Layer(void)
{
	isVisble = true; //д╛хо©и╪Ш
	t = time(NULL);
}

Layer::~Layer(void)
{
}



QRectF Layer::getLayerRange()
{
	
	return QRectF();
}
