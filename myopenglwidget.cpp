#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(GeoMap *geoMap, QWidget *parent):QOpenGLWidget(parent)
{
	ui.setupUi(this);
	this->geoMap = geoMap;
	//跟踪鼠标轨迹
	setMouseTracking(true);
	//初始化mouseZoom
	mouseZoom = MouseZoomAction(geoMap->maxRange);
	this->centerPos = geoMap->maxRange.center();
	//计算缩放比例
	QRectF normalRange = geoMap->maxRange.normalized();  //先将地图范围规范化
	this->viewRange = normalRange;
	//QPointF topRight = normalRange.topRight();
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
	this->viewRange = this->viewRange.normalized();
	double max_x = viewRange.right();
	double min_x = viewRange.left();
	double min_y = viewRange.top();  //top是min_y
	double max_y = viewRange.bottom();
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
		SymbolStyle symbolStyle = feature->symbolStyle;
		float maxColorComponent = 255.0;
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
				//判断边界颜色是否储存
				if (symbolStyle.strokeColor.isValid())
					glColor3f(symbolStyle.strokeColor.red() / maxColorComponent,
						symbolStyle.strokeColor.green() / maxColorComponent,
						symbolStyle.strokeColor.blue() / maxColorComponent);
				else
					glColor3f(0.0, 1.0, 0.0);
				glVertex2f(point->x, point->y);
			}
			glEnd();
		}else if(GeometryType::GEOPOLYGON==geometry->getGeometryType()){
			//面绘制
			GeoPolygon *polygon = (GeoPolygon *)geometry;
			if (!polygon->isConvex()) {
				//不是凸多边形,开始剖分
				vector<GeoPolygon *> triangles = polygon->getTriangles();
				glBegin(GL_TRIANGLE_STRIP);
				for (int j = 0; j < triangles.size(); j++) {
					for (int i = 0; i < triangles[j]->points.size(); i++) {
						GeoPoint *point = triangles[j]->points[i];
						if (symbolStyle.fillColor.isValid())
						{
							glColor3f(symbolStyle.fillColor.red() / maxColorComponent,
								symbolStyle.fillColor.green() / maxColorComponent,
								symbolStyle.fillColor.blue() / maxColorComponent);
						}
						else glColor3f(0.0, 1.0, 0.0);
						glVertex2f(point->x, point->y);
					}
				}
				glEnd();
			}
			else {
				glBegin(GL_POLYGON);
				for (int i = 0; i < polygon->points.size(); i++) {
					GeoPoint *point = polygon->points[i];
					if (symbolStyle.fillColor.isValid())
					{
						glColor3f(symbolStyle.fillColor.red() / maxColorComponent,
							symbolStyle.fillColor.green() / maxColorComponent,
							symbolStyle.fillColor.blue() / maxColorComponent);
					}
					else glColor3f(1.0, 0.0, 0.0);
					glVertex2f(point->x, point->y);
				}
				glEnd();
			}
		}else if(GeometryType::GEOMULTIPOLYGON==geometry->getGeometryType()){
			//多面绘制
			GeoMultiPolygon* multiPly = (GeoMultiPolygon*)geometry;
			for (int i = 0; i < multiPly->polygons.size(); i++)
			{
				glBegin(GL_POLYGON);
				GeoPolygon *polygon = multiPly->polygons.at(i);
				for (int j = 0; j < polygon->points.size(); j++) {
					GeoPoint *point = polygon->points[j];
					if (symbolStyle.fillColor.isValid())
						glColor3f(symbolStyle.fillColor.red() / maxColorComponent,
							symbolStyle.fillColor.green() / maxColorComponent, 
							symbolStyle.fillColor.blue() / maxColorComponent);
					else
						glColor3f(0.0, 1.0, 0.0);
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
	if (event->button() == Qt::LeftButton) {
		QPoint p = event->pos();
		//记录下来坐标点
		mouseDrag.begin(screenCd2worldCd(p));
	}
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		mouseDrag.finish();
	}
}


void MyOpenGLWidget::wheelEvent(QWheelEvent * event)
{
	if (event->delta() > 0) {//如果滚轮往上滚，就放大
		QRectF vRange = mouseZoom.zoomIn(this->centerPos);
		this->viewRange = vRange;
		update();
	}
	else {//同样的,缩小
		QRectF vRange = mouseZoom.zoomOut(this->centerPos);
		this->viewRange = vRange;
		update();
	}
}

void MyOpenGLWidget::mouseMoveEvent(QMouseEvent * event)
{
	QPoint p = event->pos();
	//记录下来坐标点, 控制放大缩小
	QPointF currentPos = screenCd2worldCd(p);
	this->centerPos = currentPos;
	//控制拖动
	if (mouseDrag.dragFlag == 1) {//按住左键,虽然在mouseDrag有相应控制，这里加上防止每次都调用update
		QPointF center = this->viewRange.center();
		vector<double> moveMent = mouseDrag.end(screenCd2worldCd(p));//相对运动所以相减
		center.setX(center.rx() - moveMent[0]);
		center.setY(center.ry() - moveMent[1]);
		this->viewRange.moveCenter(center);
		update();
	}
	emit sendCurrentPos(currentPos);
}

QPointF MyOpenGLWidget::normalCd2worldCd(double x, double y)
{
	double max_x = viewRange.right();
	double min_x = viewRange.left();
	double min_y = viewRange.top();  //top是min_y
	double max_y = viewRange.bottom();
	double dx = viewRange.width();
	double dy = viewRange.height();
	QPointF p = viewRange.center();
	x = (dx * x) / 2 + viewRange.center().x();
	y = (dy * y) / 2 + viewRange.center().y();
	return QPointF(x, y);
}

QPointF MyOpenGLWidget::screenCd2worldCd(QPointF screenPoint)
{
	double modelview[16], projection[16];
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	double x;
	double y;
	double z;
	double winX = screenPoint.rx();
	double winY = screenPoint.ry();
	double winZ = 0;
	//反解标准坐标
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &x, &y, &z);
	// ??这里的标准化坐标是上面-1，下面+1
	return normalCd2worldCd(x, -y);
}

  //   glBegin(GL_TRIANGLES);
  //      glColor3f(1.0, 0.0, 0.0);
  //      glVertex3f(550000, 550000, 0);
  //      glColor3f(0.0, 1.0, 0.0);
  //      glVertex3f( 250000, 150000, 0);
  //      glColor3f(0.0, 0.0, 1.0);
  //      glVertex3f( 200000,  250000, 0);
  //glEnd();