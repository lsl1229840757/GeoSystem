#include "Feature.h"


Feature::Feature(void)
{
	ogrGrom = NULL;
	isSelected = false; //是否选中
}


Feature::~Feature(void)
{
	if (ogrGrom != NULL)
	{
		delete ogrGrom;
		ogrGrom = NULL;
	}
}
