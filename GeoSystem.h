#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GeoSystem.h"

class GeoSystem : public QMainWindow
{
	Q_OBJECT

public:
	GeoSystem(QWidget *parent = Q_NULLPTR);

private:
	Ui::GeoSystemClass ui;
};
