#pragma once
#include "SpatialIndex.h"
#include "QuadTree.h"
#include "Layer.h"

class QuadTreeIndex :
	public SpatialIndex
{
public:
	QuadTreeIndex(int initdepth, QRectF bound);
	virtual ~QuadTreeIndex();
	QuadTree *quadTree;
	bool createIndex();
	virtual SpatialIndexType getIndexType();
	virtual void addAllObjID(Layer *layer);
	QRectF boundary;
	int initdepth;
	Layer* layer;
	// //通过点查找要素
	Feature* queryFeaByPt(QPointF *pt);
};

