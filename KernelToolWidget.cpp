#include "KernelToolWidget.h"

KernelToolWidget::KernelToolWidget(GeoMap *map, QWidget *parent)
	: ToolWidget(parent)
{
	isInit = true;
	method = NULL;
	this->geoMap = map;
	ui.setupUi(this);
	connect(this, SIGNAL(layerSelected(int,QString)), this, SLOT(addFieldComboItem(int)));
	connect(ui.comboBox_layer, SIGNAL(currentIndexChanged(int)), this, SLOT(addFieldComboItem(int)));
	connect(ui.comboBox_field, SIGNAL(currentIndexChanged(int)), this, SLOT(clearParam()));
	connect(ui.pushButton_cancel, SIGNAL(clicked()), this, SLOT(close()));
	//connect(ui.pushButton_out, SIGNAL(clicked()), this, SLOT(openFileDialog()));
	//connect(ui.pushButton_default, SIGNAL(clicked()), this, SLOT(calculDefault()));//计算默认值
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

void KernelToolWidget::showKernelResult(QRectF extent, vector<vector<double>> *result, double cellSize)
{
	GeoMap *kernelMap = new GeoMap;
	Layer *layer = new Layer;
	int xsizes = result->size(); //列数
	int ysizes = result->back().size();  //行数
	double *buffer = new double[xsizes*ysizes];
	for (int i = 0; i < xsizes*ysizes; i++)
	{
		buffer[i] = result->at(i % xsizes).at(i / xsizes);
	}
	//构造要素
	for (int i = 0; i < ysizes; i++) {
		for (int j = 0; j < xsizes; j++) {
			//构造一个矩形
			Feature* feature = new Feature;
			GeoPolygon * polygon = new GeoPolygon;
			pair<double, double> cd = KernelUtil::getCdByNum(extent.left(), extent.top(), cellSize, j, i);//中心点
			GeoPoint* point1 = new GeoPoint(cd.first-cellSize/2, cd.second+cellSize/2);//左上角
			GeoPoint* point2 = new GeoPoint(cd.first + cellSize / 2, cd.second + cellSize / 2); //右上角度
			GeoPoint* point3 = new GeoPoint(cd.first + cellSize/2, cd.second - cellSize/2);//右下角
			GeoPoint* point4 = new GeoPoint(cd.first - cellSize / 2, cd.second - cellSize / 2);//左下角
			polygon->points.push_back(point1);
			polygon->points.push_back(point2);
			polygon->points.push_back(point3);
			polygon->points.push_back(point4);
			feature->geometry = polygon;
			layer->features.push_back(feature);
			feature->properties.insert("kernelResult", buffer[i*xsizes + j]);
		}
	}
	kernelMap->layers.push_back(layer);
	kernelMap->maxRange = this->geoMap->maxRange;
	MyOpenGLWidget *myGlWidgt = new MyOpenGLWidget(kernelMap);
	myGlWidgt->setStyleByProperties(layer, "kernelResult");
	myGlWidgt->show();
}


void KernelToolWidget::addFieldComboItem(int itemID)
{
	if (isInit) {
		isInit = false;//初始化的时候也当做一次改变
		return;
	}
	// TODO: 在此处添加实现代码.
	//获取图层
	Layer *layer = geoMap->layers[itemID];
	//获取字段名
	QStringList listHeader;
	//添加默认
	listHeader << "None";
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
	kernelDistCalculate();
	this->population.clear();
	this->points.clear();
	//this->close();
}


void KernelToolWidget::openFileDialog()
{
	// TODO: 在此处添加实现代码.
	//QString savePath = QFileDialog::getSaveFileName(NULL, "Output Path", "",tr("*.tif"));
}

void KernelToolWidget::setParam()
{
	//记录所有参数
	int lyID = ui.comboBox_layer->currentIndex();
	Layer* layer = geoMap->layers[lyID];
	this->extent = layer->range;
	this->populationName = ui.comboBox_field->currentText();
	for (int i = 0; i < layer->features.size(); i++){
		//添加点
		if (GeometryType::GEOPOINT == layer->features.at(i)->geometry->getGeometryType()) {
			this->points.push_back((GeoPoint*)layer->features.at(i)->geometry);
		}
		//添加population
		if (ui.comboBox_field->currentText() == "None") {
			this->population.push_back(1);
		}
		else {
			this->population.push_back(layer->features.at(i)->attributes[populationName].toDouble());
		}
	}
	if (ui.lineEdit_radius->text().isEmpty()) {
		//若半径为空
		calculDefaultRadius();
	}
	if (ui.lineEdit_cell->text().isEmpty()) {
		//若有参数edit为空
		calculDefaultCellSize();
	}
	QString distTypeStr = ui.comboBox_dist->currentText();
	//判断距离类型
	if ("Euclidean distance" == distTypeStr){
		this->distType = DistanceType::EuclideanDistance;
		this->method = new EuclideanDistanceUtil;
	}else if("Geodetic distance" ==distTypeStr) {
		//暂无实现
		QMessageBox::information(NULL, "Infomation", "Not supported yet");
	}
	//this->outputPath = ui.lineEdit_out->text();
	this->cellSize = ui.lineEdit_cell->text().toDouble();
	this->searchRadius = ui.lineEdit_radius->text().toDouble();
	if (cellSize < 0 || searchRadius < 0) {
		//参数不能为负值
		QMessageBox::warning(NULL, "Error", "Cell size or search radius could not be negative");
		return;
	}
	emit finishSetParam();
}

void KernelToolWidget::calculDefaultRadius()
{
	//计算population相关
	double popSum = 0;
	for (int i = 0; i < this->population.size(); i++) {
		popSum += this->population[i];
	}
	//计算x,y平均中心
	double x_ = 0;
	double y_ = 0;
	for (int i = 0; i < this->points.size(); i++) {
		x_ += this->points[i]->x;
		y_ += this->points[i]->y;
	}
	x_ /= this->points.size();
	y_ /= this->points.size();
	//计算sd
	double dxSum = 0;
	double dySum = 0;
	for (int i = 0; i < this->points.size(); i++) {
		dxSum += this->population[i] * pow(this->points[i]->x - x_, 2);
		dySum += this->population[i] * pow(this->points[i]->y - y_, 2);
	}
	dxSum /= popSum;
	dySum /= popSum;
	double sd = sqrt(dxSum + dySum);
	//计算Dm
	vector<double> distances;
	for (int i = 0; i < this->points.size(); i++) {
		double x = this->points[i]->x;
		double y = this->points[i]->y;
		double distance = sqrt(pow(x - x_, 2) + pow(y - y_, 2));
		distance *= this->population[i];
		distances.push_back(distance);
	}
	sort(distances.begin(), distances.end());
	//取中值
	double dm = distances[distances.size()/2];
	double searchRadius = 0.9 * min(sd, sqrt(1 / log(2))*dm)*powf(popSum, 0.2);
	this->ui.lineEdit_radius->setText(QString::number(searchRadius));
}

void KernelToolWidget::calculDefaultCellSize()
{
	double cellSize = this->extent.width() < this->extent.height() ? this->extent.width() / 250 : this->extent.height() / 250;
	ui.lineEdit_cell->setText(QString::number(cellSize));
}

void KernelToolWidget::clearParam()
{
	ui.lineEdit_cell->setText("");
	ui.lineEdit_radius->setText("");

}


void KernelToolWidget::kernelDistCalculate()
{
	// TODO: 在此处添加实现代码.
	if (this->method != NULL) {
		KernelUtil *kernel = new EsriKernelUtil;
		//计算核密度
		vector<vector<double>> outputMtx = kernel->computeKernelUsingPoint(this->extent, this->points, this->population, this->cellSize, this->searchRadius, this->method);
		showKernelResult(this->extent, &outputMtx, this->cellSize);
		//输出栅格GTiff
		//GdalUtil::writeGeoTiff(outputPath, extent, &outputMtx);
	}
}

