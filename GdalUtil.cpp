#include "GdalUtil.h"


GdalUtil::GdalUtil(void)
{
}


GdalUtil::~GdalUtil(void)
{
}

void GdalUtil::readShp(QString filePath){
	//注册驱动
	GDALAllRegister();	
	//gdal数据集
	GDALDataset *poDS;
	//读取数据集
	poDS = (GDALDataset*) GDALOpenEx(filePath.toStdString().c_str(),GDAL_OF_VECTOR,NULL,NULL,NULL);
	//如果为空
	if(poDS==NULL)
	{
		return;
	}
	//获取图层
	OGRLayer *poLayer;
	//测试
	poLayer = poDS->GetLayer(0);
	//准备feature
	OGRFeature *poFeature;
	while((poFeature=poLayer->GetNextFeature())!=NULL){
		GIntBig fid = poFeature->GetFID();
		qDebug()<<fid;
	}
}

void GdalUtil::shp2GeoJson(QString inFilePath, QString outFilePath){
	GDALAllRegister();	
	//gdal数据集
	GDALDataset *poDS;
	//读取数据集
	poDS = (GDALDataset*) GDALOpenEx(inFilePath.toStdString().c_str(),GDAL_OF_VECTOR,NULL,NULL,NULL);
	//如果为空
	if(poDS==NULL)
	{
		return;
	}
	GDALDriver  *dv = NULL;
	dv = GetGDALDriverManager()->GetDriverByName("GeoJson");
	if(dv==NULL){
		return;
	}
	// 输出文件
	OGR_Dr_CopyDataSource(dv, poDS, outFilePath.toStdString().c_str(),NULL);
	// 关闭文件
	GDALClose( poDS );
}

OGRDataSource * GdalUtil::readFromPgsql(QString tableName, QString dbname, QString addr, QString port, QString username, QString password){
	OGRRegisterAll();
	//拼接params
	QString params = "PG:dbname="+dbname+" host="+addr+" port="+port+" user="+username+" password="+password;
	OGRLayer *pLayer = NULL;
	OGRDataSource *poDS = NULL; 
		poDS = (OGRDataSource*)GDALOpenEx(params.toStdString().c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);
	//GDALDriverManager::GetDriverByName(params.toStdString().c_str());
	if (poDS == NULL)
	{
		 qDebug()<<"Can not open database"<<endl;
		 return poDS;
	}
	pLayer = poDS->GetLayerByName(tableName.toStdString().c_str());
	if (pLayer == NULL)
	{
		qDebug()<<"Can not get layer "<<tableName<<endl;
		return poDS;
	}
	return poDS;
}

OGRDataSource * GdalUtil::readFromPgsqlLocal(QString tableName){
	OGRRegisterAll();
	//拼接params
	QString params = "PG:dbname=postgis_23_sample host=localhost port=5432 user=postgres password=12345";
	OGRLayer *pLayer = NULL;
	OGRDataSource *poDS = NULL; 
	poDS = (OGRDataSource*) GDALOpenEx(params.toStdString().c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL );
	if (poDS == NULL)
	{
		qDebug()<<"Can not open database"<<endl;
		return poDS;
	}
	pLayer = poDS->GetLayerByName(tableName.toStdString().c_str());
	if (pLayer == NULL)
	{
		qDebug()<<"Can not get layer "<<tableName<<endl;
		return poDS;
	}
	qDebug()<<"feature count of layer1 is "<<pLayer->GetFeatureCount(1)<<endl;
	//准备feature
	OGRFeature *poFeature;
	while((poFeature=pLayer->GetNextFeature())!=NULL){
		GIntBig fid = poFeature->GetFID();
		qDebug()<<fid;
	}
	return poDS;
}

OGRDataSource * GdalUtil::readFromGeoJson(QString filePath){
	GDALAllRegister();	
	//gdal数据集
	OGRDataSource *poDS;
	//读取数据集
	poDS = (OGRDataSource*) GDALOpenEx(filePath.toStdString().c_str(),GDAL_OF_VECTOR,NULL,NULL,NULL);
	//如果为空
	if(poDS==NULL)
	{
		return poDS;
	}
	return poDS;
}

