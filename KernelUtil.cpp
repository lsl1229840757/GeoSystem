#include "KernelUtil.h"



KernelUtil::KernelUtil()
{

}


KernelUtil::~KernelUtil()
{

}

pair<double, double> KernelUtil::getCdByNum(double left, double bottom, double cellSize, int wNum, int hNum)
{
	double x = left + ((double)wNum - 0.5) * cellSize;
	double y = bottom + ((double)hNum - 0.5) * cellSize;
	return pair<double, double>(x, y);
}

