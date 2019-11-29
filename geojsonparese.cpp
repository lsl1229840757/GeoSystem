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
	//添加是否可视控制
	connect(ui.treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
	//初始化数据源
	dataSource = new GeoDataSource;

	//打印日志

}

GeoJsonParese::~GeoJsonParese()
{

}

/*
槽函数
*/
//解析json
void GeoJsonParese::parseGeoJson(){
	QString filePath = QFileDialog::getOpenFileName(this);
	if(!filePath.isEmpty()){
		QJsonObject jsonObj = JsonUtil::JsonRead(filePath);//第一级
		GeoMap* geoMap = JsonUtil::parseGeoJson(jsonObj);
		//QString str = QString::number(feature.geometry->coordinates[0].toDouble())+","+QString::number(feature.geometry->coordinates[1].toDouble());
		//ui.textBrowser->setText("coordinates:["+str+"]");
		dataSource->geoMaps.push_back(geoMap);
		//添加节点
		QTreeWidgetItem * mapItem = addTreeTopLevel(QString::number(dataSource->geoMaps.size()), QString::number(dataSource->geoMaps.size()));
		for (int i = 0; i < geoMap->layers.size(); i++) {
			Layer *layer = geoMap->layers[i];
			addTreeNode(mapItem, QString::number(dataSource->geoMaps.size()), QString::number(dataSource->geoMaps.size()));
		}
	}
}

//读取shp
void GeoJsonParese::readShp(){
	QString filePath = QFileDialog::getOpenFileName(this);
	if(!filePath.isEmpty()){
		OGRDataSource* poDS = GdalUtil::readFromGeoJson(filePath);
		//TODO 报错机制
		GeoMap *geoMap = GdalUtil::OGRDataSource2Map(poDS);
		dataSource->geoMaps.push_back(geoMap);
		//添加节点
		QTreeWidgetItem * mapItem = addTreeTopLevel(QString::number(dataSource->geoMaps.size()),QString::number(dataSource->geoMaps.size()));
		for(int i=0;i<geoMap->layers.size();i++){
			Layer *layer = geoMap->layers[i];
			addTreeNode(mapItem, geoMap, QString::number(dataSource->geoMaps.size()),QString::number(dataSource->geoMaps.size()));
		}
		log += "ShapeFile Load Successfully!\n";
		ui.textBrowser->setText(log);

	}
}

//shp文件转json
void GeoJsonParese::shp2GeoJson(){
	QString filePath = QFileDialog::getOpenFileName(this);
	QString outFilePath = "C:\\Users\\Administrator\\Desktop\\result.geojson";
	if(!filePath.isEmpty()){
		GdalUtil::shp2GeoJson(filePath, outFilePath);
		ui.textBrowser->setText("success!filePath:"+outFilePath);
	}
}

//从pgsql中读取数据
void GeoJsonParese::readFromPgsql(){
	//使用向导
	DatabaseWizard wizard(this);
	wizard.exec();
	OGRDataSource *poDS = wizard.poDS;
	GeoMap *geoMap = GdalUtil::OGRDataSource2Map(poDS);
	dataSource->geoMaps.push_back(geoMap);
	//添加地图节点
	QTreeWidgetItem * mapItem = addTreeTopLevel(QString::number(dataSource->geoMaps.size()),QString::number(dataSource->geoMaps.size()));
	//添加图层节点
	for(int i=0;i<geoMap->layers.size();i++){
		Layer *layer = geoMap->layers[i];
		addTreeNode(mapItem, geoMap, QString::number(dataSource->geoMaps.size()),QString::number(dataSource->geoMaps.size()));
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
	if (currentItem->parent() == Q_NULLPTR)
	{
		//顶级节点绘制地图
		QVariant v = currentItem->data(0, Qt::UserRole);
		GeoMap* map = dataSource->geoMaps[v.toInt()];
		QMenu *pMenu = new QMenu(this);
		QAction *drawTask = new QAction(tr("Draw Map"), this);
		connect(drawTask, SIGNAL(triggered()), this, SLOT(drawMap()));
		pMenu->addAction(drawTask);
		pMenu->exec(QCursor::pos());//弹出右键菜单，菜单位置为光标位置
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
	if (parent == NULL) {
		//为地图顶级节点
		int mapIndex = item->data(ID_COLUMN, Qt::UserRole).toInt();//获取map节点的index
		map = dataSource->geoMaps[mapIndex];
	}
	else {
		//图层节点
		int mapIndex = parent->data(ID_COLUMN, Qt::UserRole).toInt();//获取map节点的index
		map = dataSource->geoMaps[mapIndex];
		int layerIndex = item->data(ID_COLUMN, Qt::UserRole).toInt();
		layer = map->layers[layerIndex];
	}

	if (item->checkState(VISIBLE_COLUMN) == Qt::Checked)
	{
		if (parent == NULL) {
			//地图节点
			map->isVisible = true;
		}
		else {
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
}

void GeoJsonParese::closeTab(int tabIndex)
{
	ui.tabWidget->removeTab(tabIndex);
}


/*
辅助函数
*/

//添加新窗口
void GeoJsonParese::addNewWindow(GeoMap *map, QString name) {
	MyOpenGLWidget* mapView = new MyOpenGLWidget(map);
	//ui.tabWidget->addTab(mapView, name);
	int currentIndex = ui.tabWidget->currentIndex();
	ui.tabWidget->insertTab(currentIndex+1, mapView, name);
	ui.tabWidget->setCurrentIndex(currentIndex + 1);
	mapView->show();
}

//添加顶端树节点,地图树节点
QTreeWidgetItem * GeoJsonParese::addTreeTopLevel(QString id, QString name)
{
	QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << "" << id << name);//显示出来的数据
	ui.treeWidget->addTopLevelItem(item);
	//绑定数据为map的索引
	item->setData(ID_COLUMN, Qt::UserRole, QVariant(dataSource->geoMaps.size() - 1));
	//绑定数据为map的名字 TODO 暂时为ID，没有名字
	item->setData(NAME_COLUMN, Qt::UserRole, QVariant(dataSource->geoMaps.size()));
	item->setCheckState(VISIBLE_COLUMN, Qt::Checked);
	return item;
}

//添加图层节点
QTreeWidgetItem * GeoJsonParese::addTreeNode(QTreeWidgetItem *parent, GeoMap *map, QString id, QString name)
{
	QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << "" << id << name);
	parent->addChild(item);
	item->setData(ID_COLUMN, Qt::UserRole, QVariant(map->layers.size() - 1)); //图层索引
	//绑定数据为map的名字 TODO 暂时为ID，没有名字
	item->setData(NAME_COLUMN, Qt::UserRole, QVariant(map->layers.size() - 1)); //暂时使用图层索引
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