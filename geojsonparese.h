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
class GeoJsonParese : public QMainWindow
{
	Q_OBJECT

public:
	GeoJsonParese(QWidget *parent = 0);
	~GeoJsonParese();
	GeoDataSource *dataSource;
	QTreeWidgetItem *addTreeTopLevel(QString name,QString desc);
	QTreeWidgetItem * addTreeNode(QTreeWidgetItem *parent,QString name,QString desc);
	void addNewWindow(GeoMap *map, QString name);
	void updateParentItem(QTreeWidgetItem *item);
public slots:
	void parseGeoJson();
	void readShp();
	void shp2GeoJson();
	void readFromPgsql();
	void onPressed(QPoint pos);
	void drawMap();
	void treeItemChanged(QTreeWidgetItem *item, int column);
private:
	Ui::GeoJsonPareseClass ui;

};

#endif // GEOJSONPARESE_H
