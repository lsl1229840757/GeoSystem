#include "geojsonparese.h"
//定义一些数据绑定全局变量方便修改
static int VISIBLE_COLUMN = 0;
static int ID_COLUMN = 1;
static int NAME_COLUMN = 2;

GeoJsonParese::GeoJsonParese(QWidget *parent)
	: QMainWindow(parent)
{
	log += "Lauch Successfully\n";
	ui.setupUi(this);
	ui.tabWidget->clear();
	ui.tabWidget->setTabsClosable(true);
	ui.treeWidget->clear();
	ui.treeWidget->setColumnCount(3);
    ui.treeWidget->setHeaderLabels(QStringList()<<"Visible"<<"ID"<<"Name");
	connect(ui.action_2,SIGNAL(triggered()), this,SLOT(parseGeoJson()));
	connect(ui.action_shp,SIGNAL(triggered()), this,SLOT(readShp()));
	connect(ui.actionShp_GeoJson,SIGNAL(triggered()), this,SLOT(shp2GeoJson()));
	connect(ui.action_PostgreSQL,SIGNAL(triggered()),this,SLOT(readFromPgsql()));
	connect(ui.treeWidget,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(onPressed(QPoint)));
	connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

	//添加检索框
	
	QPushButton *pSearchButton = new QPushButton();

	pSearchButton->setCursor(Qt::PointingHandCursor);
	pSearchButton->setFixedSize(22, 22);
	pSearchButton->setToolTip(QStringLiteral("搜索"));
	pSearchButton->setStyleSheet("QPushButton{border-image:url(:/images/icon_search_normal); background:transparent;} \
                                     QPushButton:hover{border-image:url(:/images/icon_search_hover)} \
                                     QPushButton:pressed{border-image:url(:/images/icon_search_press)}");

	//防止文本框输入内容位于按钮之下
	QMargins margins = ui.lineEdit->textMargins();
	ui.lineEdit->setTextMargins(margins.left(), margins.top(), pSearchButton->width(), margins.bottom());
	ui.lineEdit->setPlaceholderText(QStringLiteral("请输入搜索内容"));

	QHBoxLayout *pSearchLayout = new QHBoxLayout();
	pSearchLayout->addStretch();
	pSearchLayout->addWidget(pSearchButton);
	pSearchLayout->setSpacing(0);
	pSearchLayout->setContentsMargins(0, 0, 0, 0);
	ui.lineEdit->setLayout(pSearchLayout);
	//添加搜索事件
	connect(pSearchButton, SIGNAL(clicked(bool)), this, SLOT(searchRegion()));

	//添加是否可视控制
	connect(ui.treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
	//初始化数据源
	dataSource = new GeoDataSource;

	//打印日志
	ui.textBrowser->setText(log);
}

GeoJsonParese::~GeoJsonParese()
{

}

/*
槽函数
*/
//解析json
void GeoJsonParese::parseGeoJson(){
	QString filePath = QFileDialog::getOpenFileName(this, "GeoJson Parse", "", "GeoJson Files(*.geojson)");
	if(!filePath.isEmpty()){
		GeoMap* geoMap = JsonUtil::parseGeoJson(filePath);
		//设置地图投影
		//判断是否为经纬度
		double &onex = geoMap->maxRange.topRight().rx();
		double &oney = geoMap->maxRange.topRight().ry();
		if (fabs(onex) <= 360 && fabs(oney) <= 90)
			geoMap->setMapPrj(MapPrjType::MERCATOR);
		//QString str = QString::number(feature.geometry->coordinates[0].toDouble())+","+QString::number(feature.geometry->coordinates[1].toDouble());
		//ui.textBrowser->setText("coordinates:["+str+"]");
		dataSource->geoMaps.push_back(geoMap);
		//添加节点
		addTreeTopLevel(geoMap, dataSource->geoMaps.size()-1, QString::fromStdString(geoMap->name));
		log += "GeoJson Load Successfully!\n";
		ui.textBrowser->setText(log);
	}
}

//读取shp
void GeoJsonParese::readShp(){
	QString filePath = QFileDialog::getOpenFileName(this, "ShapeFile Parse", "", "ShapeFile Files(*.shp)");
	if(!filePath.isEmpty()){
		OGRDataSource* poDS = GdalUtil::readFromGeoJson(filePath);
		//TODO 报错机制
		GeoMap *geoMap = GdalUtil::OGRDataSource2Map(poDS);
		//设置地图投影前判断是否为经纬度，否则不投影
		double &onex = geoMap->maxRange.topRight().rx();
		double &oney = geoMap->maxRange.topRight().ry();
		if (fabs(onex) <= 360 && fabs(oney) <= 90)
			geoMap->setMapPrj(MapPrjType::MERCATOR); //默认使用墨卡托
		dataSource->geoMaps.push_back(geoMap);
		//添加节点
		addTreeTopLevel(geoMap, dataSource->geoMaps.size() - 1, QString::fromStdString(geoMap->name));
		log += "ShapeFile Load Successfully!\n";
		ui.textBrowser->setText(log);
	}
}

//从pgsql中读取数据
void GeoJsonParese::readFromPgsql() {
	//使用向导
	DatabaseWizard wizard(this);
	wizard.exec();
	OGRDataSource *poDS = wizard.poDS;
	if (poDS == NULL)
		return;
	//获取table名
	QString tableName = wizard.tableLineEdit->text();
	GeoMap *geoMap = NULL;
	if (tableName == ""){
		geoMap = GdalUtil::OGRDataSource2Map(poDS);
	}else{
		//调用重载的转换函数，之后可以再加一个下拉框来确定打开的Table
		geoMap = GdalUtil::OGRDataSource2Map(poDS,tableName);
	}
	if (geoMap != NULL){
		//设置地图投影前判断是否为经纬度，否则不投影
		double &onex = geoMap->maxRange.topRight().rx();
		double &oney = geoMap->maxRange.topRight().ry();
		if (fabs(onex) <= 360 && fabs(oney) <= 90)
			geoMap->setMapPrj(MapPrjType::MERCATOR); //默认使用墨卡托
		dataSource->geoMaps.push_back(geoMap);
		//添加地图节点
		addTreeTopLevel(geoMap, dataSource->geoMaps.size() - 1, QString::fromStdString(geoMap->name));
		log += "Load data from PostgreSQL Successfully!\n";
		ui.textBrowser->setText(log);
	}else {
		log += "Loading data from PostgreSQL failed!\n";
		ui.textBrowser->setText(log);
	}
	
}

//shp文件转json
void GeoJsonParese::shp2GeoJson(){
	QString filePath = QFileDialog::getOpenFileName(this, "ShapeFile 2 GeoJson", "", "ShapeFile Files(*.shp)");
	QString outFilePath = "C:\\Users\\Administrator\\Desktop\\result.geojson";
	if(!filePath.isEmpty()){
		GdalUtil::shp2GeoJson(filePath, outFilePath);
		ui.textBrowser->setText("success!filePath:"+outFilePath);
		log += "shp2GeoJson Successfully!\n";
		ui.textBrowser->setText(log);
	}
}


//按压事件
void GeoJsonParese::onPressed(QPoint pos)
{
	QTreeWidgetItem* currentItem = ui.treeWidget->itemAt(pos);  //获取当前被点击的节点
	if (currentItem == Q_NULLPTR)
	{
		return;
	}
	QTreeWidgetItem* parent = currentItem->parent();
	if (parent == Q_NULLPTR)
	{
		//顶级节点绘制地图
		QVariant v = currentItem->data(0, Qt::UserRole);
		GeoMap* map = dataSource->geoMaps[v.toInt()];
		QMenu *pMenu = new QMenu(this);
		QAction *drawTask = new QAction(tr("Draw Map"), this);
		QAction *changeMapPrj = new QAction(tr("Change Map Projection"), this);
		QAction *setStyleSLD = new QAction(tr("Set Style From SLD"), this);
		QMenu *chooseIndex = new QMenu(tr("Choose Spatial Index"), this);
		QAction *gridIndex = new QAction(tr("Grid Index"), this);
		chooseIndex->addAction(gridIndex);
		connect(drawTask, SIGNAL(triggered()), this, SLOT(drawMap()));
		connect(changeMapPrj, SIGNAL(triggered()), this, SLOT(changeMapProjection()));
		connect(setStyleSLD, SIGNAL(triggered()), this, SLOT(setStyleFromSLD()));
		connect(gridIndex, SIGNAL(triggered()), this, SLOT(gridInfo()));
		pMenu->addAction(drawTask);
		pMenu->addAction(changeMapPrj);
		pMenu->addAction(setStyleSLD);
		pMenu->addMenu(chooseIndex);
		pMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
	}
	else {
		//图层节点
		int mapIndex = parent->data(ID_COLUMN, Qt::UserRole).toInt();
		int layerIndex = currentItem->data(ID_COLUMN, Qt::UserRole).toInt();
		Layer* layer = dataSource->geoMaps[mapIndex]->layers[layerIndex];
		QMenu *pMenu = new QMenu(this);
		MyAction* setStyleAction = new MyAction(mapIndex, layerIndex, tr("Set Style"), this);
		connect(setStyleAction, SIGNAL(triggered()), setStyleAction, SLOT(mtriggle()));
		connect(setStyleAction, SIGNAL(sendIndex(int, int)), this, SLOT(setStyle(int,int)));
		pMenu->addAction(setStyleAction);
		pMenu->exec(QCursor::pos());
		qDebug()<<layer->features.size()<<endl;
	}
}

//绘制地图
void GeoJsonParese::drawMap() {
	//获取被选择item的id
	QTreeWidgetItem *currentItem = ui.treeWidget->currentItem();
	QVariant vId = currentItem->data(ID_COLUMN, Qt::UserRole);
	QVariant vName = currentItem->data(NAME_COLUMN, Qt::UserRole);
	//绘制图形
	addNewWindow(dataSource->geoMaps[vId.toInt()], vName.toString());
}

//树节点改变
void GeoJsonParese::treeItemChanged(QTreeWidgetItem *item, int column) {
	//获取父节点
	QTreeWidgetItem *parent = item->parent();
	//设置是否可视
	GeoMap *map;
	Layer *layer;
	//获取与GeoMap绑定的MyOpenGLWidget
	MyOpenGLWidget *myOpenGLWidget;
	if (parent == NULL) {
		//为地图顶级节点
		int mapIndex = item->data(ID_COLUMN, Qt::UserRole).toInt();//获取map节点的index
		map = dataSource->geoMaps[mapIndex];
		myOpenGLWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(map);
	}
	else {
		//图层节点
		int mapIndex = parent->data(ID_COLUMN, Qt::UserRole).toInt();//获取map节点的index
		map = dataSource->geoMaps[mapIndex];
		myOpenGLWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(map);
		int layerIndex = item->data(ID_COLUMN, Qt::UserRole).toInt();
		layer = map->layers[layerIndex];
	}

	if (item->checkState(VISIBLE_COLUMN) == Qt::Checked)
	{
		//地图节点
		map->isVisible = true;
		if (parent != NULL) {
			layer->isVisble = true;
		}

		int cnt = item->childCount();
		if (cnt > 0)
		{
			for (int i = 0; i < cnt; i++)
			{
				item->child(i)->setCheckState(VISIBLE_COLUMN, Qt::Checked);
				treeItemChanged(item->child(i), i);
			}
		}
		else
		{
			updateParentItem(item);
		}
	}
	else if (item->checkState(VISIBLE_COLUMN) == Qt::Unchecked)
	{
		if (parent == NULL) {
			//地图节点
			map->isVisible = false;
		}
		else {
			layer->isVisble = false;
		}
		int cnt = item->childCount();
		if (cnt > 0)
		{
			for (int i = 0; i < cnt; i++)
			{
				item->child(i)->setCheckState(VISIBLE_COLUMN, Qt::Unchecked);
			}
		}
		else
		{
			updateParentItem(item);
		}
	}
	myOpenGLWidget->update();
}

void GeoJsonParese::closeTab(int tabIndex)
{
	ui.tabWidget->removeTab(tabIndex);
}
//区域全文检索
void GeoJsonParese::searchRegion()
{
	//调色盘
	//QColorDialog *m_pColorDialog = new QColorDialog();
	//m_pColorDialog->exec();
	//connect(m_pColorDialog, SIGNAL(colorSelected(QColor)), this, SLOT(slot_getColor(QColor)));

	bool isOK;
	QString text = ui.lineEdit->text();
	if (!text.isNull()) {
		QNetworkAccessManager *manager = new QNetworkAccessManager(this);
		manager->setNetworkAccessible(QNetworkAccessManager::Accessible);
		QNetworkRequest qnr(QUrl("http://www.cartovision.cn/LuceneDemo/search?name=" + text));
		QNetworkReply *reply = manager->get(qnr);
		connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
	}
}
//完成网络请求
void GeoJsonParese::replyFinished(QNetworkReply *reply)
{
	QString result = reply->readAll();
	//开始解析结果json
	QJsonParseError parseError;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(result.toUtf8(), &parseError);
	if (parseError.error != QJsonParseError::NoError)
	{
		qDebug() << parseError.error << endl;
	}
	QJsonObject jsonObject = jsonDocument.object();
	//分析内容
	if (jsonObject["flag"].toInt() == 1) {
		//查询到结果
		QString name = jsonObject["name"].toString(); //要素名字
		double area = jsonObject["area"].toString().toDouble(); //面积
		int index = jsonObject["id"].toString().toInt(); //要素在地图中的位置
		//通过工厂得到test.json唯一绑定的tabWidget
		GeoMap* testMap = dataSource->getGeoMapByName("test");
		if (testMap != NULL) {
			MyOpenGLWidget* myOpenGLWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(testMap);
			Layer* layer = testMap->layers[0];//此数据只有第一层
			Feature* feature = layer->features[index];
			feature->isSelected = true; //要素被选中
			//更新渲染
			myOpenGLWidget->update();
			//将此widget激活
			addNewWindow(testMap, QString::fromStdString(testMap->name));
			//弹出信息框
			QMessageBox msgBox;
			msgBox.setText(QString::fromLocal8Bit("查询结果"));
			msgBox.setInformativeText(QString::fromLocal8Bit("查询到的地区:")+name+"\n"+QString::fromLocal8Bit("区域面积:")+QString::number(area)+ QString::fromLocal8Bit("万平方千米"));
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setDefaultButton(QMessageBox::Ok);
			msgBox.setDetailedText(QString::fromLocal8Bit("目前没有详细介绍"));
			int ret = msgBox.exec();
			switch (ret) {
			case QMessageBox::Ok:
				feature->isSelected = false;
				myOpenGLWidget->update();
				break;
			default:
				break;
			}
		}
		//MyOpenGLWidget* testWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(testMap);
	}
	else {
		QMessageBox msgBox;
		msgBox.setText(QString::fromLocal8Bit("查询结果"));
		msgBox.setInformativeText(QString::fromLocal8Bit("未查询到该地区"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		int ret = msgBox.exec();
	}
	//释放
	reply->deleteLater();
}

void GeoJsonParese::showCurrentPos(QPointF currentPos) {
	QString xStr = QString::number(currentPos.rx(), 'f', 2);
	QString yStr = QString::number(currentPos.ry(), 'f', 2);
	ui.label_3->setText(QString::fromLocal8Bit("当前位置:")+xStr+","+yStr);
}

/*
辅助函数
*/

//添加新窗口
void GeoJsonParese::addNewWindow(GeoMap *map, QString name) {
	MyOpenGLWidget* mapView = myOpenGLWidgetFactory.getMyOpenGlWidget(map);
	//ui.tabWidget->addTab(mapView, name);
	int currentIndex = ui.tabWidget->currentIndex();
	int newIndex = ui.tabWidget->insertTab(currentIndex+1, mapView, name);
	ui.tabWidget->setCurrentIndex(newIndex);
	mapView->show();
	connect(mapView, SIGNAL(sendCurrentPos(QPointF)), this, SLOT(showCurrentPos(QPointF)));
}

//添加顶端树节点,地图树节点
QTreeWidgetItem * GeoJsonParese::addTreeTopLevel(GeoMap* geoMap, int id, QString name)
{
	QString idStr = QString::number(id);
	QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << "" << idStr << name);//显示出来的数据
	ui.treeWidget->addTopLevelItem(item);
	//绑定数据为map的索引
	item->setData(ID_COLUMN, Qt::UserRole, QVariant(dataSource->geoMaps.size() - 1));
	//绑定数据为map的名字 TODO 暂时为ID，没有名字
	item->setData(NAME_COLUMN, Qt::UserRole, QVariant(QString::fromStdString(geoMap->name)));
	item->setCheckState(VISIBLE_COLUMN, Qt::Checked);
	//添加图层节点
	for (int i = 0; i < geoMap->layers.size(); i++) {
		QString name = QString::fromStdString(geoMap->layers[i]->name);
		addTreeNode(item, geoMap, i, name);
	}
	return item;
}

//添加图层节点
QTreeWidgetItem * GeoJsonParese::addTreeNode(QTreeWidgetItem *parent, GeoMap *map, int id, QString name)
{
	QString idStr = QString::number(id);
	QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << "" << idStr << name);
	parent->addChild(item);
	item->setData(ID_COLUMN, Qt::UserRole, QVariant(id)); //图层索引
	//绑定数据为map的名字 TODO 暂时为ID，没有名字
	item->setData(NAME_COLUMN, Qt::UserRole, QVariant(name)); //暂时使用图层索引
	item->setCheckState(VISIBLE_COLUMN, Qt::Checked);
	//TODO 绑定属性
	return item;
}

//更新父节点
void GeoJsonParese::updateParentItem(QTreeWidgetItem *item)
{
	QTreeWidgetItem *parent = item->parent();
	if (parent == NULL)
	{
		return;
	}
	//选中的子节点个数
	int selectedCount = 0;
	int childCount = parent->childCount();
	for (int i = 0; i < childCount; i++)
	{
		QTreeWidgetItem *childItem = parent->child(i);
		if (childItem->checkState(VISIBLE_COLUMN) == Qt::Checked)
		{
			selectedCount++;
		}
	}
	if (selectedCount <= 0)
	{
		//未选中状态
		parent->setCheckState(VISIBLE_COLUMN, Qt::Unchecked);
	}
	else if (selectedCount > 0 && selectedCount < childCount)
	{
		//部分选中状态
		parent->setCheckState(VISIBLE_COLUMN, Qt::PartiallyChecked);
	}
	else if (selectedCount == childCount)
	{
		//选中状态
		parent->setCheckState(VISIBLE_COLUMN, Qt::Checked);
	}
	updateParentItem(parent);
}

//
//if(currentItem->parent()==Q_NULLPTR)
//{
//    delete ui.treeWidget->takeTopLevelItem(ui->tv_Source->currentIndex().row());
//}
//else
//{
//    //如果有父节点就要用父节点的takeChild删除节点
//    delete currentItem->parent()->takeChild(ui->tv_Source->currentIndex().row());
//}

void GeoJsonParese::changeMapProjection()
{
	// TODO: 在此处添加实现代码.
	//获取被选择item的id
	QTreeWidgetItem *currentItem = ui.treeWidget->currentItem();
	QVariant vId = currentItem->data(ID_COLUMN, Qt::UserRole);
	QVariant vName = currentItem->data(NAME_COLUMN, Qt::UserRole);

  	//改变地图投影
	if (dataSource->geoMaps[vId.toInt()]->mapPrj != NULL &&
		dataSource->geoMaps[vId.toInt()]->mapPrj->mapPrjChanged == false)
	{
		switch (dataSource->geoMaps[vId.toInt()]->mapPrj->getMapPrjType())
		{
		case MapPrjType::MERCATOR:
			delete dataSource->geoMaps[vId.toInt()]->mapPrj;
			dataSource->geoMaps[vId.toInt()]->mapPrj = new MapPrjLambert;
			dataSource->geoMaps[vId.toInt()]->mapPrj->mapPrjChanged = true;
			break;
		case MapPrjType::LAMBERT:
			delete dataSource->geoMaps[vId.toInt()]->mapPrj;
			dataSource->geoMaps[vId.toInt()]->mapPrj = new MapPrjMercator;
			dataSource->geoMaps[vId.toInt()]->mapPrj->mapPrjChanged = true;
			break;
		}
		MyOpenGLWidget* myOpenGlWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(dataSource->geoMaps[vId.toInt()]);
		myOpenGlWidget->update();
		log += "Map projection changed successfully!\n";
		ui.textBrowser->setText(log);
	
	}
}


void GeoJsonParese::setStyleFromSLD()
{
	//获取被选择item的id
	QTreeWidgetItem *currentItem = ui.treeWidget->currentItem();
	QVariant vId = currentItem->data(ID_COLUMN, Qt::UserRole);
	QVariant vName = currentItem->data(NAME_COLUMN, Qt::UserRole);
	GeoMap* map = dataSource->geoMaps[vId.toInt()];
	QString layerName = QString::fromStdString(map->layers.back()->name);
	//设置style
	QString sldPath = QFileDialog::getOpenFileName(NULL, "Read SLD", "", "SLD Files(*.sld)");
	if (!sldPath.isEmpty()) {
		QDomDocument* doc = SldUtil::sldRead(sldPath);
		SldUtil::parseSldDomFromName(doc, dataSource->geoMaps[vId.toInt()]->layers.back(),layerName);
	}
	
}

// 设置格网索引
void GeoJsonParese::setGridIndex(int colCount,int rowCount)
{
	//获取被选择item的id
	QTreeWidgetItem *currentItem = ui.treeWidget->currentItem();
	QVariant vId = currentItem->data(ID_COLUMN, Qt::UserRole);
	QVariant vName = currentItem->data(NAME_COLUMN, Qt::UserRole);
	GeoMap* map = dataSource->geoMaps[vId.toInt()];
	if (map->index == NULL)
	{
		map->index = new GridIndex(map->maxRange);
		((GridIndex*)map->index)->setColRow(colCount, rowCount);  //设置行数和列数
		map->index->createIndex();
		for (int i = 0; i < map->layers.size(); i++) {
			map->index->addAllObjID(map->layers.at(i));   //添加索引目标
		}
	}
	MyOpenGLWidget* myOpenGlWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(map);
	myOpenGlWidget->update();
	log += "Create Grid Spatial Index successfully!\n";
	ui.textBrowser->setText(log);
}
void GeoJsonParese::setStyle(int mapIndex, int layerIndex)
{
	Layer *layer = dataSource->geoMaps[mapIndex]->layers[layerIndex];
	StyleWidget* styleWidget = new StyleWidget(mapIndex, layerIndex, layer);
	connect(styleWidget, SIGNAL(update(int, int)), this, SLOT(refreshStyle(int, int)));
	styleWidget->show();
}

void GeoJsonParese::refreshStyle(int mapIndex, int layerIndex) {
	GeoMap * geoMap = dataSource->geoMaps[mapIndex];
	MyOpenGLWidget* myOpenGLWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(geoMap);
	myOpenGLWidget->update();
}



void GeoJsonParese::gridInfo()
{
	// TODO: 在此处添加实现代码.
	int defaultCol = 10;
	int defaultRow = 10;
	MGridInfoWidget *gridInfoWidget = new MGridInfoWidget(defaultCol, defaultRow);
	gridInfoWidget->show();
	connect(gridInfoWidget, SIGNAL(send(int, int)), this, SLOT(setGridIndex(int, int)));
}
