#pragma once

#include <QWidget>
#include "ui_StyleWidget.h"
#include <qlabel.h>
#include <QFormLayout>
#include <qlineedit.h>
#include <qpushbutton.h>
#include "Layer.h"
#include "qcolordialog.h"
#include "qdebug.h"
#include "MyColorButton.h"
class StyleWidget : public QWidget
{
	Q_OBJECT

public:
	StyleWidget(int mapIndex, int layerIndex, Layer *layer, QWidget *parent = Q_NULLPTR);
	~StyleWidget();
	Layer *layer;
	int mapIndex;
	int layerIndex;
	QTreeWidgetItem * addTreeTopLevel(int id, QString name);
	QTreeWidgetItem * addTreeNode(QTreeWidgetItem *parent, QString type, SymbolStyle symbol, Feature* feature);
public slots:
	void finishQColorDialog();
signals:
	void update(int mapIndex, int LayerIndex);
private:
	Ui::StyleWidget ui;
};
