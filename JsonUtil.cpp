#include "JsonUtil.h"
//��ʼ����̬����
QString JsonUtil::TYPE = "type";
QString JsonUtil::GEOMETRY = "geometry";
QString JsonUtil::PROPERTIES = "properties";
QString JsonUtil::FEATURE = "Feature";
QString JsonUtil::POINT = "Point";
QString JsonUtil::POLYLINE = "LineString";
QString JsonUtil::POLYGON = "Polygon";
QString JsonUtil::COORDINATES = "coordinates";

JsonUtil::JsonUtil(void)
{
}


JsonUtil::~JsonUtil(void)
{
}

QJsonObject JsonUtil::JsonRead(QString filePath) throw(runtime_error){
    //���ļ���
    QFile loadFile(filePath);
    if(!loadFile.open(QIODevice::ReadOnly))
    {
        runtime_error r1("could't open projects json");
        throw r1;
    }
    QByteArray allData = loadFile.readAll();
    loadFile.close();
    // json��������
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));
    //�������ͷ���
    if(jsonError.error != QJsonParseError::NoError){
        runtime_error r1("json error!");
        throw r1;
    }
    QJsonObject rootObj = jsonDoc.object();
    return rootObj;
}

Feature JsonUtil::parseGeoJson(QJsonObject geoJson){
    //����type
    QString type = geoJson.take(TYPE).toString();
    if(FEATURE.compare(type)==0){
        Feature feature = parseFeature(geoJson);
        return feature;
    }
    Feature feature;
    return feature;
}
//TODO
Feature JsonUtil::parseFeature(QJsonObject featureJson) throw(runtime_error){
    Feature feature;
    //��������
    QJsonObject properties = featureJson.take(PROPERTIES).toObject();
    feature.properties = properties;
    //��ȡgeomery
    QJsonObject geometryJson = featureJson.take(GEOMETRY).toObject();
    //��ʼ����geometry
    QString type = geometryJson.take(TYPE).toString();
    //TODO ��������е��ظ�����Ϊ�˷����������geometry�Ĳ�ͬҪ��,�Ժ������ӵĻ����Խ����ٷ�װ��һ������
    if(POINT.compare(type)==0){
        //����ǵ�����
        Geometry *geometry = new GeoPoint();
        //��ȡ��������
        QJsonArray coordinates = geometryJson.take(COORDINATES).toArray();
        geometry->coordinates = coordinates;
        feature.geometry = geometry;
    }else if(POLYLINE.compare(type)==0){
        //���������
		Geometry *geometry = new GeoPolyline();
        //��ȡ��������
        QJsonArray coordinates = geometryJson.take(COORDINATES).toArray();
        geometry->coordinates = coordinates;
        feature.geometry = geometry;
    }else if(POLYGON.compare(type)==0){
        //���������
        Geometry *geometry = new GeoPolygon();
        //��ȡ��������
        QJsonArray coordinates = geometryJson.take(COORDINATES).toArray();
        geometry->coordinates = coordinates;
        feature.geometry = geometry;
    }
    return feature;
}
