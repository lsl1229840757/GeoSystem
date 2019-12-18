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
	//���ȼ����extent�ܹ����ֳ���������
	int hTotalNum = (int)(extent.height() / cellSize + 0.5); //�߶����м�������
	int wTotalNum = (int)(extent.width() / cellSize + 0.5);//������м�������
	//����population������
	double populationSum = 0;
	for (int i = 0; i < population.size(); i++) {
		populationSum += population[i];
	}
	//��ʼ����ÿ�����ַ���ĺ�ֵ
	vector<double> hReasult; //��ʱ��¼�߶��ϵĽ������ֹ�ڴ����
	for (int i = 0; i < wTotalNum; i++) {
		hReasult.clear();
		for (int j = 0; j < hTotalNum; j++) {
			pair<double, double> coord = getCdByNum(extent.left(), extent.top(), cellSize, i, j);
			double kernelSum = 0;
			for (int k = 0; k < points.size(); k++) {
				GeoPoint *point = points[k];
				//��ʼ�����
				double distance = method->computeDistance(coord.first, coord.second, point->getX(), point->getY());//�������
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

