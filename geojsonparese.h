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
	//¹¤³§Àà
	MyOpenGlWidgetFactory myOpenGLWidgetFactory;
public slots:
	void parseGeoJson();
	void readShp();
	void shp2GeoJson();
	void readFromPgsql();
	void onPressed(QPoint pos);
	void drawMap();
	void treeItemChanged(QTreeWidgetItem *item, int column);
	void closeTab(int tabIndex);
private:
	Ui::GeoJsonPareseClass ui;

};

#endif // GEOJSONPARESE_H
