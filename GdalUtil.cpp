#include "GdalUtil.h"


GdalUtil::GdalUtil(void)
{
}


GdalUtil::~GdalUtil(void)
{
}

void GdalUtil::readShp(QString filePath){
	//ע������
	GDALAllRegister();	
	//gdal���ݼ�
	GDALDataset *poDS;
	//��ȡ���ݼ�
	poDS = (GDALDataset*) GDALOpenEx(filePath.toStdString().c_str(),GDAL_OF_VECTOR,NULL,NULL,NULL);
	//���Ϊ��
	if(poDS==NULL)
	{
		return;
	}
	//��ȡͼ��
	OGRLayer *poLayer;
	//����
	poLayer = poDS->GetLayer(0);
	//׼��feature
	OGRFeature *poFeature;
	while((poFeature=poLayer->GetNextFeature())!=NULL){
		GIntBig fid = poFeature->GetFID();
		qDebug()<<fid;
	}
}

void GdalUtil::shp2GeoJson(QString inFilePath, QString outFilePath){
	GDALAllRegister();	
	
	//gdal���ݼ�
	GDALDataset *poDS;
	//��ȡ���ݼ�
	poDS = (GDALDataset*) GDALOpenEx(inFilePath.toStdString().c_str(),GDAL_OF_VECTOR,NULL,NULL,NULL);
	//���Ϊ��
	if(poDS==NULL)
	{
		return;
	}
	GDALDriver  *dv = NULL;
	dv = GetGDALDriverManager()->GetDriverByName("GeoJson");
	if(dv==NULL){
		return;
	}
	// ����ļ�
	OGR_Dr_CopyDataSource(dv, poDS, outFilePath.toStdString().c_str(),NULL);
	// �ر��ļ�
	GDALClose( poDS );
}

OGRDataSource * GdalUtil::readFromPgsql(QString tableName, QString dbname, QString addr, QString port, QString username, QString password){
	OGRRegisterAll();

	//ƴ��params
	QString params = "PG:dbname="+dbname+" host="+addr+" port="+port+" user="+username+" password="+password;
	//�����ȡ�����ݿ������е�Table��ÿ��table��һ��ͼ��
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
	//ƴ��params
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
	//׼��feature
	OGRFeature *poFeature;
	while((poFeature=pLayer->GetNextFeature())!=NULL){
		GIntBig fid = poFeature->GetFID();
		qDebug()<<fid;
	}
	return poDS;
}

OGRDataSource * GdalUtil::readFromGeoJson(QString filePath){
	GDALAllRegister();	
	//CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO"); //֧������·�� //����Ҫ
	//gdal���ݼ�
	OGRDataSource *poDS;
	//��ȡ���ݼ�
	poDS = (OGRDataSource*) GDALOpenEx(filePath.toStdString().c_str(),GDAL_OF_VECTOR,NULL,NULL,NULL);
	//���Ϊ��
	if(poDS==NULL)
	{
		return poDS;
	}
	return poDS;
}

GeoMap* GdalUtil::OGRDataSource2Map(OGRDataSource *poDS){
	GeoMap *map = new GeoMap;
	CPLSetConfigOption("SHAPE_ENCODING", ""); //֧�������ֶ�

	//��ʼ����feature�е���Ϣ
	for(int i=0;i<poDS->GetLayerCount();i++){ //��ȡ��DataSource�����е�ͼ��
		//��ʼת��layer
		OGRLayer *ogrLayer = poDS->GetLayer(i);
		Layer *layer = OGRLayer2Layer(ogrLayer, i);
		map->addLayer(layer);//��������map�ķ�Χ
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
	//Ŀǰֻ�ǿ����⻷
	OGRLinearRing *exteriorRing = ogrPolygon->getExteriorRing();
	for(int i=0;i<exteriorRing->getNumPoints();i++){
		OGRPoint *ogrPoint = new OGRPoint;
		GeoPoint *geoPoint = new GeoPoint;
		exteriorRing->getPoint(i, ogrPoint);
		ogrPoint2GeoPoint(ogrPoint, geoPoint);
		geoPolygon->points.push_back(geoPoint);
	}
}


// ת������
void GdalUtil::ogrMultiPly2GeoMultiPly(OGRMultiPolygon* ogrMultiPly, GeoMultiPolygon* geoMultiPly)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	for (int i = 0; i < ogrMultiPly->getNumGeometries(); i++)
	{
		OGRPolygon* ogrPolygon = (OGRPolygon* )ogrMultiPly->getGeometryRef(i);
		GeoPolygon* geoPolygon = new GeoPolygon;
		geoMultiPly->polygons.push_back(geoPolygon);
		//��ÿ�������ת��
		ogrPolygon2GeoPolygon(ogrPolygon, geoPolygon);
	}
	
}


