#include "MyTableWidget.h"


MyTableWidget::MyTableWidget(int rows,int cols,Layer *layer, QWidget *parent)
	: QTableWidget(rows,cols,parent)
{
	this->rows = rows;
	this->cols = cols;
	this->layer = layer;
	this->resize(QSize(800, 400));
	initTable();
	addData();
	ui.setupUi(this);
}

MyTableWidget::~MyTableWidget()
{
}


void MyTableWidget::initTable()
{
	// TODO: 在此处添加实现代码.
	QStringList listHeader;
	QVariantMap::iterator iter;
	for (iter = layer->features.back()->attributes.begin(); iter != layer->features.back()->attributes.end(); iter++) {
		//迭代记录列名
		listHeader << iter.key();
	}
	this->setHorizontalHeaderLabels(listHeader);
	//设置列名自适应宽度
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}


void MyTableWidget::addData()
{
	// TODO: 在此处添加实现代码.
	for (int i = 0; i < rows; i++) {
		QVariantMap::iterator iter = layer->features.at(i)->attributes.begin();  //迭代
		for (int j = 0; j < cols; j++) {
			//QString attrType = iter.value().type();
			//QString attr = iter.value().toString();
			this->setItem(i, j, new QTableWidgetItem((const QString)iter.value().toString()));
			iter++;
		}
	}
}
