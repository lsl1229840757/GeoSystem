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
	// TODO: �ڴ˴����ʵ�ִ���.
	QStringList listHeader;
	QVariantMap::iterator iter;
	for (iter = layer->features.back()->attributes.begin(); iter != layer->features.back()->attributes.end(); iter++) {
		//������¼����
		listHeader << iter.key();
	}
	this->setHorizontalHeaderLabels(listHeader);
	//������������Ӧ���
	this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	this->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}


void MyTableWidget::addData()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	for (int i = 0; i < rows; i++) {
		QVariantMap::iterator iter = layer->features.at(i)->attributes.begin();  //����
		for (int j = 0; j < cols; j++) {
			//QString attrType = iter.value().type();
			//QString attr = iter.value().toString();
			this->setItem(i, j, new QTableWidgetItem((const QString)iter.value().toString()));
			iter++;
		}
	}
}
