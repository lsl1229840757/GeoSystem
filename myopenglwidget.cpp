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
	glViewport(0, 0, this->width, this->height);
	for(int i=0;i<geoMap->layers.size();i++){
		Layer *layer = geoMap->layers[i];
		drawLayer(layer);
	}
}

void MyOpenGLWidget::resizeGL(int width, int height){
	this->width = width;
	this->height = height;
	//�������
	double whRatio = (float)width / this->height;
	//�����ӿ�
    glLoadIdentity();//���õ�λ����
    glMatrixMode(GL_PROJECTION);//ͶӰ�仯
	//�������ű���
	QRectF normalRange = geoMap->maxRange.normalized();  //�Ƚ���ͼ��Χ�淶��
	QPointF bottomLeft = normalRange.bottomLeft(); //QRectF��y������
	QPointF topRight = normalRange.topRight();
	
	double max_x = topRight.x();
	double min_x = bottomLeft.x();
	double min_y = topRight.y();  //top��min_y
	double max_y = bottomLeft.y();
	
	//opengl��y������
	//�޸�ʵ�ʿ�������
	double mid_x = (max_x - min_x) / 2 + min_x;
	double mid_y = (max_y - min_y) / 2 + min_y;
	if(width>height){
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
	}else{
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
	glOrtho(min_x,max_x,min_y,max_y,-1,1);//������ά���������ܹ۲쵽�ľ�������
	//glOrtho(min_x, max_x, max_y, min_y, -1, 1);
}

void MyOpenGLWidget::drawLayer(Layer *layer){
	//�ж�ͼ���Ƿ�ɼ�
	if(!layer->isVisble)
		return;

	for(int i=0;i<layer->features.size();i++){
		Feature *feature = layer->features[i];
		Geometry *geometry = feature->geometry;
		SymbolStyle symbolStyle = feature->symbolStyle;
		float maxColorComponent = 255.0;
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
				//�жϱ߽���ɫ�Ƿ񴢴�
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
			//�����
			glBegin(GL_POLYGON);
			GeoPolygon *polygon = (GeoPolygon *)geometry;
			for(int i=0;i<polygon->points.size();i++){
				GeoPoint *point = polygon->points[i];
				//�ж��������ɫ�Ƿ񴢴�
				if(symbolStyle.fillColor.isValid())
					glColor3f(symbolStyle.fillColor.red()/ maxColorComponent,
						symbolStyle.fillColor.green()/ maxColorComponent, 
						symbolStyle.fillColor.blue()/ maxColorComponent);
				else
					glColor3f(0.0, 1.0, 0.0);
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
			//����������ݲ�ʵ��
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