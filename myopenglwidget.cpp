#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(GeoMap *geoMap, QWidget *parent):QOpenGLWidget(parent)
{
	ui.setupUi(this);
	this->geoMap = geoMap;
	//计算缩放比例
	QRectF normalRange = geoMap->maxRange.normalized();  //先将地图范围规范化
	QPointF bottomLeft = normalRange.bottomLeft(); //QRectF的y轴向下
	QPointF topRight = normalRange.topRight();
	this->max_x = topRight.x();
	this->min_x = bottomLeft.x();
	this->min_y = topRight.y();  //top是min_y
	this->max_y = bottomLeft.y();
}

MyOpenGLWidget::~MyOpenGLWidget()
{

}

void MyOpenGLWidget::initializeGL(){
	initializeOpenGLFunctions();
    glClearColor(0,0,0,1);
	glColor3f(1,0,0);
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
}

void MyOpenGLWidget::paintGL(){

	//修改实际可视区域
	double mid_x = (max_x - min_x) / 2 + min_x;
	double mid_y = (max_y - min_y) / 2 + min_y;
	if (width > height) {
		//以高为基准
		double dy = max_y - min_y;
		double dx = dy * whRatio;
		if (dx < (max_x - min_x)) {
			//如果可视区域大于dx
			dx = max_x - min_x;
			//再修改dy
			dy = dx / whRatio;
			max_y = mid_y + dy / 2;
			min_y = mid_y - dy / 2;
		}
		max_x = mid_x + dx / 2;
		min_x = mid_x - dx / 2;
	}
	else {
		//以宽为基准
		double dx = max_x - min_x;
		double dy = dx / whRatio;
		if (dy < (max_y - min_y)) {
			//如果可视区域大于dy
			dy = max_y - min_y;
			//再修改dx
			dx = dy * whRatio;
			max_x = mid_x + dx / 2;
			min_x = mid_x - dx / 2;
		}
		max_y = mid_y + dy / 2;
		min_y = mid_y - dy / 2;
	}

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();//重置单位矩阵
	glOrtho(min_x, max_x, min_y, max_y, -1, 1);//创建三维世界中所能观察到的矩形区域
	//设置视口
	glViewport(0, 0, this->width, this->height);
	//开始绘制
	//判断地图是否可见
	if(!geoMap->isVisible)
		return;
	for(int i=0;i<geoMap->layers.size();i++){
		Layer *layer = geoMap->layers[i];
		drawLayer(layer);
	}
}

void MyOpenGLWidget::resizeGL(int width, int height){
	this->width = width;
	this->height = height;
	//计算比例
	this->whRatio = (float)width / this->height;
	//glOrtho(min_x, max_x, max_y, min_y, -1, 1);
}


/*
辅助画图函数
*/
void MyOpenGLWidget::drawLayer(Layer *layer){
	//判断图层是否可见
	if(!layer->isVisble)
		return;

	for(int i=0;i<layer->features.size();i++){
		Feature *feature = layer->features[i];
		Geometry *geometry = feature->geometry;
		if(GeometryType::GEOPOINT==geometry->getGeometryType()){
			//点绘制
			GeoPoint *point = (GeoPoint*) geometry;
			glBegin(GL_POINTS);
				glColor3f(1.0, 0.0, 0.0);
				glVertex2f(point->x, point->y);
			glEnd();
		}else if(GeometryType::GEOPOLYLINE==geometry->getGeometryType()){
			//线绘制
			glBegin(GL_LINES);
			GeoPolyline *polyline = (GeoPolyline *)geometry;
			for(int i=0;i<polyline->points.size();i++){
				GeoPoint *point = polyline->points[i];
				glColor3f(1.0, 0.0, 0.0);
				glVertex2f(point->x, point->y);
			}
			glEnd();
		}else if(GeometryType::GEOPOLYGON==geometry->getGeometryType()){
			//面绘制
			glBegin(GL_POLYGON);
			GeoPolygon *polygon = (GeoPolygon *)geometry;
			for(int i=0;i<polygon->points.size();i++){
				GeoPoint *point = polygon->points[i];
				glColor3f(1.0, 0.0, 0.0);
				glVertex2f(point->x, point->y);
			}
			glEnd();
		}else if(GeometryType::GEOMULTIPOLYGON==geometry->getGeometryType()){
			//多面绘制
			GeoMultiPolygon* multiPly = (GeoMultiPolygon*)geometry;
			for (int i = 0; i < multiPly->polygons.size(); i++)
			{
				
				glBegin(GL_POLYGON);
				GeoPolygon *polygon = multiPly->polygons.at(i);
				for (int j = 0; j < polygon->points.size(); j++) {
					GeoPoint *point = polygon->points[j];
					glColor3f(1.0, 0.0, 0.0);
					glVertex2f(point->x, point->y);
				}
				glEnd();
			}
		}else {
			//其他情况，暂不实现
		}
	}
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	//鼠标左键
	if (event->button() == Qt::LeftButton) {
		//开始
		mouseZoom.start();
		if (mouseZoom.mouseFlag == 0){
			QPoint p1 = event->pos();
			double modelview[16], projection[16];
			int viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
			glGetDoublev(GL_PROJECTION_MATRIX, projection);
			double winX = p1.rx();
			double winY = p1.ry();
			double winZ = 0;
			double x;
			double y;
			double z;
			//反解标准坐标
			gluUnProject(winX, winY, winZ, modelview, projection, viewport, &x, &y, &z);
			//得到世界坐标
			QPointF worldPoint = geoMap->NormalCd2worldCd(x, y);
			mouseZoom.mousePress(worldPoint);
		}
	}
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent * event)
{
	//在释放之前点击, 在此之前已经按压
	if (mouseZoom.mouseFlag == 1) {
		QPoint p1 = event->pos();
		double modelview[16], projection[16];
		int viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		double winX = p1.rx();
		double winY = p1.ry();
		double winZ = 0;
		double x;
		double y;
		double z;
		//反解标准坐标
		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &x, &y, &z);
		//得到世界坐标
		QPointF worldPoint = geoMap->NormalCd2worldCd(x, y);
		//计算出range的范围
		mouseZoom.mouseRelease(worldPoint);
		QRectF range = mouseZoom.range;
		QRectF normalRange = range.normalized();  //先将地图范围规范化
		QPointF bottomLeft = normalRange.bottomLeft(); //QRectF的y轴向下
		QPointF topRight = normalRange.topRight();
		this->max_x = topRight.x();
		this->min_x = bottomLeft.x();
		this->min_y = topRight.y();  //top是min_y
		this->max_y = bottomLeft.y();
		mouseZoom.end();
		update();
	}
}

  //   glBegin(GL_TRIANGLES);
  //      glColor3f(1.0, 0.0, 0.0);
  //      glVertex3f(550000, 550000, 0);
  //      glColor3f(0.0, 1.0, 0.0);
  //      glVertex3f( 250000, 150000, 0);
  //      glColor3f(0.0, 0.0, 1.0);
  //      glVertex3f( 200000,  250000, 0);
  //glEnd();