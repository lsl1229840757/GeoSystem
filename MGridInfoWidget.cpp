#include "MGridInfoWidget.h"

MGridInfoWidget::MGridInfoWidget(int colCount,int rowCount, QWidget *parent)
	: QWidget(parent)
{
	this->colCount = colCount;
	this->rowCount = rowCount;
	ui.setupUi(this);
	ui.spinBox_col->setValue(colCount);
	ui.spinBox_row->setValue(rowCount);
	connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(finishSetInfo()));
	connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(close()));
}

MGridInfoWidget::~MGridInfoWidget()
{
}
void MGridInfoWidget::setValue()
{
	this->colCount = ui.spinBox_col->value();
	this->rowCount = ui.spinBox_row->value();
}
void MGridInfoWidget::finishSetInfo()
{
	setValue();
	emit send(this->colCount, this->rowCount);
}