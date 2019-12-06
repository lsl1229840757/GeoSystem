#pragma once
#include <qcolor.h>
class SymbolStyle
{
public:
	SymbolStyle();
	~SymbolStyle();
	//对应SLD标签
	//point mark，暂时用不上
	float markSize;
	float markRotation;
	QString markWellKnownName;
	//stroke
	float strokeWidth;
	float strokeOpacity;
	QColor strokeColor;
	//fill
	float fillOpacity;
	QColor fillColor;
	//font,暂时用不上


};

