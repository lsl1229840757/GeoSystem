#include "geojsonparese.h"
//����һЩ���ݰ�ȫ�ֱ��������޸�
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
	//����Ƿ���ӿ���
	connect(ui.treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(treeItemChanged(QTreeWidgetItem*,int)));
	//��ʼ������Դ
	dataSource = new GeoDataSource;

	//��ӡ��־

}

GeoJsonParese::~GeoJsonParese()
{

}

/*
�ۺ���
*/
//����json
void GeoJsonParese::parseGeoJson(){
	QString filePath = QFileDialog::getOpenFileName(this);
	if(!filePath.isEmpty()){
		QJsonObject jsonObj = JsonUtil::JsonRead(filePath);//��һ��
		GeoMap* geoMap = JsonUtil::parseGeoJson(jsonObj);
		//QString str = QString::number(feature.geometry->coordinates[0].toDouble())+","+QString::number(feature.geometry->coordinates[1].toDouble());
		//ui.textBrowser->setText("coordinates:["+str+"]");
		dataSource->geoMaps.push_back(geoMap);
		//��ӽڵ�
		QTreeWidgetItem * mapItem = addTreeTopLevel(QString::number(dataSource->geoMaps.size()), QString::number(dataSource->geoMaps.size()));
		for (int i = 0; i < geoMap->layers.size(); i++) {
			Layer *layer = geoMap->layers[i];
			addTreeNode(mapItem, QString::number(dataSource->geoMaps.size()), QString::number(dataSource->geoMaps.size()));
		}
	}
}

//��ȡshp
void GeoJsonParese::readShp(){
	QString filePath = QFileDialog::getOpenFileName(this);
	if(!filePath.isEmpty()){
		OGRDataSource* poDS = GdalUtil::readFromGeoJson(filePath);
		//TODO �������
		GeoMap *geoMap = GdalUtil::OGRDataSource2Map(poDS);
		dataSource->geoMaps.push_back(geoMap);
		//��ӽڵ�
		QTreeWidgetItem * mapItem = addTreeTopLevel(QString::number(dataSource->geoMaps.size()),QString::number(dataSource->geoMaps.size()));
		for(int i=0;i<geoMap->layers.size();i++){
			Layer *layer = geoMap->layers[i];
			addTreeNode(mapItem, geoMap, QString::number(dataSource->geoMaps.size()),QString::number(dataSource->geoMaps.size()));
		}
		log += "ShapeFile Load Successfully!\n";
		ui.textBrowser->setText(log);

	}
}

//shp�ļ�תjson
void GeoJsonParese::shp2GeoJson(){
	QString filePath = QFileDialog::getOpenFileName(this);
	QString outFilePath = "C:\\Users\\Administrator\\Desktop\\result.geojson";
	if(!filePath.isEmpty()){
		GdalUtil::shp2GeoJson(filePath, outFilePath);
		ui.textBrowser->setText("success!filePath:"+outFilePath);
	}
}

//��pgsql�ж�ȡ����
void GeoJsonParese::readFromPgsql(){
	//ʹ����
	DatabaseWizard wizard(this);
	wizard.exec();
	OGRDataSource *poDS = wizard.poDS;
	GeoMap *geoMap = GdalUtil::OGRDataSource2Map(poDS);
	dataSource->geoMaps.push_back(geoMap);
	//��ӵ�ͼ�ڵ�
	QTreeWidgetItem * mapItem = addTreeTopLevel(QString::number(dataSource->geoMaps.size()),QString::number(dataSource->geoMaps.size()));
	//���ͼ��ڵ�
	for(int i=0;i<geoMap->layers.size();i++){
		Layer *layer = geoMap->layers[i];
		addTreeNode(mapItem, geoMap, QString::number(dataSource->geoMaps.size()),QString::number(dataSource->geoMaps.size()));
	}
}

//��ѹ�¼�
void GeoJsonParese::onPressed(QPoint pos)
{
	QTreeWidgetItem* currentItem = ui.treeWidget->itemAt(pos);  //��ȡ��ǰ������Ľڵ�
	if (currentItem == Q_NULLPTR)
	{
		return;
	}
	if (currentItem->parent() == Q_NULLPTR)
	{
		//�����ڵ���Ƶ�ͼ
		QVariant v = currentItem->data(0, Qt::UserRole);
		GeoMap* map = dataSource->geoMaps[v.toInt()];
		QMenu *pMenu = new QMenu(this);
		QAction *drawTask = new QAction(tr("Draw Map"), this);
		connect(drawTask, SIGNAL(triggered()), this, SLOT(drawMap()));
		pMenu->addAction(drawTask);
		pMenu->exec(QCursor::pos());//�����Ҽ��˵����˵�λ��Ϊ���λ��
	}
}

