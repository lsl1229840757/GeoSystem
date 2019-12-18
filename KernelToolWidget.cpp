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

void KernelToolWidget::showKernelResult(QRectF extent, vector<vector<double>> *result, double cellSize)
{
	GeoMap *kernelMap = new GeoMap;
	Layer *layer = new Layer;
	int xsizes = result->size(); //����
	int ysizes = result->back().size();  //����
	double *buffer = new double[xsizes*ysizes];
	for (int i = 0; i < xsizes*ysizes; i++)
	{
		buffer[i] = result->at(i % xsizes).at(i / xsizes);
	}
	//����Ҫ��
	for (int i = 0; i < ysizes; i++) {
		for (int j = 0; j < xsizes; j++) {
			//����һ������
			Feature* feature = new Feature;
			GeoPolygon * polygon = new GeoPolygon;
			pair<double, double> cd = KernelUtil::getCdByNum(extent.left(), extent.top(), cellSize, j, i);//���ĵ�
			GeoPoint* point1 = new GeoPoint(cd.first-cellSize/2, cd.second+cellSize/2);//���Ͻ�
			GeoPoint* point2 = new GeoPoint(cd.first + cellSize / 2, cd.second + cellSize / 2); //���ϽǶ�
			GeoPoint* point3 = new GeoPoint(cd.first + cellSize/2, cd.second - cellSize/2);//���½�
			GeoPoint* point4 = new GeoPoint(cd.first - cellSize / 2, cd.second - cellSize / 2);//���½�
			polygon->points.push_back(point1);
			polygon->points.push_back(point2);
			polygon->points.push_back(point3);
			polygon->points.push_back(point4);
			feature->geometry = polygon;
			layer->features.push_back(feature);
			layer->range = QRectF(point4->getX(), point4->getY(), cellSize, cellSize);
			feature->properties.insert("kernelResult", buffer[i*xsizes + j]);
		}
	}
	geoMap->addLayer(layer);
	MyOpenGLWidget *myGlWidgt = new MyOpenGLWidget(geoMap);
	myGlWidgt->show();
	myGlWidgt->setStyleByProperties(layer, "kernelResult");
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
	if (ui.lineEdit_cell->text().isEmpty() || ui.lineEdit_radius->text().isEmpty()){
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
		vector<double> pop;
		for (int i = 0; i < this->points.size(); i++) {
			pop.push_back(1);
		}
		//������ܶ�
		vector<vector<double>> outputMtx = kernel->computeKernelUsingPoint(this->extent, this->points, pop, this->cellSize, this->searchRadius, this->method);
		showKernelResult(this->extent, &outputMtx, this->cellSize);
		//���դ��GTiff
		//GdalUtil::writeGeoTiff(outputPath, extent, &outputMtx);
	}
}

