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
	// TODO: �ڴ˴����ʵ�ִ���.
	if (isIndexCreated == true ) return false;
	int gridID;
	//QRectF gridBoundary;  //���ӱ߽�����
	//ÿһ���ӵĳ���
	double dx = fabs(mapRange.width()) / colNum;
	double dy = fabs(mapRange.height()) / rowNum;
	Grid *grid = NULL;
	//��������
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			grid = new Grid;
			gridID = j + i *colNum;
			//��������߽�����
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





// //��ȡ��������
SpatialIndexType GridIndex::getIndexType()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	return SpatialIndexType::GRID;
}


void GridIndex::setColRow(int col, int row)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	colNum = col;
	rowNum = row;//
}


// �������Ŀ��ID
void GridIndex::addAllObjID(Layer *layer)
{
	if (isIndexCreated) {
		// TODO: �ڴ˴����ʵ�ִ���.
		for (int j = 0; j < layer->features.size(); j++)
		{
			Feature *fea = layer->features.at(j);
			for (int k = 0; k < this->grids.size(); k++)
			{
				Grid *grid = this->grids.at(k);
				if (isIntersect(fea->ogrGrom, grid->ogrBound))
				{
					QPair<int, int> lyAndFeaID;
					//���н����ĸ����д洢Ҫ�ص�ͼ��
					lyAndFeaID.first = layer->layerID;
					lyAndFeaID.second = fea->featureID;
					grid->featureIDs.push_back(lyAndFeaID);
					qDebug() << this->grids.at(k)->gridID << "::layer:" << lyAndFeaID.first << " feature:" << lyAndFeaID.second;
				}
			}
		}
	}
}

//�ж��ཻ,��ҪGEOS��֧��
bool GridIndex::isIntersect(OGRGeometry *ogrGeom, OGRGeometry *ogrGridBound)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	//wkt�ķ�ʽתΪGEOS����Ҳ����wkb ���������±���Gdal
	if(ogrGeom==NULL || ogrGridBound==NULL)
	{ 
		return false;
	}
	int featureType = ogrGeom->getGeometryType();
	OGRGeometry *intersection = ogrGeom->Intersection(ogrGridBound);  //�ж��Ƿ��ཻ
	if (intersection != NULL)
	{
		return true;
	}
	else {
		return false;
	}
}
