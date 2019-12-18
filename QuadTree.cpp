#include "QuadTree.h"



QuadTree::QuadTree()
{
	root = NULL;
}


QuadTree::~QuadTree()
{
	if (root != NULL)
	{
		delete root;
		root = NULL;
	}
}


void QuadTree::initTree(int depth, QRectF bound)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	root = new QuadNode(depth, bound);
}




void QuadTree::addFeatures(Layer *layer)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (root != NULL)
	{
		for (int i = 0; i < layer->features.size(); i++)
		{
			root->insertEle(layer->features.at(i));
		}
	}
}


Feature* QuadTree::queryByPt(QPointF *pt)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	QuadNode* node = root->queryEleByPt(pt);
	Feature* fea = NULL;
	//ptתGEOS
	const geos::geom::GeometryFactory *geosFactory = MGeosUtil::getDefaultGeometryFactory();
	pgeom = geosFactory->createPoint(Coordinate(pt->rx(), pt->ry()));
	for (int i = 0; i < node->eleNum; i++)
	{
		if (!node->pfeatures[i]->geosGeom->disjoint(pgeom))
		{
			fea = node->pfeatures[i];
			return fea;
		}

	}
	return nullptr;  //δ�ҵ����ؿ�ָ��
}
