#pragma once
#include "SpatialIndex.h"
#include "QuadTree.h"
#include "Layer.h"
class QuadTreeIndex :
	public SpatialIndex
{
public:
	QuadTreeIndex();
	virtual ~QuadTreeIndex();
	QuadTree *quadTree;
	void createQuadTree(int initdepth, QRectF bound,Layer* layer);
	virtual SpatialIndexType getIndexType();
	virtual void addAllObjID(Layer *layer);
};

