#include "QuadNode.h"
const int QuadNode::MAX_ELEM_NUM = 5;

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


void QuadNode::insertEle(Feature *fea,int k)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	if (isLeaf()==true)
	{	//����Ҷ�ڵ�
		if (this->eleNum + 1 > MAX_ELEM_NUM)
		{
			this->splitNode();
			this->insertEle(fea,4);
		}
		else 
		{
			//Ŀ����ӽ��ڵ�
			this->pfeatures.push_back(fea);
			this->eleNum++;
			QString test;
			switch (k)
			{
			case 0:
				test = "UR";
				break;
			case 1:
				test = "UL";
				break;
			case 2:
				test = "LL";
				break;
			case 3:
				test = "LR";
				break;
			default:
				test = "not";
				break;
			}

			qDebug() << "depth:" << this->depth<<" from:"<<test << " feature:" <<  fea->attributes.value("NAME");
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
				this->childNodes[i]->insertEle(fea, i);
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
	this->childNodes[UR] = createChildNode(QRectF(QPointF(midHorizontal, this->box.bottom()),QPointF(this->box.right(),midVertical)));
	this->childNodes[UL] = createChildNode(QRectF(QPointF(this->box.left(), this->box.bottom()), QPointF(midHorizontal, midVertical)));
	this->childNodes[LL] = createChildNode(QRectF(QPointF(this->box.left(), midVertical), QPointF(midHorizontal, this->box.top())));
	this->childNodes[LR] = createChildNode(QRectF(QPointF(midHorizontal, midVertical), QPointF(this->box.right(), this->box.top())));
	nChildCount = 4;
	int nCount = this->eleNum;
	for (int i = 0; i < nCount; i++)
	{
		this->insertEle(pfeatures[i],4);
		this->eleNum--;
	}
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
