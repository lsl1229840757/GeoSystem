#pragma once
#include "QuadNode.h"
#include "Layer.h"
class QuadTree
{
public:
	QuadTree();
	~QuadTree();
	QuadNode *root; //根节点
	int depth;  //树深度
	int maxElemNum; //每个节点的最大目标数
	
	void createQuadBranch();// 创建分支
	void insertEle(Feature *fea);
	void splitNode();

	void initTree(int depth, QRectF bound);
	void addFeatures(Layer *layer);
};

