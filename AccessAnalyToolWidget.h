#pragma once

#include <ToolWidget.h>
#include "ui_AccessAnalyToolWidget.h"
#include "GeoMap.h"
#include "qcombobox.h"
#include "qcheckbox.h"
#include "qslider.h"
#include "GeoMap.h"
#include "qdebug.h"
#include "AccessibilityUtil.h"
class AccessAnalyToolWidget : public ToolWidget
{
	Q_OBJECT

public:
	AccessAnalyToolWidget(GeoMap* map, QWidget *parent = Q_NULLPTR);
	~AccessAnalyToolWidget();
	GeoMap* geoMap;
	Layer* demandLayer;  //需求图层
	vector<Layer*> supplyLayers;  //供给
	Layer* road;  //路网
public slots:
	void addLayerComboItem(QComboBox* comboBox);
	void setComboItemToParamDemand(int itemID);
	void setComboItemToParamRoad(int itemID);

	void demandLayerChanged(int itemID);
	void roadLayerChanged(int itemID);
	void setCheckBoxToParam();
	void finishSetParam();
signals:
	void sendLayer(int itemID, QString layerName);
	void finishAnalyse(vector<double> result, GeoMap *map, Layer *layer);
private:
	Ui::AccessAnalyToolWidget ui;
public:
	
	
	void addCheckBox();
};
