#pragma once
#include <vector>
#include "qpair.h"
#include "Feature.h"
#include "qrect.h"
#include "geos.h"
#include"MGeosUtil.h"
#include "qdebug.h"
using namespace std;
typedef enum {
	UR = 0, // UR右上
	UL = 1, // UL为左上
	LL = 2, // LL为左下
	LR = 3  // LR为右下
}QuadranEnum;

class QuadNode
{
public:
	QuadNode(int depth, QRectF bound);
	~QuadNode();
	vector<QPair<int, int>> featureIDs;  //记录图层ID及其要素ID
	vector<Feature*> pfeatures;  //记录节点内的要素
	int nChildCount; //子节点个数
	QuadNode *childNodes[4]; //指向子节点
	QRectF box;  //节点范围
	int depth;   //节点深度
	int eleNum;  //位置点数
	bool isLeaf();  //是否叶子结点
	geos::geom::Geometry *geosBound;  //边界转为GEOS对象
	void createQuadBranch();// 创建分支
	void insertEle(Feature *fea,int k);
	void splitNode();
	void setNodeBoundary(QRectF bound);
	// //生成子节点
	QuadNode* createChildNode(QRectF bound);
	const static int MAX_ELEM_NUM;  //每个节点最大存储目标数
	QuadNode* queryEleByPt(QPointF *pt); 
};

