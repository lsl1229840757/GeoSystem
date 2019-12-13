#include "GridIndex.h"



GridIndex::GridIndex()
{
	
}

GridIndex::GridIndex(QRectF mapRange)
{
	this->mapRange = mapRange;
	
}

GridIndex::~GridIndex()
{
}


bool GridIndex::createIndex()
{
	// TODO: 在此处添加实现代码.
	if (isIndexCreated == true ) return false;
	int gridID;
	//QRectF gridBoundary;  //格子边界坐标
	//每一格子的长宽
	double dx = fabs(mapRange.width()) / colNum;
	double dy = fabs(mapRange.height()) / rowNum;
	Grid *grid = NULL;
	//构建格网
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			grid = new Grid;
			gridID = j + i *colNum;
			//计算格网边界坐标
			double left = mapRange.left() + dx * j;
			double top = mapRange.top() + dy * i;
			//gridBoundary.right = mapRange.right() + dx * (j + 1);
			//gridBoundary.bottom = mapRange.bottom() + dy * (i + 1);
			grid->setGridBoundary(QRectF(left, top, dx, dy));
			grid->gridID = gridID;
			this->grids.push_back(grid);
		}
	}

	isIndexCreated = true;
	return true;
}





// //获取索引类型
SpatialIndexType GridIndex::getIndexType()
{
	// TODO: 在此处添加实现代码.
	return SpatialIndexType::GRID;
}


void GridIndex::setColRow(int col, int row)
{
	// TODO: 在此处添加实现代码.
	colNum = col;
	rowNum = row;//
}


// 添加索引目标ID
void GridIndex::addAllObjID(Layer *layer)
{
	if (isIndexCreated) {
		// TODO: 在此处添加实现代码.
		for (int j = 0; j < layer->features.size(); j++)
		{
			Feature *fea = layer->features.at(j);
			for (int k = 0; k < this->grids.size(); k++)
			{
				Grid *grid = this->grids.at(k);
				if (isIntersect(fea->ogrGrom, grid->ogrBound))
				{
					QPair<int, int> lyAndFeaID;
					//在有交集的格网中存储要素的图层
					lyAndFeaID.first = layer->layerID;
					lyAndFeaID.second = fea->featureID;
					grid->featureIDs.push_back(lyAndFeaID);
					qDebug() << this->grids.at(k)->gridID << "::layer:" << lyAndFeaID.first << " feature:" << lyAndFeaID.second;
				}
			}
		}
	}
}

//判断相交,需要GEOS库支持
bool GridIndex::isIntersect(OGRGeometry *ogrGeom, OGRGeometry *ogrGridBound)
{
	// TODO: 在此处添加实现代码.
	//wkt的方式转为GEOS对象，也可用wkb 。避免重新编译Gdal
	if(ogrGeom==NULL || ogrGridBound==NULL)
	{ 
		return false;
	}
	int featureType = ogrGeom->getGeometryType();
	OGRGeometry *intersection = ogrGeom->Intersection(ogrGridBound);  //判断是否相交
	if (intersection != NULL)
	{
		return true;
	}
	else {
		return false;
	}
}
