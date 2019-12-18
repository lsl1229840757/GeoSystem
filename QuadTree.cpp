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
	// TODO: 在此处添加实现代码.
	root = new QuadNode(depth, bound);
}




void QuadTree::addFeatures(Layer *layer)
{
	// TODO: 在此处添加实现代码.
	if (root != NULL)
	{
		for (int i = 0; i < layer->features.size(); i++)
		{
			root->insertEle(layer->features.at(i));
		}
	}
}
