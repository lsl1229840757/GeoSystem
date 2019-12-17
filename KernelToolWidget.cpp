#include "KernelToolWidget.h"

KernelToolWidget::KernelToolWidget(GeoMap *map,QWidget *parent)
	: ToolWidget(parent)
{
	this->geoMap = map;
	ui.setupUi(this);
	connect(this, SIGNAL(layerSelected(int,QString)), this, SLOT(addFieldComboItem(int)));
	connect(ui.comboBox_layer, SIGNAL(currentIndexChanged(int)), this, SLOT(addFieldComboItem(int)));
	connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_out, SIGNAL(clicked()), this, SLOT(openFileDialog()));
	connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(setParam()));
	connect(this, SIGNAL(finishSetParam()), this, SLOT(okSetParam()));
	addLayerComboItem();  //后发信号
	addDistType();
}

KernelToolWidget::~KernelToolWidget()
{
	
}


void KernelToolWidget::addLayerComboItem()
{
	// TODO: 在此处添加实现代码.
	ui.comboBox_layer->clear();
	QStringList layerList;
	for (int i = 0; i < geoMap->layers.size(); i++)
	{
		layerList << QString::fromStdString(geoMap->layers.at(i)->name);
	}
	ui.comboBox_layer->addItems(layerList);
	//返回当前图层index,和name
	int itemID = ui.comboBox_layer->currentIndex();
	QString layerName = ui.comboBox_layer->currentText();
	sendLayer(itemID,layerName);
}

void KernelToolWidget::addDistType()
{
	ui.comboBox_dist->addItem(QString("Euclidean distance"));
	ui.comboBox_dist->addItem(QString("Geodetic distance"));
}


void KernelToolWidget::addFieldComboItem(int itemID)
{
	// TODO: 在此处添加实现代码.
	//获取图层
	Layer *layer = geoMap->layers[itemID];
	//获取字段名
	QStringList listHeader;
	QVariantMap::iterator iter;
	for (iter = layer->features.back()->attributes.begin(); iter != layer->features.back()->attributes.end(); iter++) {
		//迭代记录字段名
		listHeader << iter.key();
	}
	ui.comboBox_field->addItems(listHeader);
}

void KernelToolWidget::sendLayer(int itemID, QString lyName)
{
	emit layerSelected(itemID, lyName);
}


void KernelToolWidget::okSetParam()
{
	// TODO: 在此处添加实现代码.
	
	this->close();
}


void KernelToolWidget::openFileDialog()
{
	// TODO: 在此处添加实现代码.
	QString savePath = QFileDialog::getSaveFileName(NULL, "Output Path", "",tr("*.tif"));
	ui.lineEdit_out->setText(savePath);
}

void KernelToolWidget::setParam()
{
	if (ui.lineEdit_cell->text().isEmpty() || ui.lineEdit_out->text().isEmpty() || ui.lineEdit_radius->text().isEmpty()){
		QMessageBox::warning(NULL, "Error", "Parameters could not be empty");
		return;
	}
	int lyID = ui.comboBox_layer->currentIndex();
	Layer* layer = geoMap->layers[lyID];
	this->extent = layer->range;
	this->populationName = ui.comboBox_field->currentText();
	for (int i = 0; i < layer->features.size(); i++){
		//添加点
		if (GeometryType::GEOPOINT == layer->features.at(i)->geometry->getGeometryType()) {
			this->points.push_back((GeoPoint*)layer->features.at(i)->geometry);
		}
		this->population.push_back(layer->features.at(i)->attributes[populationName].toDouble());
	}
	this->distType = ui.comboBox_dist->currentText();
	this->cellSize = ui.lineEdit_cell->text().toDouble();
	this->searchRadius = ui.lineEdit_radius->text().toDouble();
	if (cellSize < 0 || searchRadius < 0) {
		QMessageBox::warning(NULL, "Error", "Cell size or search radius could not be negative");
		return;
	}
	emit finishSetParam();
}
