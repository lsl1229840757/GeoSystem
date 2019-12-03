#ifndef QPAINTERWIDGET_H
#define QPAINTERWIDGET_H
#include <QWidget>
#include "ui_qpainterwidget.h"
#include <qpainter.h>
class QPainterWidget : public QWidget
{
	Q_OBJECT

public:
	QPainterWidget(QWidget *parent = 0);
	~QPainterWidget();
protected:
	void paintEvent(QPaintEvent *event);
private:
	Ui::QPainterWidget ui;
};

#endif // QPAINTERWIDGET_H
