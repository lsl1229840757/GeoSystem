#ifndef GEOJSONPARESE_H
#define GEOJSONPARESE_H
#include <QtWidgets/QMainWindow>
#include "ui_geojsonparese.h"
#include <qstring.h>
#include <qfiledialog.h>
#include "Feature.h"
#include "JsonUtil.h"
#include "GdalUtil.h"
#include <QWizard>
#include "DatabaseWizard.h"
#include <qinputdialog.h>
#include <QMessageBox>
#include <qpainter.h>
#include "myopenglwidget.h"
#include "GeoDataSource.h"
#include "SldUtil.h"
#include "MyOpenGlWidgetFactory.h"
#include "QNetworkAccessManager"
#include "QNetworkReply"
#include "QTextCodec"
#include "MapPrjUtil.h"
#include "QPushButton"
#include "QColorDialog"
#include "MyAction.h"
#include "StyleWidget.h"
class GeoJsonParese : public QMainWindow
{
	Q_OBJECT
public:
	GeoJsonParese(QWidget *parent = 0);
	~GeoJsonParese();
	GeoDataSource *dataSource;
	QTreeWidgetItem * GeoJsonParese::addTreeTopLevel(GeoMap* geoMap, int id, QString name);
	QTreeWidgetItem * addTreeNode(QTreeWidgetItem *parent, GeoMap *map, int id, QString name);
	void addNewWindow(GeoMap *map, QString name);
	void updateParentItem(QTreeWidgetItem *item);
	QString log;
	MyOpenGlWidgetFactory myOpenGLWidgetFactory;
public slots:
	void showCurrentPos(QPointF currentPos);
	void parseGeoJson();
	void readShp();
	void shp2GeoJson();
	void readFromPgsql();
	void onPressed(QPoint pos);
	void drawMap();
	void treeItemChanged(QTreeWidgetItem *item, int column);
	void closeTab(int tabIndex);
	void searchRegion();
	void replyFinished(QNetworkReply*);
	void changeMapProjection();
	void setStyleFromSLD();
	void setStyle(int mapIndex, int layerIndex); //通过窗口设置样式
	void refreshStyle(int mapIndex, int layerIndex);
private:
	Ui::GeoJsonPareseClass ui;

public:
	
};

#endif // GEOJSONPARESE_H
