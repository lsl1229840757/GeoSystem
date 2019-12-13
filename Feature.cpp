#include "Feature.h"


Feature::Feature(void)
{
	ogrGrom = NULL;
	isSelected = false; //Ä¬ÈÏÃ»ÓÐ±»Ñ¡ÖÐ
}


Feature::~Feature(void)
{
	if (ogrGrom != NULL)
	{
		delete ogrGrom;
		ogrGrom = NULL;
	}
}
