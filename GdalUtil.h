#pragma once
#include "gdal.h"
#include "ogr_api.h"
#include "ogr_feature.h"
#include "ogrsf_frmts.h"
#include <qstring.h>
#include <qdebug.h>
#include "GeoMap.h"
#include "GeoPoint.h"
#include "GeoPolyline.h"
#include "GeoPolygon.h"
#include "GeoMultiPolygon.h"
#include "qmessagebox.h"
#include "GeoMultiPolyline.h"
#include "geos.h"
class GdalUtil
{
public:
	GdalUtil(void);
	~GdalUtil(void);
	//暂时返回void
	void static readShp(QString filePath);
	void static shp2GeoJson(QString inFilePath, QString outFilePath);
	static OGRDataSource* readFromPgsql(QString tableName, QString dbname, QString addr="localhost", QString port="5432", QString username="postgres", QString password="123456");
	static OGRDataSource* readFromPgsqlLocal(QString tableName);
	static OGRDataSource* readFromGeoJson(QString filePath);
	static GeoMap* OGRDataSource2Map(OGRDataSource *);
	static void ogrPoint2GeoPoint(OGRPoint *ogrPoint, GeoPoint *point);
	static void ogrLineString2GeoPolyline(OGRLineString *ogrPolyline, GeoPolyline *geoPolyline);
	static void ogrPolygon2GeoPolygon(OGRPolygon *ogrPolygon, GeoPolygon *geoPolygon);
	// 转换多面
	static void ogrMultiPly2GeoMultiPly(OGRMultiPolygon* ogrMultiPly, GeoMultiPolygon* geoMultiPly);
private:
	static void storeAttributes(Feature* feature, OGRFeature* poFeature, OGRFeatureDefn* poFeatureDefn);
public:
	// 为数据库的调用重载转换函数
	static GeoMap* OGRDataSource2Map(OGRDataSource *poDS, QString tableName);

	// OGRLayer转换到Layer
	static Layer* OGRLayer2Layer(OGRLayer *ogrLayer,int layerID);
	// //写入tiff
	static void writeGeoTiff(QString outputPath, QRectF extent, vector<vector<double>>* outputMtx);
};