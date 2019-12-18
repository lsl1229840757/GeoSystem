#include "QuadTreeIndex.h"



QuadTreeIndex::QuadTreeIndex(int initdepth, QRectF bound)
{
	quadTree = NULL;
	this->boundary = bound;
	this->initdepth = initdepth;
}


QuadTreeIndex::~QuadTreeIndex()
{
}

//初始深度，初始边界,图层
bool QuadTreeIndex::createIndex()
{
	// TODO: 在此处添加实现代码.
	if (this->isIndexCreated == true) return false;
	quadTree = new QuadTree;
	quadTree->initTree(initdepth,boundary);
	this->isIndexCreated = true;
	return true;
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


// //通过原始点查找要素
Feature* QuadTreeIndex::queryFeaByPt(QPointF *pt)
{
	// TODO: 在此处添加实现代码.
	Feature* fea = quadTree->queryByPt(pt);
	return fea;
}