GeoMap* GdalUtil::OGRDataSource2Map(OGRDataSource *poDS){
	GeoMap *map = new GeoMap;
	//开始绘制feature中的信息
	for(int i=0;i<poDS->GetLayerCount();i++){
		//开始转换layer
		OGRLayer *ogrLayer = poDS->GetLayer(i);
		Layer *layer = new Layer;
		//构建layer的范围矩形
		OGREnvelope *envelope = new OGREnvelope;
		ogrLayer->GetExtent(envelope);
		double min_x = envelope->MinX;
		double max_x = envelope->MaxX;
		double min_y = envelope->MinY;
		double max_y = envelope->MaxY;
		QRectF rect(QPointF(min_x,max_y), QPointF(max_x,min_y));
		layer->range = rect;
		OGRFeature *poFeature;
		while((poFeature=ogrLayer->GetNextFeature())!=NULL){
			//开始转换feature
			Feature* feature = new Feature;
			//开始转换geometry
			OGRGeometry *ogrGeometry = poFeature->GetGeometryRef();
			int a = ogrGeometry->getGeometryType();
			if(OGRwkbGeometryType::wkbPoint==ogrGeometry->getGeometryType()){
				//点转换
				OGRPoint *ogrPoint = (OGRPoint *)ogrGeometry;
				GeoPoint *geoPoint = new GeoPoint;
				ogrPoint2GeoPoint(ogrPoint, geoPoint);
				feature->geometry = geoPoint;
			}else if(OGRwkbGeometryType::wkbLineString==ogrGeometry->getGeometryType()){
				//线转换
				OGRLineString *lineString = (OGRLineString *)ogrGeometry;
				GeoPolyline *geoPolyline = new GeoPolyline;
				ogrLineString2GeoPolyline(lineString, geoPolyline);
				feature->geometry = geoPolyline;
			}else if(OGRwkbGeometryType::wkbPolygon==ogrGeometry->getGeometryType()){
				OGRPolygon *ogrPolygon = (OGRPolygon *)ogrGeometry;
				GeoPolygon *geoPolygon = new GeoPolygon;
				//暂时只是考虑外环
				ogrPolygon2GeoPolygon(ogrPolygon,geoPolygon);
				feature->geometry = geoPolygon;
			}
			layer->features.push_back(feature);
		}
		map->addLayer(layer);//附带跟新map的范围
	}
	return map;
}

void GdalUtil::ogrPoint2GeoPoint(OGRPoint *ogrPoint, GeoPoint *geoPoint){
	geoPoint->x = ogrPoint->getX();
	geoPoint->y = ogrPoint->getY();
}

void GdalUtil::ogrLineString2GeoPolyline(OGRLineString *lineString, GeoPolyline *geoPolyline){
	for(int i=0;i<lineString->getNumPoints();i++){
		OGRPoint *ogrPoint = new OGRPoint;
		GeoPoint *geoPoint = new GeoPoint;
		lineString->getPoint(i, ogrPoint);
		ogrPoint2GeoPoint(ogrPoint, geoPoint);
		geoPolyline->points.push_back(geoPoint);
	}
}

void GdalUtil::ogrPolygon2GeoPolygon(OGRPolygon *ogrPolygon, GeoPolygon *geoPolygon){
	//目前只是考虑外环
	OGRLinearRing *exteriorRing = ogrPolygon->getExteriorRing();
	for(int i=0;i<exteriorRing->getNumPoints();i++){
		OGRPoint *ogrPoint = new OGRPoint;
		GeoPoint *geoPoint = new GeoPoint;
		exteriorRing->getPoint(i, ogrPoint);
		ogrPoint2GeoPoint(ogrPoint, geoPoint);
		geoPolygon->points.push_back(geoPoint);
	}
}
