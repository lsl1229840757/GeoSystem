#include "qpainterwidget.h"
#include <qdebug.h>
QPainterWidget::QPainterWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

QPainterWidget::~QPainterWidget()
{

}
void QPainterWidget::paintEvent(QPaintEvent *event){
	QPainter painter(this);
	painter.setRenderHint(QPainter:: Antialiasing, true);  //ÉèÖÃäÖÈ¾,Æô¶¯·´¾â³İ
	//painter.setPen(QPen(QColor(11,67,127),3));
	painter.setBrush(QColor(255, 160, 90));
	painter.drawEllipse(40.0, 40.0, 100.0, 100.0);   //°ë¾¶Îª50µÄÔ²
	QRect rect(QPoint(40,40),QPoint(80,80));
	painter.drawPolygon(rect);
}