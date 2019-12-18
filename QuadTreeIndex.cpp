#include "QuadTreeIndex.h"



QuadTreeIndex::QuadTreeIndex()
{
	quadTree = NULL;
}


QuadTreeIndex::~QuadTreeIndex()
{
}

//��ʼ��ȣ���ʼ�߽�,ͼ��
void QuadTreeIndex::createQuadTree(int initdepth, QRectF bound,Layer* layer)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	quadTree = new QuadTree;
	quadTree->initTree(initdepth,bound);
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
