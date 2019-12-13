#include "Feature.h"


Feature::Feature(void)
{
	ogrGrom = NULL;
}


Feature::~Feature(void)
{
	if (ogrGrom != NULL)
	{
		delete ogrGrom;
		ogrGrom = NULL;
	}
}
