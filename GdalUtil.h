#pragma once
#include "gdal.h"
#include "ogr_api.h"
#include "ogrsf_frmts.h"
#include <qstring.h>
#include <qdebug.h>
#include "GeoMap.h"
#include "GeoPoint.h"
#include "GeoPolyline.h"
#include "GeoPolygon.h"
class GdalUtil
{
public:
	GdalUtil(void);
	~GdalUtil(void);
	//ÔÝÊ±·µ»Øvoid
	void static readShp(QString filePath);
	void static shp2GeoJson(QString inFilePath, QString outFilePath);
	static OGRDataSource* readFromPgsql(QString tableName, QString dbname, QString addr="localhost", QString port="5432", QString username="postgres", QString password="12345");
	static OGRDataSource* readFromPgsqlLocal(QString tableName);
	static OGRDataSource* readFromGeoJson(QString filePath);
	static GeoMap* OGRDataSource2Map(OGRDataSource *);
	static void ogrPoint2GeoPoint(OGRPoint *ogrPoint, GeoPoint *point);
	static void ogrLineString2GeoPolyline(OGRLineString *ogrPolyline, GeoPolyline *geoPolyline);
	static void ogrPolygon2GeoPolygon(OGRPolygon *ogrPolygon, GeoPolygon *geoPolygon);
};