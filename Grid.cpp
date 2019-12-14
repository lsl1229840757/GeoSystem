#include "Grid.h"



Grid::Grid()
{
	ogrBound = NULL;
	geosBound = NULL;
}


Grid::~Grid()
{
	if (ogrBound != NULL)
	{
		delete ogrBound;
		ogrBound = NULL;
	}
	if (geosBound != NULL)
	{
		delete geosBound;
		geosBound = NULL;
	}
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
	this->ogrBound = new OGRPolygon;
	((OGRPolygon*)ogrBound)->addRing(ring);
	this->geosBound = MGeosUtil::OGR2GEOSGeom(this->ogrBound);
}
