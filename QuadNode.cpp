#include "QuadNode.h"
const int QuadNode::MAX_ELEM_NUM = 30;

QuadNode::QuadNode(int depth,QRectF bound)
{
	//初始化node
	for (int i = 0; i < 4; i++)
	{
		childNodes[i] = NULL;
	}
	nChildCount = 0;
	eleNum = 0;
	this->depth = depth;
	setNodeBoundary(bound);
}


QuadNode::~QuadNode()
{
}


bool QuadNode::isLeaf()
{
	for (int i = 0; i < 4; i++)
	{
		if (this->childNodes[i] != NULL)
		{
			return false;
		}
	}
	return true;
}

// 创建分支
void QuadNode::createQuadBranch()
{
	// TODO: 在此处添加实现代码.
}


void QuadNode::insertEle(Feature *fea)
{
	// TODO: 在此处添加实现代码.
	if (isLeaf()==true)
	{	//对于叶节点
		if (this->eleNum+1 > MAX_ELEM_NUM)
		{
			//qDebug() << "*********分割前********";
			this->splitNode();
			this->insertEle(fea);
		}
		else 
		{
			//目标添加进节点
			this->pfeatures.push_back(fea);
			this->eleNum++;
			QString test;
		}
		return;
	}
	else 
	{
		//对于非叶子节点
		for (int i = 0; i < 4; i++)
		{
			if (!fea->geosGeom->disjoint(this->childNodes[i]->geosBound))
			{
				this->childNodes[i]->insertEle(fea);
			}
		}
	}
}


void QuadNode::splitNode()
{
	// TODO: 在此处添加实现代码.
	double midVertical = (this->box.top() + this->box.bottom()) / 2;
	double midHorizontal = (this->box.left() + this->box.right()) / 2;
	//添加子节点 
	this->childNodes[UR] = createChildNode(QRectF(QPointF(midHorizontal, this->box.top()),QPointF(this->box.right(),midVertical)));
	this->childNodes[UL] = createChildNode(QRectF(QPointF(this->box.left(), this->box.top()), QPointF(midHorizontal, midVertical)));
	this->childNodes[LL] = createChildNode(QRectF(QPointF(this->box.left(), midVertical), QPointF(midHorizontal, this->box.bottom())));
	this->childNodes[LR] = createChildNode(QRectF(QPointF(midHorizontal, midVertical), QPointF(this->box.right(), this->box.bottom())));
	nChildCount = 4;
	int nCount = this->eleNum;
	for (int i = 0; i < nCount; i++)
	{
		this->insertEle(pfeatures[i]);
		this->eleNum--;
	}
	//qDebug() << "-----------------------";
	pfeatures.clear();  //清除非叶子结点所有目标
}


void QuadNode::setNodeBoundary(QRectF bound)
{
	// TODO: 在此处添加实现代码.
	this->box = bound;
	//同时保存为GEOS对象
	OGRLinearRing* ring = new OGRLinearRing;
	//添加点坐标
	ring->addPointM(bound.left(), bound.top(), 0);
	ring->addPointM(bound.right(), bound.top(), 0);
	ring->addPointM(bound.right(), bound.bottom(), 0);
	ring->addPointM(bound.left(), bound.bottom(), 0);
	ring->addPointM(bound.left(), bound.top(), 0);
	OGRPolygon *ogrBound = new OGRPolygon;
	((OGRPolygon*)ogrBound)->addRing(ring);
	this->geosBound = MGeosUtil::OGR2GEOSGeom(ogrBound);
}


// //生成子节点
QuadNode* QuadNode::createChildNode(QRectF bound)
{
	// TODO: 在此处添加实现代码.
	int childDepth = this->depth + 1;  //.
	QuadNode *childNode = new QuadNode(childDepth, bound);
	return childNode;
}

//根据点查找node
QuadNode* QuadNode::queryEleByPt(QPointF *pt)
{
	// TODO: 在此处添加实现代码.
	if (this->isLeaf() == true)
	{
		if (this->box.contains(*pt)) 
		{
			//若节点包含改点
			return this;
		}
	}
	else
	{
		//对于非叶子节点
		for (int i = 0; i < 4; i++)
		{
			if (this->childNodes[i]->box.contains(*pt))
			{
				this->childNodes[i]->queryEleByPt(pt);
				break;  //只需要找到一个
			}
		}
	}
	return nullptr;
}
