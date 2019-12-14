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
	this->ogrBound = new OGRPolygon;
	((OGRPolygon*)ogrBound)->addRing(ring);
	this->geosBound = MGeosUtil::OGR2GEOSGeom(this->ogrBound);
}
