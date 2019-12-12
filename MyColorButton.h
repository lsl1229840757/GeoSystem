#pragma once

#include <QPushButton>
#include "Feature.h"
#include "qcolordialog.h"
class MyColorButton : public QPushButton
{
	Q_OBJECT
public:
	MyColorButton(Feature* feature, QString type,QObject *parent=0);
	~MyColorButton();
	QString type;
	Feature *feature;
public slots:
	void openColorDialog();
	void getColor(QColor cor);
signals:
	void mupdate();
};
