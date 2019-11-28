#include "JsonUtil.h"
//初始化静态变量
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
    //打开文件类
    QFile loadFile(filePath);
    if(!loadFile.open(QIODevice::ReadOnly))
    {
        runtime_error r1("could't open projects json");
        throw r1;
    }
    QByteArray allData = loadFile.readAll();
    loadFile.close();
    // json解析错误
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &jsonError));
    //如果出错就返回
    if(jsonError.error != QJsonParseError::NoError){
        runtime_error r1("json error!");
        throw r1;
    }
    QJsonObject rootObj = jsonDoc.object();
    return rootObj;
}

Feature JsonUtil::parseGeoJson(QJsonObject geoJson){
    //解析type
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
    //解析属性
    QJsonObject properties = featureJson.take(PROPERTIES).toObject();
    feature.properties = properties;
    //获取geomery
    QJsonObject geometryJson = featureJson.take(GEOMETRY).toObject();
    //开始解析geometry
    QString type = geometryJson.take(TYPE).toString();
    //TODO 这里代码有点重复，是为了方便满足各种geometry的不同要求,以后功能增加的话可以将其再封装出一个函数
    if(POINT.compare(type)==0){
        //如果是点数据
        Geometry *geometry = new GeoPoint();
        //获取坐标数据
        QJsonArray coordinates = geometryJson.take(COORDINATES).toArray();
        geometry->coordinates = coordinates;
        feature.geometry = geometry;
    }else if(POLYLINE.compare(type)==0){
        //如果是折线
		Geometry *geometry = new GeoPolyline();
        //获取坐标数据
        QJsonArray coordinates = geometryJson.take(COORDINATES).toArray();
        geometry->coordinates = coordinates;
        feature.geometry = geometry;
    }else if(POLYGON.compare(type)==0){
        //如果是折线
        Geometry *geometry = new GeoPolygon();
        //获取坐标数据
        QJsonArray coordinates = geometryJson.take(COORDINATES).toArray();
        geometry->coordinates = coordinates;
        feature.geometry = geometry;
    }
    return feature;
}