//���Ƶ�ͼ
void GeoJsonParese::drawMap() {
	//��ȡ��ѡ��item��id
	QTreeWidgetItem *currentItem = ui.treeWidget->currentItem();
	QVariant vId = currentItem->data(ID_COLUMN, Qt::UserRole);
	QVariant vName = currentItem->data(NAME_COLUMN, Qt::UserRole);
	//����ͼ��
	addNewWindow(dataSource->geoMaps[vId.toInt()], vName.toString());
}

//���ڵ�ı�
void GeoJsonParese::treeItemChanged(QTreeWidgetItem *item, int column) {
	//��ȡ���ڵ�
	QTreeWidgetItem *parent = item->parent();
	//�����Ƿ����
	GeoMap *map;
	Layer *layer;
	if (parent == NULL) {
		//Ϊ��ͼ�����ڵ�
		int mapIndex = item->data(ID_COLUMN, Qt::UserRole).toInt();//��ȡmap�ڵ��index
		map = dataSource->geoMaps[mapIndex];
	}
	else {
		//ͼ��ڵ�
		int mapIndex = parent->data(ID_COLUMN, Qt::UserRole).toInt();//��ȡmap�ڵ��index
		map = dataSource->geoMaps[mapIndex];
		int layerIndex = item->data(ID_COLUMN, Qt::UserRole).toInt();
		layer = map->layers[layerIndex];
	}

	if (item->checkState(VISIBLE_COLUMN) == Qt::Checked)
	{
		if (parent == NULL) {
			//��ͼ�ڵ�
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
			//��ͼ�ڵ�
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
��������
*/

//����´���
void GeoJsonParese::addNewWindow(GeoMap *map, QString name) {
	MyOpenGLWidget* mapView = new MyOpenGLWidget(map);
	//ui.tabWidget->addTab(mapView, name);
	int currentIndex = ui.tabWidget->currentIndex();
	ui.tabWidget->insertTab(currentIndex+1, mapView, name);
	ui.tabWidget->setCurrentIndex(currentIndex + 1);
	mapView->show();
}

//��Ӷ������ڵ�,��ͼ���ڵ�
QTreeWidgetItem * GeoJsonParese::addTreeTopLevel(QString id, QString name)
{
	QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << "" << id << name);//��ʾ����������
	ui.treeWidget->addTopLevelItem(item);
	//������Ϊmap������
	item->setData(ID_COLUMN, Qt::UserRole, QVariant(dataSource->geoMaps.size() - 1));
	//������Ϊmap������ TODO ��ʱΪID��û������
	item->setData(NAME_COLUMN, Qt::UserRole, QVariant(dataSource->geoMaps.size()));
	item->setCheckState(VISIBLE_COLUMN, Qt::Checked);
	return item;
}

//���ͼ��ڵ�
QTreeWidgetItem * GeoJsonParese::addTreeNode(QTreeWidgetItem *parent, GeoMap *map, QString id, QString name)
{
	QTreeWidgetItem* item = new QTreeWidgetItem(QStringList() << "" << id << name);
	parent->addChild(item);
	item->setData(ID_COLUMN, Qt::UserRole, QVariant(map->layers.size() - 1)); //ͼ������
	//������Ϊmap������ TODO ��ʱΪID��û������
	item->setData(NAME_COLUMN, Qt::UserRole, QVariant(map->layers.size() - 1)); //��ʱʹ��ͼ������
	item->setCheckState(VISIBLE_COLUMN, Qt::Checked);
	//TODO ������
	return item;
}

//���¸��ڵ�
void GeoJsonParese::updateParentItem(QTreeWidgetItem *item)
{
	QTreeWidgetItem *parent = item->parent();
	if (parent == NULL)
	{
		return;
	}
	//ѡ�е��ӽڵ����
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
		//δѡ��״̬
		parent->setCheckState(VISIBLE_COLUMN, Qt::Unchecked);
	}
	else if (selectedCount > 0 && selectedCount < childCount)
	{
		//����ѡ��״̬
		parent->setCheckState(VISIBLE_COLUMN, Qt::PartiallyChecked);
	}
	else if (selectedCount == childCount)
	{
		//ѡ��״̬
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
//    //����и��ڵ��Ҫ�ø��ڵ��takeChildɾ���ڵ�
//    delete currentItem->parent()->takeChild(ui->tv_Source->currentIndex().row());
//}