#include "Feature.h"


Feature::Feature(void)
{
	ogrGeom = NULL;
	geosGeom = NULL;
	isSelected = false; //是否选中
}


Feature::~Feature(void)
{
	if (ogrGeom != NULL)
	{
		delete ogrGeom;
		ogrGeom = NULL;
	}
	if (geosGeom != NULL)
	{
		delete geosGeom;
		geosGeom = NULL;
	}
}
