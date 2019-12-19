#pragma once

#include <QLineEdit>
#include "Feature.h"
class MyStyleLine : public QLineEdit
{
	Q_OBJECT

public:
	MyStyleLine(Feature* feature, QString type, QObject *parent=0);
	~MyStyleLine();
	QString type;
	Feature *feature;

public slots:
	void updSize();
signals:
	void mupdate();
};
