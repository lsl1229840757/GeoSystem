#pragma once
#include <vector>
#include "GeoPoint.h"
#include "DistanceUtil.h"
#include <qrect.h>
#include <utility>
#define _USE_MATH_DEFINES 
#include <math.h>
using namespace std;
class KernelUtil
{
public:
	KernelUtil();
	~KernelUtil();
	//��Ҫ�ط�Χ, ��Ҫ�ؼ��ϣ�population�ֶΣ� cell��С�� �����뾶�� ��������
	virtual vector<vector<double>> computeKernelUsingPoint(QRectF extent, vector<GeoPoint*> points, vector<double> population, double cellSize, double searchRadius, DistanceUtil* method) = 0;//ʹ�õ�Ҫ�ؼ����
	//�����bottom���߼���y����Сֵ
	static pair<double, double> getCdByNum(double left, double bottom, double cellSize, int wNum, int hNum);
};