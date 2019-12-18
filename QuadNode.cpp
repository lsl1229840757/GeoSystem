#include "QuadNode.h"
const int QuadNode::MAX_ELEM_NUM = 30;

QuadNode::QuadNode(int depth,QRectF bound)
{
	//��ʼ��node
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

// ������֧
void QuadNode::createQuadBranch()
{
	// TODO: �ڴ˴����ʵ�ִ���.
}


void QuadNode::insertEle(Feature *fea)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (isLeaf()==true)
	{	//����Ҷ�ڵ�
		if (this->eleNum+1 > MAX_ELEM_NUM)
		{
			//qDebug() << "*********�ָ�ǰ********";
			this->splitNode();
			this->insertEle(fea);
		}
		else 
		{
			//Ŀ����ӽ��ڵ�
			this->pfeatures.push_back(fea);
			this->eleNum++;
			QString test;
		}
		return;
	}
	else 
	{
		//���ڷ�Ҷ�ӽڵ�
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
	// TODO: �ڴ˴����ʵ�ִ���.
	double midVertical = (this->box.top() + this->box.bottom()) / 2;
	double midHorizontal = (this->box.left() + this->box.right()) / 2;
	//����ӽڵ� 
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
	pfeatures.clear();  //�����Ҷ�ӽ������Ŀ��
}


void QuadNode::setNodeBoundary(QRectF bound)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	this->box = bound;
	//ͬʱ����ΪGEOS����
	OGRLinearRing* ring = new OGRLinearRing;
	//��ӵ�����
	ring->addPointM(bound.left(), bound.top(), 0);
	ring->addPointM(bound.right(), bound.top(), 0);
	ring->addPointM(bound.right(), bound.bottom(), 0);
	ring->addPointM(bound.left(), bound.bottom(), 0);
	ring->addPointM(bound.left(), bound.top(), 0);
	OGRPolygon *ogrBound = new OGRPolygon;
	((OGRPolygon*)ogrBound)->addRing(ring);
	this->geosBound = MGeosUtil::OGR2GEOSGeom(ogrBound);
}


// //�����ӽڵ�
QuadNode* QuadNode::createChildNode(QRectF bound)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	int childDepth = this->depth + 1;  //.
	QuadNode *childNode = new QuadNode(childDepth, bound);
	return childNode;
}

//���ݵ����node
QuadNode* QuadNode::queryEleByPt(QPointF *pt)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (this->isLeaf() == true)
	{
		if (this->box.contains(*pt)) 
		{
			//���ڵ�����ĵ�
			return this;
		}
	}
	else
	{
		//���ڷ�Ҷ�ӽڵ�
		for (int i = 0; i < 4; i++)
		{
			if (this->childNodes[i]->box.contains(*pt))
			{
				this->childNodes[i]->queryEleByPt(pt);
				break;  //ֻ��Ҫ�ҵ�һ��
			}
		}
	}
	return nullptr;
}
