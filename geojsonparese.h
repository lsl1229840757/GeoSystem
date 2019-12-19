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
#include "GridIndex.h"
#include "QPushButton"
#include "QColorDialog"
#include "MyAction.h"
#include "StyleWidget.h"
#include "MGridInfoWidget.h"
#include "qmessagebox.h"
#include "MyTableWidget.h"
#include "AudioInputWidget.h"
#include "KernelToolWidget.h"
#include "QuadTreeIndex.h"
#include "AccessAnalyToolWidget.h"
typedef enum
{
	SHP,GEOJSON,POSTGRESQL
}OpenFileType;

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
	void initProject(GeoMap* map); //重置投影
	void addLayerToCurrentMap(GeoMap* loaddataMap, OpenFileType type);
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
	void setGridIndex(int colCount,int rowCount);	// 设置格网索引
	void setStyle(int mapIndex, int layerIndex); //设置style
	void refreshStyle(int mapIndex, int layerIndex);
	void gridInfo();
	void showAttrTable(int mapIndex, int layerIndex);
	void voiceSearchRegion();
	void refreshSelectFeature(QString replyStr);//工具函数，刷新渲染类
	void openKernelTool(); //打开核密度工具面板
	void setQuadTreeIndex();  //设置四叉树四叉树索引
	void readShpToLayer();
	void readGeoJsonToLayer();
	void readPostgisToLayer();
	void openAccessAnalyTool();
private:
	Ui::GeoJsonPareseClass ui;
};

#endif // GEOJSONPARESE_H
