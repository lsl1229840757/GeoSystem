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
// 参数 ： polygon : 输入的多边形顶点
//
bool GeoPolygon::isConvex()
{
	for (int i = 1; i < points.size()-1; i++) {
		//判断每一个点的凹凸性
		GeoPoint *p1 = points[i-1];
		GeoPoint *op = points[i];
		GeoPoint *p2 = points[i+1];
		if (multiply(op, p1, p2) > 0) {
			return false;
		}
	}
	return true;
}


//op为中间点,p1第一个点,p2第二个
double GeoPolygon::multiply(GeoPoint* op, GeoPoint* p1, const GeoPoint* p2)
{
	double vec[] = { op->x - p1->x, op->y - p1->y };
	double vec1[] = { p2->x - op->x, p2->y - op->y };
	double z = vec[0] * vec1[1] - vec[1] * vec1[0];
	return z;
}


vector<GeoPolygon*> GeoPolygon::getTriangles()
{
	if (triangles.size() == 0) {
		//先把points转为库的格式
		gpc_vertex* vertexes = new gpc_vertex[points.size()];
		for (int i = 0; i < points.size(); i++) {
			vertexes[i] = {points[i]->x, points[i]->y};
		}
		// 只是考虑单边界
		gpc_vertex_list vertexList = { points.size(), vertexes };
		int holeFlag = 0; 
		gpc_polygon polygon = {
			1, &holeFlag, &vertexList
		};
		gpc_tristrip tristrip;
		//开始剖分
		gpc_polygon_to_tristrip(&polygon, &tristrip);
		//转为自己的格式
		for (int i = 0; i < tristrip.num_strips; i++) {
			gpc_vertex_list triangleVertexList = tristrip.strip[i];
			for (int j = 0; j < triangleVertexList.num_vertices-2; j++) {
				GeoPolygon* triangle = new GeoPolygon;
				triangle->points.push_back(new GeoPoint(triangleVertexList.vertex[j].x, triangleVertexList.vertex[j].y));
				triangle->points.push_back(new GeoPoint(triangleVertexList.vertex[j+1].x, triangleVertexList.vertex[j+1].y));
				triangle->points.push_back(new GeoPoint(triangleVertexList.vertex[j+2].x, triangleVertexList.vertex[j+2].y));
				triangles.push_back(triangle);
			}
		}
	}
	return triangles;
}

//Vector2dVector a;
//for (int i = 0; i < points.size(); i++) {
//	a.push_back(Vector2d(points[i]->x, points[i]->y));
//}
////开始剖分
//Vector2dVector result;
//Triangulate::Process(a, result);
////把结果转为自己的格式
//int tcount = result.size() / 3;
//for (int i = 0; i < tcount; i++)
//{
//	const Vector2d &p1 = result[i * 3 + 0];
//	const Vector2d &p2 = result[i * 3 + 1];
//	const Vector2d &p3 = result[i * 3 + 2];
//	//首尾相连
//	GeoPolygon* triangle = new GeoPolygon;
//	triangle->points.push_back(new GeoPoint(p1.GetX(), p1.GetY()));
//	triangle->points.push_back(new GeoPoint(p2.GetX(), p2.GetY()));
//	triangle->points.push_back(new GeoPoint(p3.GetX(), p3.GetY()));
//	triangle->points.push_back(new GeoPoint(p1.GetX(), p1.GetY()));
//	triangles.push_back(triangle);
//}

