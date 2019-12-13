#pragma once
#include <vector>
#include "Feature.h"
#include "qrect.h"
#include "ogrsf_frmts.h"
#include "qpair.h"
//�洢ÿ���������
using namespace std;
class Grid
{
public:
	Grid();
	~Grid();
	vector<QPair<int,int>> featureIDs;  //��¼ͼ��ID����Ҫ��ID
	QRectF gridBoundary;  //���ӱ߽�
	int gridID;
	OGRGeometry *ogrBound;  //���ӱ߽�תΪOGR����
	void setGridBoundary(QRectF bound);
};

