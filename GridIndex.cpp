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
	if (this->isIndexCreated == true ) return false;
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
			grid->setGridBoundary(QRectF(left, top, dx, dy));  //���߽��¼ΪQRectF��OGR��GEOS
			grid->gridID = gridID;
			this->grids.push_back(grid);
		}
	}

	this->isIndexCreated = true;
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
				if (SpatialIndex::isIntersect(fea->geosGeom, grid->geosBound))
				{
					QPair<int, int> lyAndFeaID;
					//���н����ĸ����д洢Ҫ�ص�ͼ��
					lyAndFeaID.first = layer->layerID;
					lyAndFeaID.second = fea->featureID;
					grid->featureIDs.push_back(lyAndFeaID);
					grid->pfeatures.push_back(fea); //����Ŀ���ָ��
					qDebug() << this->grids.at(k)->gridID << "::layer:" << lyAndFeaID.first << " feature:" << lyAndFeaID.second;
				}
			}
		}
	}
}


