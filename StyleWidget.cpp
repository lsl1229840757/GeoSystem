#include "StyleWidget.h"
//定义一些数据绑定全局变量方便修改
static int ID_COLUMN = 0;
//TYPE 有Fill, STROKECOLOR, 
static int TYPE_COLUMN = 1;
/*
TYPE对照表
stroke
	strokeWidth;
	strokeColor;
	strokeOpacity;
fill
	fillColor;
	fillOpacity;
*/
static QString FILLCOLOR = "fillColor";
static QString FILLOPACIRT = "fillOpacity";
static QString STROKEWIDTH = "strokeWidth";
static QString STROKECOLOR = "strokeColor";
static QString STROKEOPACITY = "strokeOpacity";


StyleWidget::StyleWidget(int mapIndex, int layerIndex, Layer *layer, QWidget *parent)
	: QWidget(parent)
{
	this->mapIndex = mapIndex;
	this->layerIndex = layerIndex;
	ui.setupUi(this);
	ui.treeWidget->setColumnCount(2);
	ui.treeWidget->setHeaderLabels(QStringList() <<"ID" << "Name");
	ui.treeWidget->setColumnWidth(0, 550);
	ui.treeWidget->setColumnWidth(1, 50);
	/*QFormLayout* formLayout = new QFormLayout(this);
	formLayout->setSpacing(6);
	formLayout->setContentsMargins(11, 11, 11, 11);
	formLayout->setObjectName(QString::fromUtf8("formLayout"));
	QLabel* label = new QLabel("color");

	formLayout->setWidget(0, QFormLayout::LabelRole, label);

	QPushButton* lineEdit = new QPushButton("sadfsda");

	formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

	QLabel* label_2 = new QLabel("sssss");

	formLayout->setWidget(1, QFormLayout::LabelRole, label_2);
	this->setLayout(formLayout);*/
	ui.treeWidget->clear();
	//添加顶端树节点,地图树节点
	int n = layer->features.size() > 50 ? 50 : layer->features.size();
	for (int i = 0; i < n;i++) {
		Feature *feature = layer->features[i];
		mgeo::Geometry *geometry = feature->geometry;
		SymbolStyle symbolStyle = feature->symbolStyle;
		if (GeometryType::GEOPOINT == geometry->getGeometryType()) {
		

		}
		else if (GeometryType::GEOPOLYLINE == geometry->getGeometryType()) {
		
		
		
		}
		else if (GeometryType::GEOPOLYGON == geometry->getGeometryType() ||
			GeometryType::GEOMULTIPOLYGON == geometry->getGeometryType()) {
			//绘制边界颜色, 名字暂时设置为ID
			QTreeWidgetItem *featureItem = addTreeTopLevel(i, QString::number(i));
			//多边形的Fill
			addTreeNode(featureItem, FILLCOLOR, symbolStyle, feature);//fillColor
			addTreeNode(featureItem, STROKECOLOR, symbolStyle, feature);//strokeColor
			addTreeNode(featureItem, STROKEWIDTH, symbolStyle, feature);//strokeColor
		}
	}
}

StyleWidget::~StyleWidget()
{
}

//顶级节点代表feature
QTreeWidgetItem * StyleWidget::addTreeTopLevel(int id, QString name)
{
	QString idStr = QString::number(id);
	QTreeWidgetItem * item = new QTreeWidgetItem(QStringList() << idStr << name);//显示出来的数据
	ui.treeWidget->addTopLevelItem(item);
	//给feature绑定数据为索引
	item->setData(ID_COLUMN, Qt::UserRole, QVariant(id));
	return item;
}

//绑定的是哪种
QTreeWidgetItem * StyleWidget::addTreeNode(QTreeWidgetItem *parent, QString type, SymbolStyle symbol, Feature* feature)
{
	QTreeWidgetItem* item = new QTreeWidgetItem();
	if (type.compare(FILLCOLOR)==0) {
		QLabel *label = new QLabel("fill color");
		MyColorButton *colorButton = new MyColorButton(feature, type);
		QColor color = symbol.fillColor;
		colorButton->setStyleSheet("border-style:solid;background-color: rgb("+QString::number(color.red())+","+
			QString::number(color.green())+","+ QString::number(color.blue())+")");
		//添加修改颜色事件
		parent->addChild(item);
		ui.treeWidget->setItemWidget(item, 0, label);
		ui.treeWidget->setItemWidget(item, 1, colorButton);
		connect(colorButton, SIGNAL(mupdate()), this, SLOT(finishQColorDialog()));
	}
	else if(type.compare(STROKECOLOR) == 0){
		QLabel *label = new QLabel("stroke color");
		MyColorButton *colorButton = new MyColorButton(feature, type);
		QColor color = symbol.strokeColor;
		colorButton->setStyleSheet("border-style:solid;background-color: rgb(" + QString::number(color.red()) + "," +
			QString::number(color.green()) + "," + QString::number(color.blue()) + ")");
		parent->addChild(item);
		ui.treeWidget->setItemWidget(item, 0, label);
		ui.treeWidget->setItemWidget(item, 1, colorButton);
		connect(colorButton, SIGNAL(mupdate()), this, SLOT(finishQColorDialog()));
	}
	else if (type.compare(STROKEWIDTH) == 0) {
		QLabel *label = new QLabel("stroke width");
		QPushButton *colorButton = new QPushButton();
		parent->addChild(item);
		ui.treeWidget->setItemWidget(item, 0, label);
		ui.treeWidget->setItemWidget(item, 1, new QLabel(QString::number(symbol.strokeWidth)));
	}
	item->setData(TYPE_COLUMN, Qt::UserRole, QVariant(type)); //绑定是哪种样式
	return item;
}

void StyleWidget::finishQColorDialog() {
	emit update(this->mapIndex, this->layerIndex);
}