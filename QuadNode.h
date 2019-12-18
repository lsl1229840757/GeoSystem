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
	UR = 0, // UR����
	UL = 1, // ULΪ����
	LL = 2, // LLΪ����
	LR = 3  // LRΪ����
}QuadranEnum;

class QuadNode
{
public:
	QuadNode(int depth, QRectF bound);
	~QuadNode();
	vector<QPair<int, int>> featureIDs;  //��¼ͼ��ID����Ҫ��ID
	vector<Feature*> pfeatures;  //��¼�ڵ��ڵ�Ҫ��
	int nChildCount; //�ӽڵ����
	QuadNode *childNodes[4]; //ָ���ӽڵ�
	QRectF box;  //�ڵ㷶Χ
	int depth;   //�ڵ����
	int eleNum;  //λ�õ���
	bool isLeaf();  //�Ƿ�Ҷ�ӽ��
	geos::geom::Geometry *geosBound;  //�߽�תΪGEOS����
	void createQuadBranch();// ������֧
	void insertEle(Feature *fea,int k);
	void splitNode();
	void setNodeBoundary(QRectF bound);
	// //�����ӽڵ�
	QuadNode* createChildNode(QRectF bound);
	const static int MAX_ELEM_NUM;  //ÿ���ڵ����洢Ŀ����
	QuadNode* queryEleByPt(QPointF *pt); 
};

