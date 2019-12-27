#include "geojsonparese.h"
#include <QGUIApplication>
#include "trianglewindow.h"
#include "qpainterwidget.h"
#include "myopenglwidget.h"
#include "GdalUtil.h"
#include "qdom.h"
#include "SldUtil.h"
#include "StyleWidget.h"
#include "AudioInputWidget.h"
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "MapPrjMercator.h"
#include "AccessibilityUtil.h"
#include "EuclideanDistanceUtil.h"
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebChannel/qwebchannel.h>
#include "Bridge.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/*GeoJsonParese w;
	w.show();*/
	/*
		LOADING
	*/
	QWebEngineView* m_webView = new QWebEngineView();
	QWebChannel *channel = new QWebChannel();
	Bridge * instance = Bridge::instance();
	instance->m_webView = m_webView;
	channel->registerObject("bridge", (QObject*)instance);
	m_webView->page()->setWebChannel(channel);
	m_webView->page()->load(QUrl("file:///C:/Users/Administrator/Desktop/201706221500/index.html"));
	m_webView->show();

	/*QString htmlPath = "C:\\Users\\Administrator\\Desktop\\201706221500\\index.html";
	QFile file(htmlPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		return 0;
	}
	QString htmlData = file.readAll().constData();
	file.close();
	m_webView->page()->setHtml(htmlData, QUrl("C:\\Users\\Administrator\\Desktop\\201706221500"));*/
	//QBarSet *set0 = new QBarSet("Jane");
	//*set0 << 1 << 2 << 3 << 4 << 5 << 6;

	//QBarSeries *series = new QBarSeries();
	//series->append(set0);

	//QChart *chart = new QChart();
	//chart->addSeries(series);

	//QStringList categories;
	//categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
	//QBarCategoryAxis *axis = new QBarCategoryAxis();
	//axis->append(categories);
	//chart->createDefaultAxes();
	//chart->setAxisX(axis, series);

	//chart->legend()->setVisible(true);
	//chart->legend()->setAlignment(Qt::AlignBottom);

	//QChartView *chartView = new QChartView(chart);
	//chartView->setRenderHint(QPainter::Antialiasing);

	//chartView->show();


	/*EuclideanDistanceUtil e;
	double distance = e.computeDistanceByLatLng(39.9465141, 116.906654, 39.945908, 116.906084);
	qDebug() << distance << endl;
	system("pause");*/

	return a.exec();

	//MapPrjMercator * m = new MapPrjMercator;
	//double x, y;
	//m->getXY(122.10626710683903, 37.48735577057014, &x, &y);
	//qDebug() << "x:" << x << "y:" << y << endl;
	//double b, l;
	//m->getBL(x, y, &l, &b);
	//qDebug() << "l:" << l << "b:" << b << endl;
	//system("pause");

	/*QVariant k(123);
	QVariant::Type type = k.type();
	if (QVariant::Type::Int == type)
		qDebug() << "111111";*/
	//Layer* layer = new Layer;
	//QDomDocument* doc = new QDomDocument;
	//doc = SldUtil::sldRead("C:\\Users\\123\\Desktop\\datas\\centrl_ply_sld3.sld");
	//SldUtil::parseSldDom(doc, layer);

	//QSurfaceFormat format;
 //format.setSamples(16);

 //TriangleWindow window;
 //window.setFormat(format);
 //window.resize(640, 480);
 //window.show();

 //window.setAnimating(true);
	//QPainterWidget qw;
	//qw.show();

	//OGREnvelope *maxEnvolope = new OGREnvelope;
	//for(int i=0;i<poDS->GetLayerCount();i++){
	//	OGRLayer *layer = poDS->GetLayer(i);
	//	OGREnvelope *envolope = new OGREnvelope;
	//	layer->GetExtent(envolope);
	//	maxEnvolope->Merge(*envolope);
	//}
	//double max_x = maxEnvolope->MaxX;
	//double min_x = maxEnvolope->MinX;
	//double max_y = maxEnvolope->MaxY;
	//double min_y = maxEnvolope->MinY;
	//OGRDataSource *poDS = GdalUtil::readShp("C:\\Users\\Administrator\\Desktop\\shapefile\\border.shp");

	//GeoMap *geoMap = GdalUtil::OGRDataSource2Map(poDS);
	//MyOpenGLWidget my(geoMap);
	//my.show();
}
