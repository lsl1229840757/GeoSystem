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
	QuadNode *root; //���ڵ�
	int depth;  //�����
	int maxElemNum; //ÿ���ڵ�����Ŀ����
	void initTree(int depth, QRectF bound);
	void addFeatures(Layer *layer);
	Feature* queryByPt(QPointF *pt);
	geos::geom::Geometry *pgeom;
};

