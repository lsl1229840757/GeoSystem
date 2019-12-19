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
int main(int argc, char *argv[])
{

	/*EuclideanDistanceUtil e;
	double distance = e.computeDistanceByLatLng(39.9465141, 116.906654, 39.945908, 116.906084);
	qDebug() << distance << endl;
	system("pause");*/
	//QApplication a(argc, argv);
	//GeoJsonParese w;
	//w.show();
	//return a.exec();

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
