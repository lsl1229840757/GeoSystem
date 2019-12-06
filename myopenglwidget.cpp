#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(GeoMap *geoMap, QWidget *parent):QOpenGLWidget(parent)
{
	ui.setupUi(this);
	this->geoMap = geoMap;
	//�������ű���
	QRectF normalRange = geoMap->maxRange.normalized();  //�Ƚ���ͼ��Χ�淶��
	QPointF bottomLeft = normalRange.bottomLeft(); //QRectF��y������
	QPointF topRight = normalRange.topRight();
	this->max_x = topRight.x();
	this->min_x = bottomLeft.x();
	this->min_y = topRight.y();  //top��min_y
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

	//�޸�ʵ�ʿ�������
	double mid_x = (max_x - min_x) / 2 + min_x;
	double mid_y = (max_y - min_y) / 2 + min_y;
	if (width > height) {
		//�Ը�Ϊ��׼
		double dy = max_y - min_y;
		double dx = dy * whRatio;
		if (dx < (max_x - min_x)) {
			//��������������dx
			dx = max_x - min_x;
			//���޸�dy
			dy = dx / whRatio;
			max_y = mid_y + dy / 2;
			min_y = mid_y - dy / 2;
		}
		max_x = mid_x + dx / 2;
		min_x = mid_x - dx / 2;
	}
	else {
		//�Կ�Ϊ��׼
		double dx = max_x - min_x;
		double dy = dx / whRatio;
		if (dy < (max_y - min_y)) {
			//��������������dy
			dy = max_y - min_y;
			//���޸�dx
			dx = dy * whRatio;
			max_x = mid_x + dx / 2;
			min_x = mid_x - dx / 2;
		}
		max_y = mid_y + dy / 2;
		min_y = mid_y - dy / 2;
	}

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();//���õ�λ����
	glOrtho(min_x, max_x, min_y, max_y, -1, 1);//������ά���������ܹ۲쵽�ľ�������
	//�����ӿ�
	glViewport(0, 0, this->width, this->height);
	//��ʼ����
	//�жϵ�ͼ�Ƿ�ɼ�
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
	//�������
	this->whRatio = (float)width / this->height;
	//glOrtho(min_x, max_x, max_y, min_y, -1, 1);
}


/*
������ͼ����
*/
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
		}else if(GeometryType::GEOMULTIPOLYGON==geometry->getGeometryType()){
			//�������
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
			//����������ݲ�ʵ��
		}
	}
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	//������
	if (event->button() == Qt::LeftButton) {
		//��ʼ
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
			//�����׼����
			gluUnProject(winX, winY, winZ, modelview, projection, viewport, &x, &y, &z);
			//�õ���������
			QPointF worldPoint = geoMap->NormalCd2worldCd(x, y);
			mouseZoom.mousePress(worldPoint);
		}
	}
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent * event)
{
	//���ͷ�֮ǰ���, �ڴ�֮ǰ�Ѿ���ѹ
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
		//�����׼����
		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &x, &y, &z);
		//�õ���������
		QPointF worldPoint = geoMap->NormalCd2worldCd(x, y);
		//�����range�ķ�Χ
		mouseZoom.mouseRelease(worldPoint);
		QRectF range = mouseZoom.range;
		QRectF normalRange = range.normalized();  //�Ƚ���ͼ��Χ�淶��
		QPointF bottomLeft = normalRange.bottomLeft(); //QRectF��y������
		QPointF topRight = normalRange.topRight();
		this->max_x = topRight.x();
		this->min_x = bottomLeft.x();
		this->min_y = topRight.y();  //top��min_y
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