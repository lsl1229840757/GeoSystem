#include "GeoPolygon.h"


GeoPolygon::GeoPolygon(void)
{
}


GeoPolygon::~GeoPolygon(void)
{
}

GeometryType GeoPolygon::getGeometryType(){
	return GeometryType::GEOPOLYGON;
}


//
// ���� �� polygon : ����Ķ���ζ���
//
bool GeoPolygon::isConvex()
{
	for (int i = 1; i < points.size()-1; i++) {
		//�ж�ÿһ����İ�͹��
		GeoPoint *p1 = points[i-1];
		GeoPoint *op = points[i];
		GeoPoint *p2 = points[i+1];
		if (multiply(op, p1, p2) > 0) {
			return false;
		}
	}
	return true;
}


//opΪ�м��,p1��һ����,p2�ڶ���
double GeoPolygon::multiply(GeoPoint* op, GeoPoint* p1, const GeoPoint* p2)
{
	double vec[] = { op->x - p1->x, op->y - p1->y };
	double vec1[] = { p2->x - op->x, p2->y - op->y };
	double z = vec[0] * vec1[1] - vec[1] * vec1[0];
	return z;
}


vector<GeoPolygon*> GeoPolygon::getTriangles()
{
	//�Ȱ�pointsתΪ��ĸ�ʽ
	Vector2dVector a;
	for (int i = 0; i < points.size(); i++) {
		a.push_back(Vector2d(points[i]->x, points[i]->y));
	}
	//��ʼ�ʷ�
	Vector2dVector result;
	Triangulate::Process(a, result);
	//�ѽ��תΪ�Լ��ĸ�ʽ
	vector<GeoPolygon*> triangles;
	int tcount = result.size() / 3;
	for (int i = 0; i < tcount; i++)
	{
		const Vector2d &p1 = result[i * 3 + 0];
		const Vector2d &p2 = result[i * 3 + 1];
		const Vector2d &p3 = result[i * 3 + 2];
		//��β����
		GeoPolygon* triangle = new GeoPolygon;
		triangle->points.push_back(new GeoPoint(p1.GetX(), p1.GetY()));
		triangle->points.push_back(new GeoPoint(p2.GetX(), p2.GetY()));
		triangle->points.push_back(new GeoPoint(p3.GetX(), p3.GetY()));
		triangle->points.push_back(new GeoPoint(p1.GetX(), p1.GetY()));
		triangles.push_back(triangle);
	}
	return triangles;
}

