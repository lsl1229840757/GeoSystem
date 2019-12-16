#include "MyColorButton.h"
static QString FILLCOLOR = "fillColor";
static QString FILLOPACIRT = "fillOpacity";
static QString STROKEWIDTH = "strokeWidth";
static QString STROKECOLOR = "strokeColor";
static QString STROKEOPACITY = "strokeOpacity";
MyColorButton::MyColorButton(Feature* feature, QString type, QObject *parent): QPushButton()
{
	this->type = type;
	this->feature = feature;
	connect(this, SIGNAL(clicked()), this, SLOT(openColorDialog()));
}

MyColorButton::~MyColorButton()
{

}

void MyColorButton::openColorDialog() {
	QColorDialog *m_pColorDialog = new QColorDialog;//调色板显示类
	connect(m_pColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(getColor(QColor)));
	m_pColorDialog->exec();
}

void MyColorButton::getColor(QColor cor)
{
	//开始换颜色
	if (this->type.compare(FILLCOLOR) == 0) {
		//更换填充颜色
		feature->symbolStyle.fillColor = cor;
	}
	else if (this->type.compare(STROKECOLOR) == 0) {
		feature->symbolStyle.strokeColor = cor;
	}
	//修改自己背景颜色
	this->setStyleSheet("border-style:solid;background-color: rgb(" + QString::number(cor.red()) + "," +
		QString::number(cor.green()) + "," + QString::number(cor.blue()) + ")");
	emit mupdate();
}