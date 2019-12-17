#pragma once

#include "ToolWidget.h"
#include "ui_KernelToolWidget.h"
#include "GeoMap.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "GeoPoint.h"

class KernelToolWidget : public ToolWidget
{
	Q_OBJECT

public:
	KernelToolWidget(GeoMap *map,QWidget *parent = Q_NULLPTR);
	~KernelToolWidget();
	GeoMap *geoMap;
	QRectF extent;
	QString populationName;
	QString outputPath;
	vector<double> population;
	vector<GeoPoint*> points;
	double cellSize;
	double searchRadius;
	QString distType;
private:
	Ui::KernelToolWidget ui;
public:
	void addLayerComboItem();
	void addDistType();
signals:
	void layerSelected(int itemID, QString lyName);  //选中的图层index和名称
	void finishSetParam();
private:
	void sendLayer(int itemID, QString lyName);

public slots:
	void okSetParam();
	void addFieldComboItem(int itemID);
	void openFileDialog();
	void setParam();
public:
	
};
