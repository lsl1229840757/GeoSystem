#pragma once
#include "SpatialIndex.h"
#include "GeoMap.h"
#include "qpair.h"
using namespace std;
class GridIndex :
	public SpatialIndex
{
public:
	GridIndex();
	GridIndex(QRectF mapRange);
	virtual ~GridIndex();
	vector<Grid*> grids;
	virtual bool createIndex();
	int colNum, rowNum;
	// //��ȡ��������
	virtual SpatialIndexType getIndexType();
	void setColRow(int col, int row);
	// �������Ŀ��ID
	virtual void addAllObjID(Layer *layer);
private:
	bool isIntersect(OGRGeometry *fea, OGRGeometry *gridBound);
};

