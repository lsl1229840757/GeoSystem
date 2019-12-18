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
