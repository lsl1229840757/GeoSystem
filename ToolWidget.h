#pragma once

#include <QWidget>
#include "ui_ToolWidget.h"

class ToolWidget : public QWidget
{
	Q_OBJECT

public:
	ToolWidget(QWidget *parent = Q_NULLPTR);
	~ToolWidget();

private:
	Ui::ToolWidget ui;
};