void GdalUtil::storeAttributes(Feature* feature, OGRFeature* poFeature, OGRFeatureDefn* poFeatureDefn)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	//qDebug()<<poFDefn->GetName();  //shpҪ�ؼ�����ͼ����һ��  
	//OGRFeature��GetFiledIndex()���ֶ�����ȡ����ֵ

	int fieldCount = poFeatureDefn->GetFieldCount();//��ȡ�ֶ�����������ǰ�����ֶ�(FID��Shape)
	//���ÿ���ֶ�������ֵ
	for (int j = 0; j < fieldCount; j++)
	{
		OGRFieldDefn* poFiledDefn = poFeatureDefn->GetFieldDefn(j);//��ȡÿ���ֶεĶ���
		OGRFieldType fieldType = poFiledDefn->GetType();  //��ȡ�ֶ�����
		//qDebug() << poFiledDefn->GetNameRef();  
		//qDebug() << poFeature->GetFieldAsString(j);
		QString fieldName = poFiledDefn->GetNameRef();//�ֶ���
		if (OFTInteger == fieldType||OFTInteger64==fieldType)
		{
			//����
			feature->attributes.insert(fieldName, poFeature->GetFieldAsInteger(j));
		}
		else if (OFTReal == fieldType)
		{
			//ʵ��,����ARMƽ̨(Ƕ��ʽ)�൱��float��QReal��windows���൱��double
			feature->attributes.insert(fieldName, poFeature->GetFieldAsDouble(j));
		}
		else if (OFTString == fieldType)
		{
			//�ַ���
			feature->attributes.insert(fieldName, poFeature->GetFieldAsString(j));
		}
		else
		{
			//����������ʱ����
			qDebug() << "Could not support this type at present.";
		}
	}
}


// Ϊ���ݿ�ĵ�������ת������
GeoMap* GdalUtil::OGRDataSource2Map(OGRDataSource *poDS, QString tableName)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	GeoMap *map = new GeoMap;
	CPLSetConfigOption("SHAPE_ENCODING", ""); //֧�������ֶ�
	//��ʼת��layer
	OGRLayer *ogrLayer = poDS->GetLayerByName(tableName.toStdString().data());
	if (ogrLayer == NULL)
	{
		qDebug() << "No such layer named " << tableName;
		return nullptr;
	}
	int layerID = 0;  //Ĭ��tableΪ�½�map�ĵ�һ��ͼ��
	Layer *layer = OGRLayer2Layer(ogrLayer,layerID);
	map->addLayer(layer);//��������map�ķ�Χ
	return map;
}




