#include "AccessAnalyToolWidget.h"

AccessAnalyToolWidget::AccessAnalyToolWidget(GeoMap* map,QWidget *parent)
	: ToolWidget(parent)
{
	this->geoMap = map;
	road = NULL;
	demandLayer = NULL;
	ui.setupUi(this);
	//ui.comboBox_demand->setCurrentText("");
	//ui.comboBox_road->setCurrentText("");
	//connect(ui.comboBox_demand, SIGNAL(currentIndexChanged(int)), this, SLOT(demandLayerChanged(int)));
	//connect(ui.comboBox_road, SIGNAL(currentIndexChanged(int)), this, SLOT(roadLayerChanged(int)));
	connect(ui.comboBox_demand, SIGNAL(currentIndexChanged(int)), this, SLOT(setComboItemToParam(int)));
	connect(ui.comboBox_road, SIGNAL(currentIndexChanged(int)), this, SLOT(setComboItemToParam(int)));
	connect(ui.pushButton_supply, SIGNAL(clicked()), this, SLOT(setCheckBoxToParam()));
	connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(finishSetParam()));
	addLayerComboItem(ui.comboBox_demand);
	addLayerComboItem(ui.comboBox_road);
	addCheckBox();
	
}

AccessAnalyToolWidget::~AccessAnalyToolWidget()
{
}


void AccessAnalyToolWidget::addLayerComboItem(QComboBox *comboBox)
{
	// TODO: 在此处添加实现代码.
	comboBox->clear();
	comboBox->addItem("None");
	QStringList layerList;
	for (int i = 0; i < geoMap->layers.size(); i++)
	{
		layerList << QString::fromStdString(geoMap->layers.at(i)->name);
	}
	comboBox->addItems(layerList);
	//返回当前图层index,和name
	//int itemID = comboBox->currentIndex();
	//QString layerName = comboBox->currentText();
	//emit sendLayer(itemID, layerName);
}



void AccessAnalyToolWidget::setComboItemToParam(int itemID)
{
	// TODO: 在此处添加实现代码.
	if (itemID != 0) itemID -= 1;   //0位置为None
	this->demandLayer = geoMap->layers[itemID];
	this->road = geoMap->layers[itemID];
}


void AccessAnalyToolWidget::demandLayerChanged(int itemID)
{
	// TODO: 在此处添加实现代码.
	//int itemID = ui.comboBox_demand->currentIndex();
	QString itemName = ui.comboBox_demand->currentText();
	emit sendLayer(itemID, itemName);
}

void AccessAnalyToolWidget::roadLayerChanged(int itemID)
{
	QString itemName = ui.comboBox_road->currentText();
	emit sendLayer(itemID, itemName);
}

void AccessAnalyToolWidget::setCheckBoxToParam()
{
	this->supplyLayers.clear();
	QListWidget *listWidget = ui.listWidget;
	for (int i = 0; i <listWidget->count(); i++)
	{
		QListWidgetItem *item = listWidget->item(i);
		if (((QCheckBox*)listWidget->itemWidget(item))->isChecked())
		{
			this->supplyLayers.push_back(geoMap->layers[i]);
		}
	}
}

void AccessAnalyToolWidget::finishSetParam()
{
	qDebug()<<QString::fromStdString(demandLayer->name) << "   " << QString::fromStdString(road->name);
	for(int i=0;i<supplyLayers.size();i++)
		qDebug() << QString::fromStdString(supplyLayers.at(i)->name);
	this->close();  
}


void AccessAnalyToolWidget::addCheckBox()
{
	// TODO: 在此处添加实现代码.
	for (int i = 0; i < geoMap->layers.size(); i++)
	{
		QCheckBox *checkBox = new QCheckBox;
		QListWidgetItem *item = new QListWidgetItem;
		checkBox->setText(QString::fromStdString(geoMap->layers[i]->name));
		checkBox->setMinimumSize(QSize(60, 30));   //widthheight
		ui.listWidget->addItem(item);
		ui.listWidget->setItemWidget(item,checkBox);
	}
	
}
