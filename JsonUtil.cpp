#include "JsonUtil.h"
//初始化静态变量
QString JsonUtil::FEATURECOLLECTION = "FeatureCollection";
QString JsonUtil::GEOMETRYCOLLECTION = "GeometryCollection";
QString JsonUtil::FEATURES = "features";
QString JsonUtil::TYPE = "type";
QString JsonUtil::GEOMETRY = "geometry";
QString JsonUtil::PROPERTIES = "properties";
QString JsonUtil::FEATURE = "Feature";
QString JsonUtil::POINT = "Point";
QString JsonUtil::POLYLINE = "LineString";
QString JsonUtil::POLYGON = "Polygon";
QString JsonUtil::COORDINATES = "coordinates";
QString JsonUtil::MULTIPOLYGON = "MultiPolygon";
QString JsonUtil::GEOMETRYS = "geometries";

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
		//QMessageBox::warning(NULL, QString("Error"), QString(r1.what()));
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

GeoMap* JsonUtil::parseGeoJson(QJsonObject geoJson){
    //解析type
	GeoMap* geoMap = new GeoMap;
    QString type = geoJson.take(TYPE).toString();
    if(type.compare(FEATURECOLLECTION)==0)
	{
        parseFeatureCollection(geoJson,geoMap);  //解析要素集
    }
	else if(type.compare(GEOMETRYCOLLECTION)==0)
	{
		parseGeometryCollection(geoJson, geoMap);  //解析几何集
	}
    return geoMap;

}
//TODO
QRectF JsonUtil::parseFeature(QJsonObject feaJObj ,Layer* layer) throw(runtime_error){
    Feature* feature = new Feature;
	layer->features.push_back(feature);
    //解析属性
    QJsonObject properties = feaJObj.take(PROPERTIES).toObject();
    feature->properties = properties;
    //获取geomery
    QJsonObject geomJObj = feaJObj.take(GEOMETRY).toObject();
    //开始解析geometry
	QRectF maxRange=parseGeometry(geomJObj, feature);
	return maxRange;
    
}


void JsonUtil::parseFeatureCollection(QJsonObject geoJson, GeoMap* geoMap)
{
	// TODO: 在此处添加实现代码.
	Layer* layer = new Layer;
	QRectF maxRange;
	if (geoJson.value(FEATURES).isArray())
	{
		QJsonArray featureArray = geoJson.take(FEATURES).toArray();
		for (int i = 0; i < featureArray.count(); i++)
		{
			QJsonValue feature = featureArray.at(i);  //对于每一个feature
			QJsonObject feaJObj = feature.toObject();
			maxRange = maxRange.united(parseFeature(feaJObj, layer));
		}
	}

	layer->range = maxRange;
	geoMap->addLayer(layer);
}


void JsonUtil::parseGeometryCollection(QJsonObject geoJson, GeoMap* geoMap)
{
	// TODO: 在此处添加实现代码.
	Layer* layer = new Layer;
	
	QRectF maxRange;
	if (geoJson.value(GEOMETRYS).isArray())
	{
		QJsonArray geomArray = geoJson.take(GEOMETRYS).toArray();
		for (int i = 0; i < geomArray.count(); i++)
		{
			Feature* feature = new Feature;
			layer->features.push_back(feature);
			QJsonValue geom = geomArray.at(i);  //对于每一个geometry
			QJsonObject geomJObj = geom.toObject();
			maxRange=maxRange.united(parseGeometry(geomJObj, feature));
		}
	}
	layer->range = maxRange;
	geoMap->addLayer(layer);
}


