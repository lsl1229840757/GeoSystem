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

	QPushButton *pSearchButton2 = new QPushButton();

	pSearchButton2->setCursor(Qt::PointingHandCursor);
	pSearchButton2->setFixedSize(22, 22);
	pSearchButton2->setToolTip(QStringLiteral("语音搜索"));
	pSearchButton2->setStyleSheet("QPushButton{border-image:url(:/images/icon_voiceSearch); background:transparent;} \
                                     QPushButton:hover{border-image:url(:/images/icon_voiceSearch)} \
                                     QPushButton:pressed{border-image:url(:/images/icon_voiceSearch)}");
	//防止文本框输入内容位于按钮之下
	QMargins margins = ui.lineEdit->textMargins();
	ui.lineEdit->setTextMargins(margins.left(), margins.top(), pSearchButton->width(), margins.bottom());
	ui.lineEdit->setPlaceholderText(QStringLiteral("请输入搜索内容"));

	QHBoxLayout *pSearchLayout = new QHBoxLayout();
	pSearchLayout->addStretch();
	pSearchLayout->addWidget(pSearchButton);
	pSearchLayout->addWidget(pSearchButton2);
	pSearchLayout->setSpacing(1);
	pSearchLayout->setContentsMargins(0, 0, 0, 0);
	ui.lineEdit->setLayout(pSearchLayout);
	//添加搜索事件
	connect(pSearchButton, SIGNAL(clicked(bool)), this, SLOT(searchRegion()));
	connect(pSearchButton2, SIGNAL(clicked(bool)), this, SLOT(voiceSearchRegion()));

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
		QMenu *mapPrjMenu = new QMenu(tr("Map Prejection"), this);
		//QAction *changeMapPrj = new QAction(tr("Change Map Projection"), this);
		QAction *addMapPfjMerc =new QAction(tr("Mercator Projection"), this);  //默认添加墨卡托投影
		QAction *setStyleSLD = new QAction(tr("Set Style From SLD"), this);
		QMenu *chooseIndex = new QMenu(tr("Choose Spatial Index"), this);
		QAction *gridIndex = new QAction(tr("Grid Index"), this);
		QAction *quadIndex = new QAction(tr("Quadtree Index"), this);
		QMenu *chooseTool = new QMenu(tr("Choose Tool"), this);
		QAction *kernelDens = new QAction(tr("Kernel Density"), this);
		QAction *accessAnaly = new QAction(tr("Access Analysis"), this);
		QAction *addLayerShp = new QAction(tr("Add Layer From Shp"), this);
		QAction *addLayerJson = new QAction(tr("Add Layer From GeoJson"), this);
		QAction *addLayerPostgis = new QAction(tr("Add Layer From PostgreSQL"), this);
		QList<QAction*> *addLayerList = new QList<QAction*>;
		addLayerList->append(addLayerJson);
		addLayerList->append(addLayerShp);
		addLayerList->append(addLayerPostgis);
		QMenu *addLayer = new QMenu(tr("Add Layer"), this);
		addLayer->addActions(*addLayerList);
		chooseIndex->addAction(gridIndex);
		chooseIndex->addAction(quadIndex);
		chooseTool->addAction(kernelDens);
		chooseTool->addAction(accessAnaly);
		mapPrjMenu->addAction(addMapPfjMerc);
		//mapPrjMenu->addAction(changeMapPrj);
		connect(drawTask, SIGNAL(triggered()), this, SLOT(drawMap()));
		//connect(changeMapPrj, SIGNAL(triggered()), this, SLOT(changeMapProjection()));
		connect(addMapPfjMerc, SIGNAL(triggered()), this, SLOT(setMapProjection()));
		connect(setStyleSLD, SIGNAL(triggered()), this, SLOT(setStyleFromSLD()));
		connect(gridIndex, SIGNAL(triggered()), this, SLOT(gridInfo()));
		connect(quadIndex, SIGNAL(triggered()), this, SLOT(setQuadTreeIndex()));
		connect(kernelDens, SIGNAL(triggered()), this, SLOT(openKernelTool()));
		connect(addLayerShp, SIGNAL(triggered()), this, SLOT(readShpToLayer()));
		connect(addLayerJson, SIGNAL(triggered()), this, SLOT(readGeoJsonToLayer()));
		connect(addLayerPostgis, SIGNAL(triggered()), this, SLOT(readPostgisToLayer()));
		connect(accessAnaly, SIGNAL(triggered()), this, SLOT(openAccessAnalyTool()));
		
		pMenu->addAction(drawTask);
		pMenu->addMenu(addLayer);
		pMenu->addMenu(mapPrjMenu);
		//pMenu->addAction(changeMapPrj);
		pMenu->addAction(setStyleSLD);
		pMenu->addMenu(chooseIndex);
		pMenu->addMenu(chooseTool);
		
		pMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
	}
	else {
		//图层节点
		int mapIndex = parent->data(ID_COLUMN, Qt::UserRole).toInt();
		int layerIndex = currentItem->data(ID_COLUMN, Qt::UserRole).toInt();
		Layer* layer = dataSource->geoMaps[mapIndex]->layers[layerIndex];
		QMenu *pMenu = new QMenu(this);
		MyAction* setStyleAction = new MyAction(mapIndex, layerIndex, tr("Set Style"), this);
		MyAction *showAttrTableAction = new MyAction(mapIndex, layerIndex, tr("Attribute Table"), this);
		connect(setStyleAction, SIGNAL(triggered()), setStyleAction, SLOT(mtriggle()));
		connect(setStyleAction, SIGNAL(sendIndex(int, int)), this, SLOT(setStyle(int,int)));
		connect(showAttrTableAction, SIGNAL(triggered()), showAttrTableAction, SLOT(mtriggle()));
		connect(showAttrTableAction, SIGNAL(sendIndex(int, int)), this, SLOT(showAttrTable(int,int)));
		pMenu->addAction(setStyleAction);
		pMenu->addAction(showAttrTableAction);
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

//语音区域搜索
void GeoJsonParese::voiceSearchRegion()
{
	//打开语音搜索窗口
	AudioInputWidget* audioInputWidget = new AudioInputWidget;
	audioInputWidget->show();
	connect(audioInputWidget, SIGNAL(sendResult(QString)), this, SLOT(refreshSelectFeature(QString)));
}

//完成网络请求
void GeoJsonParese::replyFinished(QNetworkReply *reply)
{
	QString result = reply->readAll();
	refreshSelectFeature(result);
	//释放
	reply->deleteLater();
}

void GeoJsonParese::showCurrentPos(QPointF currentPos) {
	QString xStr = QString::number(currentPos.rx(), 'f', 2);
	QString yStr = QString::number(currentPos.ry(), 'f', 2);
	ui.label_3->setText(QString::fromLocal8Bit("当前位置:")+xStr+","+yStr);
}


//网络请求完后刷新选中要素
void GeoJsonParese::refreshSelectFeature(QString replyStr)
{
	GeoMap* testMap = dataSource->getGeoMapByName("test");
	if (testMap == NULL)
		return;
	//准备要素数据
	MyOpenGLWidget* myOpenGLWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(testMap);
	Layer* layer = testMap->layers[0];//此数据只有第一层
	//开始解析结果json
	QJsonParseError parseError;
	QJsonDocument jsonDocument = QJsonDocument::fromJson(replyStr.toUtf8(), &parseError);
	if (parseError.error != QJsonParseError::NoError)
	{
		qDebug() << parseError.error << endl;
	}
	QJsonObject jsonObject = jsonDocument.object();
	//分析内容
	if (jsonObject["flag"].toInt() == 1) {
		//记录改变的id,后面方便还原
		vector<int> indexList;
		//窗口显示结果
		QString info;
		//查询到了结果
		for (int i = 0; i < jsonObject["resultList"].toArray().size(); i++) {//开始循环结果集
			QJsonObject resultObj = jsonObject["resultList"].toArray()[i].toObject();
			QString name = resultObj["name"].toString();//开始取名字
			int index = resultObj["id"].toString().toInt();
			double area = resultObj["area"].toString().toDouble();
			indexList.push_back(index);
			//要素选中
			Feature* feature = layer->features[index];
			feature->isSelected = true;
			myOpenGLWidget->selectedFeature.push_back(feature);
			//积累信息
			info += QString::fromLocal8Bit("查询到的地区:") + name + QString::fromLocal8Bit(",面积为:") + QString::number(area) + QString::fromLocal8Bit("万平方千米\n");
		}
		//弹出消息框
		QMessageBox msgBox;
		msgBox.setText(QString::fromLocal8Bit("查询结果"));
		msgBox.setInformativeText(info);
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		msgBox.setDetailedText(QString::fromLocal8Bit("目前没有详细介绍"));
		//更新渲染
		myOpenGLWidget->update();
		//将此widget激活
		addNewWindow(testMap, QString::fromStdString(testMap->name));
		msgBox.exec();
	}
	else {
		QMessageBox msgBox;
		msgBox.setText(QString::fromLocal8Bit("查询结果"));
		msgBox.setInformativeText(QString::fromLocal8Bit("未查询到该地区"));
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.setDefaultButton(QMessageBox::Ok);
		int ret = msgBox.exec();
	}
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

void GeoJsonParese::initProject(GeoMap* map)
{
	for (int i = 0; i < map->layers.size(); i++) {
		Layer* layer = map->layers[i];
		for (int j = 0; j<layer->features.size(); j++) {
			//修改所有的投影flag
			mgeo::Geometry* geometry = layer->features[j]->geometry;
			geometry->isProjeted = false;
			if (GeometryType::GEOPOINT == geometry->getGeometryType()) {
			
			}
			else if (GeometryType::GEOPOLYLINE == geometry->getGeometryType()) {
				GeoPolyline * polyline = (GeoPolyline *)geometry;
				for (int k = 0; k < polyline->points.size(); k++) {
					polyline->points[k]->isProjeted = false;
				}
			}
			else if (GeometryType::GEOPOLYGON == geometry->getGeometryType()) {
				GeoPolygon * polygon = (GeoPolygon *)geometry;
				polygon->triangles.clear();//删除三角剖分
				for (int k = 0; k < polygon->points.size(); k++) {
					polygon->points[k]->isProjeted = false;
				}
			}
			else if (GeometryType::GEOMULTIPOLYGON == geometry->getGeometryType()) {
				GeoMultiPolygon * multiPolygon = (GeoMultiPolygon *)geometry;
				for (int m = 0; m < multiPolygon->polygons.size(); m++) {
					GeoPolygon *polygon = multiPolygon->polygons[m];
					polygon->isProjeted = false;
					polygon->triangles.clear();//删除三角剖分
					for (int k = 0; k < polygon->points.size(); k++) {
						polygon->points[k]->isProjeted = false;
					}
				}
			}
		}
	}
}

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
		//重置map投影
		initProject(dataSource->geoMaps[vId.toInt()]);
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
	if (map->index != NULL){
		//有索引时重建索引
		delete map->index;
		map->index = NULL;
	}
	map->index = new GridIndex(map->maxRange);
	((GridIndex*)map->index)->setColRow(colCount, rowCount);  //设置行数和列数
	map->index->createIndex();
	for (int i = 0; i < map->layers.size(); i++) {
		map->index->addAllObjID(map->layers.at(i));   //添加索引目标
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


void GeoJsonParese::showAttrTable(int mapIndex, int layerIndex)
{
	// TODO: 在此处添加实现代码.
	Layer *layer = dataSource->geoMaps[mapIndex]->layers[layerIndex];
	int rows = layer->features.size();
	int cols = layer->features.back()->attributes.size();
	if (0 == cols){
		QMessageBox::warning(NULL, "Not found attributes", "Not found attibutes");
		return;
	}
	MyTableWidget *attrTable = new MyTableWidget(rows,cols,layer);
	attrTable->show();
}



void GeoJsonParese::openKernelTool()
{
	// TODO: 在此处添加实现代码.
	//获取被选择item的id
	QTreeWidgetItem *currentItem = ui.treeWidget->currentItem();
	QVariant vId = currentItem->data(ID_COLUMN, Qt::UserRole);
	QVariant vName = currentItem->data(NAME_COLUMN, Qt::UserRole);
	GeoMap* map = dataSource->geoMaps[vId.toInt()];
	KernelToolWidget *kernelToolWidget = new KernelToolWidget(map);
	kernelToolWidget->show();
}


void GeoJsonParese::setQuadTreeIndex()
{
	// TODO: 在此处添加实现代码.
	//获取被选择item的id
	QTreeWidgetItem *currentItem = ui.treeWidget->currentItem();
	QVariant vId = currentItem->data(ID_COLUMN, Qt::UserRole);
	QVariant vName = currentItem->data(NAME_COLUMN, Qt::UserRole);
	GeoMap* map = dataSource->geoMaps[vId.toInt()];
	if (map->index != NULL) {
		//有索引时重建索引
		delete map->index;
		map->index = NULL;
	}
	map->index = new QuadTreeIndex(0,map->maxRange);
	map->index->createIndex();
	for (int i = 0; i < map->layers.size(); i++) {
		map->index->addAllObjID(map->layers.at(i));   //添加索引目标
	}
	MyOpenGLWidget* myOpenGlWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(map);
	myOpenGlWidget->update();
	log += "Create Quadtree Spatial Index successfully!\n";
	ui.textBrowser->setText(log);
}


void GeoJsonParese::addLayerToCurrentMap(GeoMap* loaddataMap,OpenFileType type)
{
	// TODO: 在此处添加实现代码.
	//获取被选择item的id
	QTreeWidgetItem *currentItem = ui.treeWidget->currentItem();
	QVariant vId = currentItem->data(ID_COLUMN, Qt::UserRole);
	QVariant vName = currentItem->data(NAME_COLUMN, Qt::UserRole);
	GeoMap* currentMap = dataSource->geoMaps[vId.toInt()];
	int nowIds = currentItem->childCount();  //现有的ID号数量
	for (int i = 0; i < loaddataMap->layers.size(); i++)
	{
		currentMap->addLayer(loaddataMap->layers.at(i));
		QString name = QString::fromStdString(loaddataMap->layers.at(i)->name);
		addTreeNode(currentItem, loaddataMap, nowIds++, name);  //id添加后+1
	}
	switch (type)
	{
	case OpenFileType::SHP:
		log += "Load shp to layer successfully!\n";
		ui.textBrowser->setText(log);
		break;
	case OpenFileType::GEOJSON:
		log += "Load geojson to layer successfully!\n";
		ui.textBrowser->setText(log);
		break;
	case OpenFileType::POSTGRESQL:
		log += "Load postgreSQL to layer successfully!\n";
		ui.textBrowser->setText(log);
		break;
	}
	MyOpenGLWidget *curGlWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(currentMap);
	curGlWidget->update();
}


void GeoJsonParese::readShpToLayer()
{
	// TODO: 在此处添加实现代码.
	//读取文件
	QString filePath = QFileDialog::getOpenFileName(this, "ShapeFile Parse", "", "ShapeFile Files(*.shp)");
	if (!filePath.isEmpty()) {
		OGRDataSource* poDS = GdalUtil::readFromGeoJson(filePath);
		//TODO 报错机制
		GeoMap *geoMap = GdalUtil::OGRDataSource2Map(poDS);
		//添加图层·
		addLayerToCurrentMap(geoMap, OpenFileType::SHP);
	}
}


void GeoJsonParese::readGeoJsonToLayer()
{
	// TODO: 在此处添加实现代码.
	QString filePath = QFileDialog::getOpenFileName(this, "GeoJson Parse", "", "GeoJson Files(*.geojson)");
	if (!filePath.isEmpty()) {
		GeoMap* geoMap = JsonUtil::parseGeoJson(filePath);
		//添加图层
		addLayerToCurrentMap(geoMap, OpenFileType::GEOJSON);
	}
}


void GeoJsonParese::readPostgisToLayer()
{
	// TODO: 在此处添加实现代码.
	//使用向导
	DatabaseWizard wizard(this);
	wizard.exec();
	OGRDataSource *poDS = wizard.poDS;
	if (poDS == NULL)
		return;
	//获取table名
	QString tableName = wizard.tableLineEdit->text();
	GeoMap *geoMap = NULL;
	if (tableName == "") {
		geoMap = GdalUtil::OGRDataSource2Map(poDS);
	}
	else {
		//调用重载的转换函数，之后可以再加一个下拉框来确定打开的Table
		geoMap = GdalUtil::OGRDataSource2Map(poDS, tableName);
	}
	addLayerToCurrentMap(geoMap, OpenFileType::GEOJSON);
}


void GeoJsonParese::openAccessAnalyTool()
{
	// TODO: 在此处添加实现代码.
	//获取被选择item的id
	QTreeWidgetItem *currentItem = ui.treeWidget->currentItem();
	QVariant vId = currentItem->data(ID_COLUMN, Qt::UserRole);
	QVariant vName = currentItem->data(NAME_COLUMN, Qt::UserRole);
	GeoMap* map = dataSource->geoMaps[vId.toInt()];
	AccessAnalyToolWidget *accessToolWidget = new AccessAnalyToolWidget(map);
	connect(accessToolWidget, SIGNAL(finishAnalyse(vector<double>, GeoMap*, Layer*)), this, SLOT(finishAccessAnalyse(vector<double>, GeoMap*, Layer*)));
	accessToolWidget->show();
}
void GeoJsonParese::setMapProjection()
{
	// TODO: 在此处添加实现代码.
	//获取被选择item的id
	QTreeWidgetItem *currentItem = ui.treeWidget->currentItem();
	QVariant vId = currentItem->data(ID_COLUMN, Qt::UserRole);
	QVariant vName = currentItem->data(NAME_COLUMN, Qt::UserRole);
	GeoMap* currentMap = dataSource->geoMaps[vId.toInt()];
	if (currentMap != NULL) {
		//设置地图投影前判断是否为经纬度，否则不投影
		double &onex = currentMap->maxRange.topRight().rx();
		double &oney = currentMap->maxRange.topRight().ry();
		if (fabs(onex) <= 360 && fabs(oney) <= 90)
		{
			currentMap->setMapPrj(MapPrjType::MERCATOR); //默认使用墨卡托
			log += "Set projection successfully!\n";
			ui.textBrowser->setText(log);
		}
		else
		{
			log += "Do not need projection!\n";
			ui.textBrowser->setText(log);
		}
	}
	else {
		log += "Set projection failed!\n";
		ui.textBrowser->setText(log);
	}
	MyOpenGLWidget* myOpenGlWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(currentMap);
	myOpenGlWidget->resetMaprange();  //设置地图投影后重置range
	myOpenGlWidget->update();
}
void GeoJsonParese::finishAccessAnalyse(vector<double> result, GeoMap *map, Layer *layer)
{
	MyOpenGLWidget* myOpenGlWidget = myOpenGLWidgetFactory.getMyOpenGlWidget(map);
	myOpenGlWidget->setStyleByProperties(layer, "accessbility");
	//排序
	sort(result.begin(), result.end(), [](double x, double y) -> bool {  return x > y; });//降序排列
	//绘制图表,展示前十名
	int num = 10;
	QBarSet *set0 = new QBarSet("Accessbility");
	QBarSeries *series = new QBarSeries();
	QStringList categories;
	for (int i = 0; i < num; i++) {
		categories << QString::number(i);
		*set0 << result[i];
	}
	series->append(set0);
	QChart *chart = new QChart();
	chart->addSeries(series);
	QBarCategoryAxis *axis = new QBarCategoryAxis();
	axis->append(categories);
	chart->createDefaultAxes();
	chart->setAxisX(axis, series);

	chart->legend()->setVisible(true);
	chart->legend()->setAlignment(Qt::AlignBottom);

	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	chartView->setFixedSize(QSize(600, 600));
	chartView->show();
	chartView->setWindowTitle(QString::fromLocal8Bit("可达性前10名展示"));
}
