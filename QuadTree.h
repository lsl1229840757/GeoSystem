#pragma once
#include "QuadNode.h"
#include "Layer.h"
class QuadTree
{
public:
	QuadTree();
	~QuadTree();
	QuadNode *root; //���ڵ�
	int depth;  //�����
	int maxElemNum; //ÿ���ڵ�����Ŀ����
	
	void createQuadBranch();// ������֧
	void insertEle(Feature *fea);
	void splitNode();

	void initTree(int depth, QRectF bound);
	void addFeatures(Layer *layer);
};

