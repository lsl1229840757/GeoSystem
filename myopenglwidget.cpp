#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(GeoMap *geoMap, QWidget *parent):QOpenGLWidget(parent)
{
	ui.setupUi(this);

	this->geoMap = geoMap;
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
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//开始绘制
	//判断地图是否可见
	if(!geoMap->isVisible)
		return;

	if(this->width<this->height){
		//以宽为参考
		double ratioHeigt = this->width*geoMap->hwRatio < this->height?this->width*geoMap->hwRatio:this->height;
		double ratioWidth = ratioHeigt == this->height?ratioHeigt*geoMap->whRatio:this->width;
		glViewport((this->width-ratioWidth)/2,(this->height-ratioHeigt)/2,ratioWidth, ratioHeigt);//告诉opengl应该绘制在哪个区域
	}else{
		double ratioWidth = this->height*geoMap->whRatio < this->width?this->height*geoMap->whRatio:this->width;
		double ratioHeight = ratioWidth==this->width?ratioWidth*geoMap->hwRatio:this->height;
		glViewport((this->width-ratioWidth)/2,(this->height-ratioHeight)/2,ratioWidth, ratioHeight);//告诉opengl应该绘制在哪个区域
	}
	for(int i=0;i<geoMap->layers.size();i++){
		Layer *layer = geoMap->layers[i];
		drawLayer(layer);
	}
}

void MyOpenGLWidget::resizeGL(int width, int height){
	this->width = width;
	this->height = height;
	//设置视口
    glLoadIdentity();//重置单位矩阵
    glMatrixMode(GL_PROJECTION);//投影变化
	//计算缩放比例
	QPointF bottomLeft = geoMap->maxRange.bottomLeft();
	QPointF topRight = geoMap->maxRange.topRight();
	double max_x = topRight.x();
	double min_x = bottomLeft.x();
	double max_y = topRight.y();
	double min_y = bottomLeft.y();
	//if(width>height){
	//	glScalef(1, geoMap->whRatio, 1);
	//}else{
	//	glScalef(1, geoMap->hwRatio, 1);
	//}
	glOrtho(min_x,max_x,min_y,max_y,-1,1);//创建三维世界中所能观察到的矩形区域
}

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
		}
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