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

//��ʼ��ȣ���ʼ�߽�,ͼ��
bool QuadTreeIndex::createIndex()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (this->isIndexCreated == true) return false;
	quadTree = new QuadTree;
	quadTree->initTree(initdepth,boundary);
	this->isIndexCreated = true;
	return true;
}


SpatialIndexType QuadTreeIndex::getIndexType()
{
	// TODO: �ڴ˴����ʵ�ִ���.
	return SpatialIndexType::QUADTREE;
}


void QuadTreeIndex::addAllObjID(Layer *layer)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	quadTree->addFeatures(layer);
}


// //ͨ��ԭʼ�����Ҫ��
Feature* QuadTreeIndex::queryFeaByPt(QPointF *pt)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	Feature* fea = quadTree->queryByPt(pt);
	return fea;
}
