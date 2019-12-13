#pragma once
#include <vector>
#include "Feature.h"
#include "qrect.h"
#include "ogrsf_frmts.h"
#include "qpair.h"
//存储每个格子情况
using namespace std;
class Grid
{
public:
	Grid();
	~Grid();
	vector<QPair<int,int>> featureIDs;  //记录图层ID及其要素ID
	vector<Feature*> pfeatures;  //记录格网内的要素
	QRectF gridBoundary;  //格子边界
	int gridID;
	OGRGeometry *ogrBound;  //格子边界转为OGR对象
	void setGridBoundary(QRectF bound);

};

