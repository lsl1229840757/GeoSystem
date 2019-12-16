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
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/*AudioInputWidget ad;
	ad.show();*/
	GeoJsonParese w;
	w.show();
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
	return a.exec();
}
