#include "MyStyleLine.h"
static QString MARKSIZE = "markSize";
static QString STROKEWIDTH = "strokeWidth";

MyStyleLine::MyStyleLine(Feature* feature, QString type, QObject *parent)
	: QLineEdit()
{
	this->type = type;
	this->feature = feature;
	connect(this, SIGNAL(returnPressed()), this, SLOT(updSize()));

}

MyStyleLine::~MyStyleLine()
{
}

void MyStyleLine::updSize()
{
	if (this->type.compare(MARKSIZE)==0) {
		double num = this->text().toDouble();
		feature->symbolStyle.markSize = num;
	}
	else if (this->type.compare(STROKEWIDTH)==0) {
		double num = this->text().toDouble();
		feature->symbolStyle.strokeWidth = num;
	}
	emit mupdate();
}
