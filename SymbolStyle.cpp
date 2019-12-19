#include "SymbolStyle.h"
#include <ctime>


SymbolStyle::SymbolStyle()
{
	srand(time(NULL));
	fillColor = QColor(rand() % 255, rand() % 255, rand() % 255);
	strokeWidth = 2.0;
	strokeColor = QColor(255, 255, 0);
	markSize = 5000;
}


SymbolStyle::~SymbolStyle()
{
}
