#include "KernelToolWidget.h"

KernelToolWidget::KernelToolWidget(GeoMap *map,QWidget *parent)
	: ToolWidget(parent)
{
	method = NULL;
	this->geoMap = map;
	ui.setupUi(this);
	connect(this, SIGNAL(layerSelected(int,QString)), this, SLOT(addFieldComboItem(int)));
	connect(ui.comboBox_layer, SIGNAL(currentIndexChanged(int)), this, SLOT(addFieldComboItem(int)));
	connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButton_out, SIGNAL(clicked()), this, SLOT(openFileDialog()));
	connect(ui.pushButton_ok, SIGNAL(clicked()), this, SLOT(setParam()));
	connect(this, SIGNAL(finishSetParam()), this, SLOT(okSetParam()));
	addLayerComboItem();  //���ź�
	addDistType();
}

KernelToolWidget::~KernelToolWidget()
{
	
}


void KernelToolWidget::addLayerComboItem()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	ui.comboBox_layer->clear();
	QStringList layerList;
	for (int i = 0; i < geoMap->layers.size(); i++)
	{
		layerList << QString::fromStdString(geoMap->layers.at(i)->name);
	}
	ui.comboBox_layer->addItems(layerList);
	//���ص�ǰͼ��index,��name
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
	// TODO: �ڴ˴����ʵ�ִ���.
	//��ȡͼ��
	Layer *layer = geoMap->layers[itemID];
	//��ȡ�ֶ���
	QStringList listHeader;
	QVariantMap::iterator iter;
	for (iter = layer->features.back()->attributes.begin(); iter != layer->features.back()->attributes.end(); iter++) {
		//������¼�ֶ���
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
	// TODO: �ڴ˴����ʵ�ִ���.
	kernelDistCalculate();
	this->close();
}


void KernelToolWidget::openFileDialog()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	QString savePath = QFileDialog::getSaveFileName(NULL, "Output Path", "",tr("*.tif"));
	ui.lineEdit_out->setText(savePath);
}

void KernelToolWidget::setParam()
{
	//��¼���в���
	if (ui.lineEdit_cell->text().isEmpty() || ui.lineEdit_out->text().isEmpty() || ui.lineEdit_radius->text().isEmpty()){
		//���в���editΪ��
		QMessageBox::warning(NULL, "Error", "Parameters could not be empty");
		return;
	}
	int lyID = ui.comboBox_layer->currentIndex();
	Layer* layer = geoMap->layers[lyID];
	this->extent = layer->range;
	this->populationName = ui.comboBox_field->currentText();
	for (int i = 0; i < layer->features.size(); i++){
		//��ӵ�
		if (GeometryType::GEOPOINT == layer->features.at(i)->geometry->getGeometryType()) {
			this->points.push_back((GeoPoint*)layer->features.at(i)->geometry);
		}
		this->population.push_back(layer->features.at(i)->attributes[populationName].toDouble());
	}
	QString distTypeStr = ui.comboBox_dist->currentText();
	//�жϾ�������
	if ("Euclidean distance" == distTypeStr){
		this->distType = DistanceType::EuclideanDistance;
		this->method = new EuclideanDistanceUtil;
	}else if("Geodetic distance" ==distTypeStr) {
		//����ʵ��
		QMessageBox::information(NULL, "Infomation", "Not supported yet");
	}
	this->outputPath = ui.lineEdit_out->text();
	this->cellSize = ui.lineEdit_cell->text().toDouble();
	this->searchRadius = ui.lineEdit_radius->text().toDouble();
	if (cellSize < 0 || searchRadius < 0) {
		//��������Ϊ��ֵ
		QMessageBox::warning(NULL, "Error", "Cell size or search radius could not be negative");
		return;
	}
	emit finishSetParam();
}


void KernelToolWidget::kernelDistCalculate()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (this->method != NULL) {
		KernelUtil *kernel = new EsriKernelUtil;
		//������ܶ�
		vector<vector<double>> outputMtx = kernel->computeKernelUsingPoint(this->extent, this->points, this->population, this->cellSize, this->searchRadius, this->method);
		//���դ��GTiff
		GdalUtil::writeGeoTiff(outputPath, extent, &outputMtx);
	}

}
