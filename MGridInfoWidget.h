#pragma once

#include <QWidget>
#include "ui_MGridInfoWidget.h"

class MGridInfoWidget : public QWidget
{
	Q_OBJECT

public:
	MGridInfoWidget(int colCount, int rowCount,QWidget *parent = Q_NULLPTR);
	~MGridInfoWidget();
	int colCount, rowCount;
	void setValue();
signals:
	void send(int colCount, int rowCount);
public slots:
	void finishSetInfo();
private:
	Ui::MGridInfoWidget ui;
};