// OGRLayerת����Layer
Layer* GdalUtil::OGRLayer2Layer(OGRLayer *ogrLayer,int layerID)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	Layer *layer = new Layer;
	layer->layerID = layerID;
	QString name = ogrLayer->GetName();
	layer->name = name.toStdString();
	//����layer�ķ�Χ����
	OGREnvelope *envelope = new OGREnvelope;
	ogrLayer->GetExtent(envelope);
	double min_x = envelope->MinX;
	double max_x = envelope->MaxX;
	double min_y = envelope->MinY;
	double max_y = envelope->MaxY;
	//QRectF rect(QPointF(min_x,max_y), QPointF(max_x,min_y));
	QRectF rect(QPointF(min_x, min_y), QPointF(max_x, max_y));  //QRectF y������
	layer->range = rect;
	OGRFeature *poFeature;
	int feaID = 0; //��¼Ҫ��ID
	//��ȡҪ�ظ���
	int featureCount = ogrLayer->GetFeatureCount();
	while ((poFeature = ogrLayer->GetNextFeature()) != NULL) {
		//��ʼת��feature
		Feature* feature = new Feature;
		feature->featureID = feaID;   //����Ҫ��ID
		feaID++;
		//��ȡ��ǰҪ�ؼ��Ķ���
		OGRFeatureDefn* poFeatureDefn = ogrLayer->GetLayerDefn();
		//��ʼת������Attributes
		storeAttributes(feature, poFeature, poFeatureDefn);
		//��ʼת��geometry
		OGRGeometry *ogrGeometry = poFeature->GetGeometryRef();
		int a = ogrGeometry->getGeometryType();
		if (OGRwkbGeometryType::wkbPoint == ogrGeometry->getGeometryType()) {
			//��ת��
			OGRPoint *ogrPoint = (OGRPoint *)ogrGeometry;
			//�洢OGR����
			feature->ogrGeom = ogrPoint;
			//�洢ΪGEOS����
			feature->geosGeom = MGeosUtil::OGR2GEOSGeom(feature->ogrGeom);

			GeoPoint *geoPoint = new GeoPoint;
			ogrPoint2GeoPoint(ogrPoint, geoPoint);
			feature->geometry = geoPoint;
		}
		else if (OGRwkbGeometryType::wkbLineString == ogrGeometry->getGeometryType()) {
			//��ת��
			OGRLineString *lineString = (OGRLineString *)ogrGeometry;
			//�洢OGR����
			feature->ogrGeom = lineString;
			//�洢ΪGEOS����
			feature->geosGeom = MGeosUtil::OGR2GEOSGeom(feature->ogrGeom);
			GeoPolyline *geoPolyline = new GeoPolyline;
			ogrLineString2GeoPolyline(lineString, geoPolyline);
			feature->geometry = geoPolyline;
		}
		else if (OGRwkbGeometryType::wkbPolygon == ogrGeometry->getGeometryType()) {
			OGRPolygon *ogrPolygon = (OGRPolygon *)ogrGeometry;
			//�洢OGR����
			feature->ogrGeom = ogrPolygon;
			//�洢ΪGEOS����
			feature->geosGeom = MGeosUtil::OGR2GEOSGeom(feature->ogrGeom);
			GeoPolygon *geoPolygon = new GeoPolygon;
			//��ʱֻ�ǿ����⻷
			ogrPolygon2GeoPolygon(ogrPolygon, geoPolygon);
			feature->geometry = geoPolygon;
		}
		else if (OGRwkbGeometryType::wkbMultiPolygon == ogrGeometry->getGeometryType()) {
			//����ת��
			OGRMultiPolygon* ogrMultiPolygon = (OGRMultiPolygon*)ogrGeometry;
			//�洢OGR����
			feature->ogrGeom = ogrMultiPolygon;
			//�洢ΪGEOS����
			feature->geosGeom = MGeosUtil::OGR2GEOSGeom(feature->ogrGeom);
			GeoMultiPolygon* geoMultiPolygon = new GeoMultiPolygon;
			ogrMultiPly2GeoMultiPly(ogrMultiPolygon, geoMultiPolygon);
			feature->geometry = geoMultiPolygon;
		}
		else
		{
			//��������δ��������,��ʱ�ȼٶ�������multipolyline����ʵ��
			OGRMultiLineString* ogrMultiLineStr = (OGRMultiLineString*)ogrGeometry;
			//�洢OGR����
			feature->ogrGeom = ogrMultiLineStr;
			//�洢ΪGEOS����
			feature->geosGeom = MGeosUtil::OGR2GEOSGeom(feature->ogrGeom);
			GeoMultiPolyline* geoMultiPolyline = new GeoMultiPolyline;
			feature->geometry = geoMultiPolyline;
			//QMessageBox::warning(NULL, "Error", "Could not support this type");
		}
		layer->features.push_back(feature);
	}
	return layer;
}


// //д��tiff
void GdalUtil::writeGeoTiff(QString outputPath,QRectF extent,vector<vector<double>>* outputMtx)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	//ע������
	GDALAllRegister();
	//����������ݼ�
	GDALDataset *poDatasetCreate;
	//������������������
	GDALDriver *poDriver;
	char *pszFormat = "GTiff";
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	//��ȡ���ݼ�
	int xsizes = outputMtx->size(); //����
	int ysizes = outputMtx->back().size();  //����
	char **papszMetadata = poDriver->GetMetadata();
	string da = outputPath.toStdString();
	const char *dd = da.data();
	poDatasetCreate = poDriver->Create(outputPath.toStdString().data(), xsizes, ysizes, 1, GDT_Float64, papszMetadata);
	//���÷���任ϵ��:���Ͻ�����x , sizeX , ָ��Ϊ0 , ���Ͻ�����y ,ָ��Ϊ0�� -sizeY 
	double geoTransForm[6] = { extent.left(),xsizes,0,extent.top(),0,-ysizes };
	poDatasetCreate->SetGeoTransform(geoTransForm);
	//����tiff�ռ�ο�
	OGRSpatialReference oSRS;
	oSRS.SetWellKnownGeogCS("WGS84");
	char *pszWKT = NULL;
	oSRS.exportToWkt(&pszWKT);
	poDatasetCreate->SetProjection(pszWKT);
	//ת��ΪgdalҪ��
	double *buffer = new double[xsizes*ysizes];
	for (int i = 0; i < xsizes*ysizes; i++)
	{
		buffer[i] = outputMtx->at(i%xsizes).at(i/xsizes); //
	}
	//���դ��
	poDatasetCreate->RasterIO(GF_Write, 0, 0, xsizes, ysizes, buffer, xsizes, ysizes, GDT_Float64, 1, 0, 0, 0, 0);
	GDALClose(GDALDatasetH(poDatasetCreate));
}
