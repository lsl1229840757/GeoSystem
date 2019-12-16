#pragma once

#include <QTableWidget>
#include "ui_MyTableWidget.h"
#include "Layer.h"

class MyTableWidget : public QTableWidget
{
	Q_OBJECT

public:
	MyTableWidget(int rows,int cols,Layer* layer, QWidget *parent = Q_NULLPTR);
	~MyTableWidget();
	int cols, rows;

private:
	Ui::MyTableWidget ui;
public:
	void initTable();
	Layer* layer;
	void addData();
};