QRectF JsonUtil::parseGeometry(QJsonObject geomJObj, Feature* feature)
{
	// TODO: 在此处添加实现代码.
	QString type = geomJObj.take(TYPE).toString();
	Geometry* geom = NULL;

	//记录边界
	QRectF geomRange;
	
	double max_x = 0;
	double min_x = 0; 
	double max_y = 0;
	double min_y = 0; 
	if (type.compare(POINT) == 0)
	{
		geom = new GeoPoint;
		feature->geometry = geom;
		if (geomJObj.value(COORDINATES).isArray())
		{
			geom->coordinates = geomJObj.value(COORDINATES).toArray();
			QJsonArray ptCoorArray = geom->coordinates;
			((GeoPoint*)geom)->x = ptCoorArray.at(0).toDouble();  //存储点坐标
			((GeoPoint*)geom)->y = ptCoorArray.at(1).toDouble();
			
		}
	}
	else if(type.compare(POLYLINE) == 0)
	{
		geom = new GeoPolyline;
		feature->geometry = geom;
		if (geomJObj.value(COORDINATES).isArray())
		{
			geom->coordinates = geomJObj.value(COORDINATES).toArray();
			QJsonArray lineCoorArray = geom->coordinates;
			//初始化range
			max_x = lineCoorArray.at(0).toArray().at(0).toDouble();
			min_x = lineCoorArray.at(0).toArray().at(0).toDouble();
			max_y = lineCoorArray.at(0).toArray().at(1).toDouble();
			min_y = lineCoorArray.at(0).toArray().at(1).toDouble();
			for (int i = 0; i < lineCoorArray.count(); i++)
			{
				if (lineCoorArray.at(i).isArray())
				{
					GeoPoint* tmpPt = new GeoPoint;
					((GeoPolyline*)geom)->points.push_back(tmpPt);
					tmpPt->coordinates = lineCoorArray.at(i).toArray();

					tmpPt->x = tmpPt->coordinates.at(0).toDouble();  //存储点坐标
					tmpPt->y = tmpPt->coordinates.at(1).toDouble();
					//qDebug() << QString("%1").arg(tmpPt->x, 0, 'g', 14) << "," << QString("%1").arg(tmpPt->y, 0, 'g', 14) << endl;
					//比较Range
					if (min_x > tmpPt->x)min_x = tmpPt->x;
					else if (max_x < tmpPt -> x)max_x = tmpPt->x;
					if (min_y > tmpPt->y)min_y = tmpPt->y;
					else if (max_y < tmpPt->y)max_y = tmpPt->y;
				}
			}
			QPointF bottomRight(max_x, min_y), topLeft(min_x, max_y);
			geomRange = QRectF(topLeft, bottomRight);
			//QPointF bottomLeft(max_x, max_y), topRight(min_x, min_y);
			//geomRange = QRectF(topRight, bottomLeft);
		}
	}
	else if (type.compare(POLYGON) == 0)
	{
		//
		geom = new GeoPolygon;
		feature->geometry = geom;
		if (geomJObj.value(COORDINATES).isArray())
		{
			geom->coordinates = geomJObj.value(COORDINATES).toArray();
			QJsonArray plyCoorArray = geom->coordinates;
			QJsonArray exLineCoorArray;
			//初始化range
			max_x = plyCoorArray.at(0).toArray().at(0).toArray().at(0).toDouble();
			min_x = plyCoorArray.at(0).toArray().at(0).toArray().at(0).toDouble();
			max_y = plyCoorArray.at(0).toArray().at(0).toArray().at(1).toDouble();
			min_y = plyCoorArray.at(0).toArray().at(0).toArray().at(1).toDouble();
			for (int i = 0; i < plyCoorArray.count(); i++)
			{
				if (i != 0)continue;  //先只考虑简单多边形外环
				exLineCoorArray = plyCoorArray.at(i).toArray();
				for (int j = 0; j < exLineCoorArray.count(); j++)
				{
					GeoPoint* tmpPt = new GeoPoint;
					((GeoPolygon*)geom)->points.push_back(tmpPt);
					tmpPt->coordinates = exLineCoorArray.at(j).toArray();
					tmpPt->x = tmpPt->coordinates.at(0).toDouble();  //存储点坐标
					tmpPt->y = tmpPt->coordinates.at(1).toDouble();
					//qDebug() <<  QString("%1").arg(tmpPt->x, 0, 'g', 14) << "," << QString("%1").arg(tmpPt->y, 0, 'g', 14) << endl;
					//比较Range
					if (min_x > tmpPt->x)min_x = tmpPt->x;
					else if (max_x < tmpPt->x)max_x = tmpPt->x;
					if (min_y > tmpPt->y)min_y = tmpPt->y;
					else if (max_y < tmpPt->y)max_y = tmpPt->y;
				}
			}
			QPointF bottomRight(max_x, min_y), topLeft(min_x, max_y);
			geomRange = QRectF(topLeft, bottomRight);
			//QPointF bottomLeft(max_x, max_y), topRight(min_x, min_y);
			//geomRange = QRectF(topRight, bottomLeft);
		}
	}
	else if (type.compare(MULTIPOLYGON) == 0)
	{
		geom = new GeoMultiPolygon;
		feature->geometry = geom;
		if (geomJObj.value(COORDINATES).isArray())
		{
			geom->coordinates = geomJObj.value(COORDINATES).toArray();
			QJsonArray mplyCoorArray = geom->coordinates;
			QJsonArray plyCoorArray, exLineCoorArray;
			//初始化range
			max_x = mplyCoorArray.at(0).toArray().at(0).toArray().at(0).toArray().at(0).toDouble();
			min_x = mplyCoorArray.at(0).toArray().at(0).toArray().at(0).toArray().at(0).toDouble();
			max_y = mplyCoorArray.at(0).toArray().at(0).toArray().at(0).toArray().at(1).toDouble();
			min_y = mplyCoorArray.at(0).toArray().at(0).toArray().at(0).toArray().at(1).toDouble();
			for (int i = 0; i < mplyCoorArray.count(); i++)
			{
				plyCoorArray = mplyCoorArray.at(i).toArray();
				GeoPolygon* ply = new GeoPolygon;
				((GeoMultiPolygon*)geom)->polygons.push_back(ply);
				ply->coordinates = plyCoorArray;
				for (int j = 0; j < plyCoorArray.count(); j++)
				{
					if (j != 0)continue;  //先只考虑简单多边形外环
					exLineCoorArray = plyCoorArray.at(j).toArray();
					for (int k = 0; k < exLineCoorArray.count(); k++)
					{
						GeoPoint* tmpPt = new GeoPoint;
						ply->points.push_back(tmpPt);
						tmpPt->coordinates = exLineCoorArray.at(k).toArray();
						tmpPt->x = tmpPt->coordinates.at(0).toDouble();  //存储点坐标
						tmpPt->y = tmpPt->coordinates.at(1).toDouble();

						//比较Range
						if (min_x > tmpPt->x)min_x = tmpPt->x;
						else if (max_x < tmpPt->x)max_x = tmpPt->x;
						if (min_y > tmpPt->y)min_y = tmpPt->y;
						else if (max_y < tmpPt->y)max_y = tmpPt->y;
					}
				}
			}
			QPointF bottomRight(max_x, min_y), topLeft(min_x, max_y);
			geomRange = QRectF(topLeft, bottomRight);
		}
	}
	return geomRange;
}
