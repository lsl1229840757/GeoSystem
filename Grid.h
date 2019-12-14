#pragma once
#include <vector>
#include "Feature.h"
#include "qrect.h"
#include "ogrsf_frmts.h"
#include "qpair.h"
#include "MGeosUtil.h"
//ע��Grid�а���GdalUtil��ѭ�����á�

//�洢ÿ���������
using namespace std;
class Grid
{
public:
	Grid();
	~Grid();
	vector<QPair<int,int>> featureIDs;  //��¼ͼ��ID����Ҫ��ID
	vector<Feature*> pfeatures;  //��¼�����ڵ�Ҫ��
	QRectF gridBoundary;  //���ӱ߽�
	int gridID;
	OGRGeometry *ogrBound;  //���ӱ߽�תΪOGR����
	geos::geom::Geometry* geosBound;  //��GEOS�жϽ������洢�����ӿ��ٶ�
	void setGridBoundary(QRectF bound);
	
};

