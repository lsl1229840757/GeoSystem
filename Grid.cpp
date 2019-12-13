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
	// TODO: 在此处添加实现代码.
	this->gridBoundary = bound;
	//同时保存为OGR对象
	OGRLinearRing* ring=new OGRLinearRing;
	//添加点坐标
	ring->addPointM(bound.left(), bound.top(), 0);
	ring->addPointM(bound.right(), bound.top(), 0);
	ring->addPointM(bound.right(), bound.bottom(), 0);
	ring->addPointM(bound.left(), bound.bottom(), 0);
	ring->addPointM(bound.left(), bound.top(), 0);
	ogrBound = new OGRPolygon;
	((OGRPolygon*)ogrBound)->addRing(ring);
}
