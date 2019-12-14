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

GeoMap* JsonUtil::parseGeoJson(QString filePath){
	QJsonObject geoJson = JsonUtil::JsonRead(filePath);
	//从文件名字中获取地图名字
	QString mapName = filePath.mid(filePath.lastIndexOf("/")+1, filePath.lastIndexOf(".")-filePath.lastIndexOf("/")-1);
    //解析type
	GeoMap* geoMap = new GeoMap;
	geoMap->name = mapName.toStdString();
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
	feature->featureID = layer->features.size() - 1;
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
	layer->layerID = geoMap->layers.size() - 1;
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
	layer->layerID = geoMap->layers.size() - 1;
}


QRectF JsonUtil::parseGeometry(QJsonObject geomJObj, Feature* feature)
{
	// TODO: 在此处添加实现代码.
	QString type = geomJObj.take(TYPE).toString();
	mgeo::Geometry* geom = NULL;
	//获取GeometryFactory
	GeometryFactory *geosGeomFactory = MGeosUtil::getPGeometryFactory();
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
			//初始化range
			max_x = ptCoorArray.at(0).toDouble();
			min_x = ptCoorArray.at(0).toDouble();
			max_y = ptCoorArray.at(1).toDouble();
			min_y = ptCoorArray.at(1).toDouble();
			((GeoPoint*)geom)->x = ptCoorArray.at(0).toDouble();  //存储点坐标
			((GeoPoint*)geom)->y = ptCoorArray.at(1).toDouble();
			feature->geosGeom = geosGeomFactory->createPoint(Coordinate(((GeoPoint*)geom)->x, ((GeoPoint*)geom)->y));  //创建GEOS点
		}
		QPointF bottomRight(max_x, min_y), topLeft(min_x, max_y);
		geomRange = QRectF(topLeft, bottomRight);
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
			//构建点序列
			CoordinateArraySequence* cas = new CoordinateArraySequence;
			for (int i = 0; i < lineCoorArray.count(); i++)
			{
				if (lineCoorArray.at(i).isArray())
				{
					GeoPoint* tmpPt = new GeoPoint;
					((GeoPolyline*)geom)->points.push_back(tmpPt);
					tmpPt->coordinates = lineCoorArray.at(i).toArray();

					tmpPt->x = tmpPt->coordinates.at(0).toDouble();  //存储点坐标
					tmpPt->y = tmpPt->coordinates.at(1).toDouble();
					//存储GEOS点序列坐标
					cas->add(Coordinate(tmpPt->x, tmpPt->y));
					//qDebug() << QString("%1").arg(tmpPt->x, 0, 'g', 14) << "," << QString("%1").arg(tmpPt->y, 0, 'g', 14) << endl;
					//比较Range
					if (min_x > tmpPt->x)min_x = tmpPt->x;
					else if (max_x < tmpPt -> x)max_x = tmpPt->x;
					if (min_y > tmpPt->y)min_y = tmpPt->y;
					else if (max_y < tmpPt->y)max_y = tmpPt->y;
				}
			}
			//存储非闭合线
			geos::geom::LineString *geosLString = geosGeomFactory->createLineString(cas);
			feature->geosGeom = geosLString;
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
			//构建点序列
			CoordinateArraySequence* cas = new CoordinateArraySequence;
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
					//存储GEOS点序列坐标
					cas->add(Coordinate(tmpPt->x, tmpPt->y));
					//qDebug() <<  QString("%1").arg(tmpPt->x, 0, 'g', 14) << "," << QString("%1").arg(tmpPt->y, 0, 'g', 14) << endl;
					//比较Range
					if (min_x > tmpPt->x)min_x = tmpPt->x;
					else if (max_x < tmpPt->x)max_x = tmpPt->x;
					if (min_y > tmpPt->y)min_y = tmpPt->y;
					else if (max_y < tmpPt->y)max_y = tmpPt->y;
				}
			}
			//外环构造多边形,并存储
			geos::geom::LinearRing *geosLRing = geosGeomFactory->createLinearRing(cas);
			geos::geom::Polygon *geosPly =geosGeomFactory->createPolygon(geosLRing,NULL);//内环为空
			feature->geosGeom = geosPly;
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
			//构造存储面的向量
			vector<geos::geom::Geometry*> fromPoly;
			for (int i = 0; i < mplyCoorArray.count(); i++)
			{
				plyCoorArray = mplyCoorArray.at(i).toArray();
				GeoPolygon* ply = new GeoPolygon;
				((GeoMultiPolygon*)geom)->polygons.push_back(ply);
				ply->coordinates = plyCoorArray;
				//构建点序列
				CoordinateArraySequence* cas = new CoordinateArraySequence;
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
						//存储GEOS点序列坐标
						cas->add(Coordinate(tmpPt->x, tmpPt->y));
						//比较Range
						if (min_x > tmpPt->x)min_x = tmpPt->x;
						else if (max_x < tmpPt->x)max_x = tmpPt->x;
						if (min_y > tmpPt->y)min_y = tmpPt->y;
						else if (max_y < tmpPt->y)max_y = tmpPt->y;
					}
				}
				//外环构造多边形
				geos::geom::LinearRing *geosLRing = geosGeomFactory->createLinearRing(cas);
				geos::geom::Polygon *geosPly = geosGeomFactory->createPolygon(geosLRing, NULL);//内环为空
				fromPoly.push_back(geosPly);
			}
			//存储GEOS多面
			geos::geom::MultiPolygon *geosMultiPly= geosGeomFactory->createMultiPolygon(fromPoly);
			feature->geosGeom = geosMultiPly;
			QPointF bottomRight(max_x, min_y), topLeft(min_x, max_y);
			geomRange = QRectF(topLeft, bottomRight);
		}
	}
	return geomRange;
}


// 写入Json文件
void JsonUtil::jsonWrite(QString destPath, QJsonObject* jsonObj)
{
	// TODO: 在此处添加实现代码.
	QFile writeFile(destPath);
	if (!writeFile.open(QIODevice::ReadWrite))
	{
		qDebug()<<"could't open projects json"<<endl;
		//QMessageBox::warning(NULL, QString("Error"), QString(r1.what()));
	}
	
	QJsonDocument jsonDoc(*jsonObj);  //转为JsonDocument
	writeFile.resize(0);  //清空文件原有的内容
	writeFile.write(jsonDoc.toJson());  //写入json
	writeFile.close();
}


QJsonObject JsonUtil::storeDbParams(QString tableName, QString dbname, QString addr, QString port, QString username, QString password)
{
	// TODO: 在此处添加实现代码.
	QJsonObject jsonObj;
	jsonObj.insert("table_name", tableName);
	jsonObj.insert("database_name", dbname);
	jsonObj.insert("address", addr);
	jsonObj.insert("port", port);
	jsonObj.insert("username", username);
	jsonObj.insert("password", password);
	return jsonObj;
}


void JsonUtil::loadDbParams(QJsonObject* pJsonObj, QString& tableName, QString& dbname, QString& addr, QString& port, QString& username, QString& password)
{
	// TODO: 在此处添加实现代码.
	if (pJsonObj != NULL)
	{
		tableName = pJsonObj->value("table_name").toString();
		dbname = pJsonObj->value("database_name").toString();
		addr = pJsonObj->value("address").toString();
		port = pJsonObj->value("port").toString();
		username = pJsonObj->value("username").toString();
		password = pJsonObj->value("password").toString();
	}
}
