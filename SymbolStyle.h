#pragma once
#include <qcolor.h>
class SymbolStyle
{
public:
	SymbolStyle();
	~SymbolStyle();
	//��ӦSLD��ǩ
	//point mark����ʱ�ò���
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
	//font,��ʱ�ò���


};

