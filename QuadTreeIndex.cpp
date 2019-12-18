#include "QuadTreeIndex.h"



QuadTreeIndex::QuadTreeIndex()
{
	quadTree = NULL;
}


QuadTreeIndex::~QuadTreeIndex()
{
}

//初始深度，初始边界,图层
void QuadTreeIndex::createQuadTree(int initdepth, QRectF bound,Layer* layer)
{
	// TODO: 在此处添加实现代码.
	quadTree = new QuadTree;
	quadTree->initTree(initdepth,bound);
}


SpatialIndexType QuadTreeIndex::getIndexType()
{
	// TODO: 在此处添加实现代码.
	return SpatialIndexType::QUADTREE;
}


void QuadTreeIndex::addAllObjID(Layer *layer)
{
	// TODO: 在此处添加实现代码.
	quadTree->addFeatures(layer);
}
