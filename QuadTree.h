#pragma once
#include "QuadNode.h"
#include "Layer.h"
#include "Feature.h"
#include"geos.h"
#include "MGeosUtil.h"
class QuadTree
{
public:
	QuadTree();
	
	~QuadTree();
	QuadNode *root; //根节点
	int depth;  //树深度
	int maxElemNum; //每个节点的最大目标数
	void initTree(int depth, QRectF bound);
	void addFeatures(Layer *layer);
	Feature* queryByPt(QPointF *pt);
	geos::geom::Geometry *pgeom;
};

