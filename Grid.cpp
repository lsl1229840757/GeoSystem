#include "Grid.h"



Grid::Grid()
{
	ogrBound = NULL;
}


Grid::~Grid()
{
}


void Grid::setGridBoundary(QRectF bound)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	this->gridBoundary = bound;
	//ͬʱ����ΪOGR����
	OGRLinearRing* ring=new OGRLinearRing;
	//��ӵ�����
	ring->addPointM(bound.left(), bound.top(), 0);
	ring->addPointM(bound.right(), bound.top(), 0);
	ring->addPointM(bound.right(), bound.bottom(), 0);
	ring->addPointM(bound.left(), bound.bottom(), 0);
	ring->addPointM(bound.left(), bound.top(), 0);
	ogrBound = new OGRPolygon;
	((OGRPolygon*)ogrBound)->addRing(ring);
}
