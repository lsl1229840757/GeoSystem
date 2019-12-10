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
	//这里读取了数据库中所有的Table，每个table是一个图层
	OGRLayer *pLayer = NULL;
	OGRDataSource *poDS = NULL; 
	poDS = (OGRDataSource*)GDALOpenEx(params.toStdString().c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);
	//GDALDriverManager::GetDriverByName(params.toStdString().c_str());
	if (poDS == NULL)
	{
		 qDebug()<<"Can not open database"<<endl;
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
	//CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO"); //支持中文路径 //不需要
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
	CPLSetConfigOption("SHAPE_ENCODING", ""); //支持中文字段
	//开始绘制feature中的信息
	for(int i=0;i<poDS->GetLayerCount();i++){ //读取了DataSource中所有的图层
		//开始转换layer
		OGRLayer *ogrLayer = poDS->GetLayer(i);
		QString name = ogrLayer->GetName();
		Layer *layer = new Layer;
		//构建layer的范围矩形
		OGREnvelope *envelope = new OGREnvelope;
		ogrLayer->GetExtent(envelope);
		double min_x = envelope->MinX;
		double max_x = envelope->MaxX;
		double min_y = envelope->MinY;
		double max_y = envelope->MaxY;
		//QRectF rect(QPointF(min_x,max_y), QPointF(max_x,min_y));
		QRectF rect(QPointF(min_x, min_y), QPointF(max_x, max_y));  //QRectF y轴向下
		layer->range = rect;
		OGRFeature *poFeature;
		//获取要素个数
		int featureCount = ogrLayer->GetFeatureCount();
		while((poFeature=ogrLayer->GetNextFeature())!=NULL){
			//开始转换feature
			Feature* feature = new Feature;
			//获取当前要素集的定义
			OGRFeatureDefn* poFeatureDefn = ogrLayer->GetLayerDefn();
			//开始转换属性Attributes
			storeAttributes(feature, poFeature, poFeatureDefn);
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
			}else if (OGRwkbGeometryType::wkbMultiPolygon == ogrGeometry->getGeometryType()){
				//多面转换
				OGRMultiPolygon* ogrMultiPolygon = (OGRMultiPolygon*)ogrGeometry;
				GeoMultiPolygon* geoMultiPolygon = new GeoMultiPolygon;
				ogrMultiPly2GeoMultiPly(ogrMultiPolygon, geoMultiPolygon);
				feature->geometry = geoMultiPolygon;
			}
			else
			{
				//处理其他未定义类型,暂时先假定他们是multipolyline，不实现
				OGRMultiLineString* ogrMultiLineStr = (OGRMultiLineString*)ogrGeometry;
				GeoMultiPolyline* geoMultiPolyline = new GeoMultiPolyline;

				feature->geometry = geoMultiPolyline;
				//QMessageBox::warning(NULL, "Error", "Could not support this type");
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


// 转换多面
void GdalUtil::ogrMultiPly2GeoMultiPly(OGRMultiPolygon* ogrMultiPly, GeoMultiPolygon* geoMultiPly)
{
	// TODO: 在此处添加实现代码.
	for (int i = 0; i < ogrMultiPly->getNumGeometries(); i++)
	{
		OGRPolygon* ogrPolygon = (OGRPolygon* )ogrMultiPly->getGeometryRef(i);
		GeoPolygon* geoPolygon = new GeoPolygon;
		geoMultiPly->polygons.push_back(geoPolygon);
		//对每个面进行转换
		ogrPolygon2GeoPolygon(ogrPolygon, geoPolygon);
	}
	
}


void GdalUtil::storeAttributes(Feature* feature, OGRFeature* poFeature, OGRFeatureDefn* poFeatureDefn)
{
	// TODO: 在此处添加实现代码.
	//qDebug()<<poFDefn->GetName();  //shp要素集名与图层名一致  
	//OGRFeature的GetFiledIndex()按字段名获取索引值

	int fieldCount = poFeatureDefn->GetFieldCount();//获取字段数，不包括前两个字段(FID，Shape)
	//输出每个字段名称与值
	for (int j = 0; j < fieldCount; j++)
	{
		OGRFieldDefn* poFiledDefn = poFeatureDefn->GetFieldDefn(j);//获取每个字段的定义
		OGRFieldType fieldType = poFiledDefn->GetType();  //获取字段类型
		//qDebug() << poFiledDefn->GetNameRef();  
		//qDebug() << poFeature->GetFieldAsString(j);
		QString fieldName = poFiledDefn->GetNameRef();//字段名
		if (OFTInteger == fieldType||OFTInteger64==fieldType)
		{
			//整数
			feature->attributes.insert(fieldName, poFeature->GetFieldAsInteger(j));
		}
		else if (OFTReal == fieldType)
		{
			//实数,除了ARM平台(嵌入式)相当于float，QReal在windows上相当于double
			feature->attributes.insert(fieldName, poFeature->GetFieldAsDouble(j));
		}
		else if (OFTString == fieldType)
		{
			//字符串
			feature->attributes.insert(fieldName, poFeature->GetFieldAsString(j));
		}
		else
		{
			//其他类型暂时不存
			qDebug() << "Could not support this type at present.";
		}
	}
}


// 为数据库的调用重载转换函数
GeoMap* GdalUtil::OGRDataSource2Map(OGRDataSource *poDS, QString tableName)
{
	// TODO: 在此处添加实现代码.
	GeoMap *map = new GeoMap;
	CPLSetConfigOption("SHAPE_ENCODING", ""); //支持中文字段
	//开始转换layer
	OGRLayer *ogrLayer = poDS->GetLayerByName(tableName.toStdString().data());
	if (ogrLayer == NULL)
	{
		qDebug() << "No such layer named " << tableName;
		return nullptr;
	}
	//开始绘制feature中的信息
	Layer *layer = new Layer;
	//构建layer的范围矩形
	OGREnvelope *envelope = new OGREnvelope;
	ogrLayer->GetExtent(envelope);
	double min_x = envelope->MinX;
	double max_x = envelope->MaxX;
	double min_y = envelope->MinY;
	double max_y = envelope->MaxY;
	//QRectF rect(QPointF(min_x,max_y), QPointF(max_x,min_y));
	QRectF rect(QPointF(min_x, min_y), QPointF(max_x, max_y));  //QRectF y轴向下
	layer->range = rect;
	OGRFeature *poFeature;
	//获取要素个数
	int featureCount = ogrLayer->GetFeatureCount();
	while ((poFeature = ogrLayer->GetNextFeature()) != NULL) {
		//开始转换feature
		Feature* feature = new Feature;
		//获取当前要素集的定义
		OGRFeatureDefn* poFeatureDefn = ogrLayer->GetLayerDefn();
		//开始转换属性Attributes
		storeAttributes(feature, poFeature, poFeatureDefn);
		//开始转换geometry
		OGRGeometry *ogrGeometry = poFeature->GetGeometryRef();
		int a = ogrGeometry->getGeometryType();
		if (OGRwkbGeometryType::wkbPoint == ogrGeometry->getGeometryType()) {
			//点转换
			OGRPoint *ogrPoint = (OGRPoint *)ogrGeometry;
			GeoPoint *geoPoint = new GeoPoint;
			ogrPoint2GeoPoint(ogrPoint, geoPoint);
			feature->geometry = geoPoint;
		}
		else if (OGRwkbGeometryType::wkbLineString == ogrGeometry->getGeometryType()) {
			//线转换
			OGRLineString *lineString = (OGRLineString *)ogrGeometry;
			GeoPolyline *geoPolyline = new GeoPolyline;
			ogrLineString2GeoPolyline(lineString, geoPolyline);
			feature->geometry = geoPolyline;
		}
		else if (OGRwkbGeometryType::wkbPolygon == ogrGeometry->getGeometryType()) {
			OGRPolygon *ogrPolygon = (OGRPolygon *)ogrGeometry;
			GeoPolygon *geoPolygon = new GeoPolygon;
			//暂时只是考虑外环
			ogrPolygon2GeoPolygon(ogrPolygon, geoPolygon);
			feature->geometry = geoPolygon;
		}
		else if (OGRwkbGeometryType::wkbMultiPolygon == ogrGeometry->getGeometryType()) {
			//多面转换
			OGRMultiPolygon* ogrMultiPolygon = (OGRMultiPolygon*)ogrGeometry;
			GeoMultiPolygon* geoMultiPolygon = new GeoMultiPolygon;
			ogrMultiPly2GeoMultiPly(ogrMultiPolygon, geoMultiPolygon);
			feature->geometry = geoMultiPolygon;
		}
		else
		{
			//处理其他未定义类型,暂时先假定他们是multipolyline，不实现
			OGRMultiLineString* ogrMultiLineStr = (OGRMultiLineString*)ogrGeometry;
			GeoMultiPolyline* geoMultiPolyline = new GeoMultiPolyline;
			feature->geometry = geoMultiPolyline;
			//QMessageBox::warning(NULL, "Error", "Could not support this type");
		}
		layer->features.push_back(feature);
	}
	map->addLayer(layer);//附带跟新map的范围
	return map;
}
