#include "EsriKernelUtil.h"



EsriKernelUtil::EsriKernelUtil()
{
}


EsriKernelUtil::~EsriKernelUtil()
{

}

vector<vector<double>> EsriKernelUtil::computeKernelUsingPoint(QRectF extent, vector<GeoPoint*> points, vector<double> population, double cellSize, double searchRadius, DistanceUtil* method)
{
	vector<vector<double>> kernelResult;
	//首先计算出extent能够划分出几个方格
	int hTotalNum = (int)(extent.height() / cellSize + 0.5); //高度上有几个方格
	int wTotalNum = (int)(extent.width() / cellSize + 0.5);//宽度上有几个方格
	//计算population的总量
	double populationSum = 0;
	for (int i = 0; i < population.size(); i++) {
		populationSum += population[i];
	}
	//开始计算每个划分方格的核值
	vector<double> hReasult; //临时记录高度上的结果，防止内存溢出
	for (int i = 0; i < wTotalNum; i++) {
		hReasult.clear();
		for (int j = 0; j < hTotalNum; j++) {
			pair<double, double> coord = getCdByNum(extent.left(), extent.top(), cellSize, i, j);
			double kernelSum = 0;
			for (int k = 0; k < points.size(); k++) {
				GeoPoint *point = points[k];
				//开始计算核
				double distance = method->computeDistance(coord.first, coord.second, point->getX(), point->getY());//计算距离
				double pop = population[k];
				double tempt = 3 / M_PI * pop * pow(1 - pow(distance / searchRadius, 2), 2);
				kernelSum += tempt;
			}
			hReasult.push_back(kernelSum / pow(searchRadius, 2) * populationSum);
		}
		kernelResult.push_back(hReasult);
	}
	return kernelResult;
}

