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
	//��ʼ����
	//�жϵ�ͼ�Ƿ�ɼ�
	if(!geoMap->isVisible)
		return;

	if(this->width<this->height){
		//�Կ�Ϊ�ο�
		double ratioHeigt = this->width*geoMap->hwRatio < this->height?this->width*geoMap->hwRatio:this->height;
		double ratioWidth = ratioHeigt == this->height?ratioHeigt*geoMap->whRatio:this->width;
		glViewport((this->width-ratioWidth)/2,(this->height-ratioHeigt)/2,ratioWidth, ratioHeigt);//����openglӦ�û������ĸ�����
	}else{
		double ratioWidth = this->height*geoMap->whRatio < this->width?this->height*geoMap->whRatio:this->width;
		double ratioHeight = ratioWidth==this->width?ratioWidth*geoMap->hwRatio:this->height;
		glViewport((this->width-ratioWidth)/2,(this->height-ratioHeight)/2,ratioWidth, ratioHeight);//����openglӦ�û������ĸ�����
	}
	for(int i=0;i<geoMap->layers.size();i++){
		Layer *layer = geoMap->layers[i];
		drawLayer(layer);
	}
}

void MyOpenGLWidget::resizeGL(int width, int height){
	this->width = width;
	this->height = height;
	//�����ӿ�
    glLoadIdentity();//���õ�λ����
    glMatrixMode(GL_PROJECTION);//ͶӰ�仯
	//�������ű���
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
	glOrtho(min_x,max_x,min_y,max_y,-1,1);//������ά���������ܹ۲쵽�ľ�������
}

void MyOpenGLWidget::drawLayer(Layer *layer){
	//�ж�ͼ���Ƿ�ɼ�
	if(!layer->isVisble)
		return;

	for(int i=0;i<layer->features.size();i++){
		Feature *feature = layer->features[i];
		Geometry *geometry = feature->geometry;
		if(GeometryType::GEOPOINT==geometry->getGeometryType()){
			//�����
			GeoPoint *point = (GeoPoint*) geometry;
			glBegin(GL_POINTS);
				glColor3f(1.0, 0.0, 0.0);
				glVertex2f(point->x, point->y);
			glEnd();
		}else if(GeometryType::GEOPOLYLINE==geometry->getGeometryType()){
			//�߻���
			glBegin(GL_LINES);
			GeoPolyline *polyline = (GeoPolyline *)geometry;
			for(int i=0;i<polyline->points.size();i++){
				GeoPoint *point = polyline->points[i];
				glColor3f(1.0, 0.0, 0.0);
				glVertex2f(point->x, point->y);
			}
			glEnd();
		}else if(GeometryType::GEOPOLYGON==geometry->getGeometryType()){
			//�����
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