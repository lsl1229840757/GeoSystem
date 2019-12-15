#include "myopenglwidget.h"

MyOpenGLWidget::MyOpenGLWidget(GeoMap *geoMap, QWidget *parent):QOpenGLWidget(parent)
{
	ui.setupUi(this);
	this->geoMap = geoMap;
	//跟踪鼠标轨迹
	setMouseTracking(true);

	QRectF normalRange;
	//计算缩放比例
	if (geoMap->mapPrj != NULL){
		QRectF prjRange = geoMap->mapPrj->getPrjRange(geoMap->maxRange.normalized());
		normalRange = prjRange.normalized();  //将地图范围规范化
	}else {
		normalRange = geoMap->maxRange.normalized();  //先将地图范围规范化
	}
	this->viewRange = normalRange;
	//QPointF topRight = normalRange.topRight();
	//初始化mouseZoom
	mouseZoom = MouseZoomAction(this->viewRange);  //改成传入实际显示的range,避免投影坐标反算
	this->centerPos = geoMap->maxRange.center();
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
	//改变地图投影之后绘制前，重置范围
	if (geoMap->mapPrj!=NULL && geoMap->mapPrj->mapPrjChanged == true)
	{
		geoMap->mapPrj->mapPrjChanged = false;
		for (int i = 0 ; i < geoMap->layers.size(); i++)
		{
			Layer *layer = geoMap->layers.at(i);
			for (int j = 0; j < layer->features.size(); j++)
			{
				//重置isFirstProjeted,回复第一次投影状态
				layer->features.at(j)->isFirstProjeted = true;
			}
		}
		QRectF prjRange = geoMap->mapPrj->getPrjRange(geoMap->maxRange.normalized());
		QRectF normalRange= prjRange.normalized();  //将地图范围规范化
		this->viewRange = normalRange;
	}
	//
	this->viewRange = this->viewRange.normalized();
	double max_x = viewRange.right();
	double min_x = viewRange.left();
	double min_y = viewRange.top();  //top是min_y
	double max_y = viewRange.bottom();
	//修改实际可视区域
	double mid_x = (max_x - min_x) / 2 + min_x;
	double mid_y = (max_y - min_y) / 2 + min_y;
	double mapwhRatio = (max_x - min_x) / (max_y - min_y);//地图宽高比
	if (mapwhRatio >= whRatio) {
		//如果地图宽高比较大,以宽为基准
		double dy = (max_x - min_x)/whRatio;
		max_y = mid_y + dy / 2;
		min_y = mid_y - dy / 2;
	}else{
		//如果地图宽高比较小，以高为基准
		double dx = (max_y - min_y)*whRatio;
		max_x = mid_x + dx / 2;
		min_x = mid_x - dx / 2;
	}
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();//重置单位矩阵
	glOrtho(min_x, max_x, min_y, max_y, -1, 1);//创建三维世界中所能观察到的矩形区域
	//设置视口
	glViewport(0, 0, this->width, this->height);
	//开始绘制
	//判断索引情况
	if (geoMap->index != NULL && geoMap->index->isIndexCreated)
	{
		if (SpatialIndexType::GRID == geoMap->index->getIndexType())
		{ 
			//绘制线框
			for (int i = 0; i < ((GridIndex*)geoMap->index)->grids.size(); i++)
			{
				Grid* grid = ((GridIndex*)geoMap->index)->grids.at(i);
				QRectF gridBound = grid->gridBoundary;
				if (geoMap->mapPrj != NULL)
					gridBound = geoMap->mapPrj->getPrjRange(gridBound);
				glBegin(GL_LINES);
					glColor3f(1, 1, 1);
					glVertex2f(gridBound.left(), gridBound.top());
					glVertex2f(gridBound.right(), gridBound.top());
				glEnd();
				glBegin(GL_LINES);
					glColor3f(1, 1, 1);
					glVertex2f(gridBound.left(), gridBound.top());
					glVertex2f(gridBound.left(), gridBound.bottom());
				glEnd();
				
			}
		}
	}
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
		mgeo::Geometry *geometry = feature->geometry;
		SymbolStyle symbolStyle = feature->symbolStyle;
		float maxColorComponent = 255.0;
		GLfloat normalFillRed, normalFillGreen, normalFillBlue, normalStrokeRed, normalStrokeGreen, normalStrokeBlue;
		//规范化fill颜色
		if (symbolStyle.fillColor.isValid()) {//判断fill颜色是否储存
			normalFillRed = symbolStyle.fillColor.red() / maxColorComponent;
			normalFillGreen = symbolStyle.fillColor.green() / maxColorComponent;
			normalFillBlue = symbolStyle.fillColor.blue() / maxColorComponent;
		}else {
			normalFillRed = 1.0;
			normalFillGreen = 0.0;
			normalFillBlue = 0.0;
		}

		//规范化stroke颜色
		if (symbolStyle.strokeColor.isValid()) {
			normalStrokeRed = symbolStyle.strokeColor.red() / maxColorComponent;
			normalStrokeGreen = symbolStyle.strokeColor.green() / maxColorComponent;
			normalStrokeBlue = symbolStyle.strokeColor.blue() / maxColorComponent;
		}else {
			normalStrokeRed = 0.0;
			normalStrokeGreen = 1.0;
			normalStrokeBlue = 1.0;
		}

		if(GeometryType::GEOPOINT==geometry->getGeometryType()){
			//点绘制
			GeoPoint *point = (GeoPoint*) geometry;
			glBegin(GL_POINTS);
			if (feature->isSelected) {
				//查询和后被选中点变成蓝色
				glColor3f(0.0, 0.0, 1.0);
				if (geoMap->mapPrj != NULL) {
					//判断是否为第一次投影，并储存投影坐标
					if(feature->isFirstProjeted){
						feature->isFirstProjeted = false;
						double prjx, prjy;
						geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
						point->prjx = prjx;
						point->prjy = prjy;
						glVertex2f(prjx, prjy);
					}else{
						glVertex2f(point->prjx, point->prjy);
					}
				}
				else {
					glVertex2f(point->x, point->y);
				}
			}
			else {
				glColor3f(1.0, 0.0, 0.0);
				if (geoMap->mapPrj != NULL) {
					if (feature->isFirstProjeted) {
						feature->isFirstProjeted = false;
						double prjx, prjy;
						geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
						point->prjx = prjx;
						point->prjy = prjy;
						glVertex2f(prjx, prjy);
					}
					else {
						glVertex2f(point->prjx, point->prjy);
					}
				}
				else {
					glVertex2f(point->x, point->y);
				}
			}
			glEnd();
		}else if(GeometryType::GEOPOLYLINE==geometry->getGeometryType()){
			//线绘制
			glLineWidth(symbolStyle.strokeWidth);//线宽
			//glLineStipple(1, 0xFFFF);  //点绘制实线
			glBegin(GL_LINE_STRIP);
			GeoPolyline *polyline = (GeoPolyline *)geometry;
			for(int j=0;j<polyline->points.size();j++){
				GeoPoint *point = polyline->points[j];
				if (feature->isSelected) {
					//查询后被选中线变为蓝色
					glColor3f(0, 0, 1);
				}
				else {
					glColor3f(normalStrokeRed, normalStrokeGreen, normalStrokeBlue);
				}
				if (geoMap->mapPrj != NULL) {
					if (feature->isFirstProjeted) {
						//最后一个元素时改变
						if(j==polyline->points.size()-1)
							feature->isFirstProjeted = false;
						double prjx, prjy;
						geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
						point->prjx = prjx;
						point->prjy = prjy;
						glVertex2f(prjx, prjy);
					}
					else {
						glVertex2f(point->prjx, point->prjy);
					}
				}
				else {
						glVertex2f(point->x, point->y);
				}
			}
			glEnd();
		}else if(GeometryType::GEOPOLYGON==geometry->getGeometryType()){
			//面绘制
			GeoPolygon *polygon = (GeoPolygon *)geometry;
			if (!polygon->isConvex()) {
				//剖分前先记录原始点坐标的投影坐标
				if (geoMap->mapPrj != NULL) {
					if (feature->isFirstProjeted) {
						for (int j = 0; j < polygon->points.size(); j++) {
							GeoPoint *point = polygon->points[j];
							double prjx, prjy;
							geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
							point->prjx = prjx;
							point->prjy = prjy;
						}
					}
				}
				//不是凸多边形,开始剖分
				vector<GeoPolygon *> triangles = polygon->getTriangles();
				glBegin(GL_TRIANGLE_STRIP);
				for (int j = 0; j < triangles.size(); j++) {
					for (int k = 0; k < triangles[j]->points.size(); k++) {
						GeoPoint *point = triangles[j]->points[k];
						if (feature->isSelected) {
							glColor3f(0, 0, 1);
						}else{
							glColor3f(normalFillRed, normalFillGreen, normalFillBlue);
						}
						if (geoMap->mapPrj != NULL) {
							if (feature->isFirstProjeted) {
								//最后一个元素时改变
								if ((k == triangles[j]->points.size() - 1)&& (j==triangles.size()-1))
									feature->isFirstProjeted = false;
								double prjx, prjy;
								geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
								point->prjx = prjx;
								point->prjy = prjy;
								glVertex2f(prjx, prjy);
							}
							else {
								glVertex2f(point->prjx, point->prjy);
							}
						}
						else {
							glVertex2f(point->x, point->y);
						}
					}
				}
				glEnd();
			}
			else {
				glBegin(GL_POLYGON);
				for (int j = 0; j < polygon->points.size(); j++) {
					GeoPoint *point = polygon->points[j];
					if (feature->isSelected) {
						glColor3f(0, 0, 1);
					}
					else {
						glColor3f(normalFillRed, normalFillGreen, normalFillBlue);
					}
					if (geoMap->mapPrj != NULL) {
						if (feature->isFirstProjeted) {
							//最后一个元素时改变
							if (j == polygon->points.size() - 1)
								feature->isFirstProjeted = false;
							double prjx, prjy;
							geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
							point->prjx = prjx;
							point->prjy = prjy;
							glVertex2f(prjx,prjy);
						}
						else {
							glVertex2f(point->prjx, point->prjy);
						}
					}
					else {
						glVertex2f(point->x, point->y);
					}
				}
				glEnd();
			}
			//描绘多边形边界
			glLineWidth(symbolStyle.strokeWidth);
			//glLineStipple(1, 0xFFFF);  //点绘制实线
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j < polygon->points.size(); j++) {
				GeoPoint *point = polygon->points[j];
				if (feature->isSelected) {
					//如果被查询后选中,边界变成蓝色
					glColor3f(0, 0, 1);
				}
				else {
					glColor3f(normalStrokeRed, normalStrokeGreen, normalStrokeBlue);
				}
				if (geoMap->mapPrj != NULL) {
					if (feature->isFirstProjeted) {
						//最后一个元素时改变
						if (j == polygon->points.size() - 1)
							feature->isFirstProjeted = false;
						double prjx, prjy;
						geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
						point->prjx = prjx;
						point->prjy = prjy;
						glVertex2f(prjx, prjx);
					}
					else {
						glVertex2f(point->prjx, point->prjy);
					}
				}
				else {
					glVertex2f(point->x, point->y);
				}
			}
			glEnd();
		}else if(GeometryType::GEOMULTIPOLYGON==geometry->getGeometryType()){
			//多面绘制
			GeoMultiPolygon* multiPly = (GeoMultiPolygon*)geometry;
			for (int m = 0; m < multiPly->polygons.size(); m++)
			{
				GeoPolygon *polygon = multiPly->polygons[m];
				if (!polygon->isConvex()) {
					//剖分前先记录原始点坐标的投影坐标
					if (geoMap->mapPrj != NULL) {
						if (feature->isFirstProjeted) {
							for (int j = 0; j < polygon->points.size(); j++) {
								GeoPoint *point = polygon->points[j];
								double prjx, prjy;
								geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
								point->prjx = prjx;
								point->prjy = prjy;
							}
						}
					}
					//不是凸多边形,开始剖分
					vector<GeoPolygon *> triangles = polygon->getTriangles();
					glBegin(GL_TRIANGLE_STRIP);
					for (int j = 0; j < triangles.size(); j++) {
						for (int k = 0; k < triangles[j]->points.size(); k++) {
							GeoPoint *point = triangles[j]->points[k];
							if (feature->isSelected) {
								glColor3f(0, 0, 1);
							}
							else {
								glColor3f(normalFillRed, normalFillGreen, normalFillBlue);
							}
							if (geoMap->mapPrj != NULL) {
								if (feature->isFirstProjeted) {
									//最后一个元素时改变
									if ((k == triangles[j]->points.size() - 1)
										&& (j == triangles.size() - 1)
										&& (m==multiPly->polygons.size()-1))
										feature->isFirstProjeted = false;
									double prjx, prjy;
									geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
									point->prjx = prjx;
									point->prjy = prjy;
									glVertex2f(prjx, prjy);
								}
								else {
									glVertex2f(point->prjx, point->prjy);
								}
							}
							else {
								glVertex2f(point->x, point->y);
							}
						}
					}
					glEnd();
				}
				else {
					glBegin(GL_POLYGON);
					for (int k = 0; k < polygon->points.size(); k++) {
						GeoPoint *point = polygon->points[k];
						glColor3f(normalFillRed, normalFillGreen, normalFillBlue);
						if (geoMap->mapPrj != NULL) {
							if (feature->isFirstProjeted) {
								//最后一个元素时改变
								if ((k == polygon->points.size() - 1) && (m == multiPly->polygons.size() - 1))
									feature->isFirstProjeted = false;
								double prjx, prjy;
								geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
								point->prjx = prjx;
								point->prjy = prjy;
								glVertex2f(prjx, prjy);
							}
							else {
								glVertex2f(point->prjx, point->prjy);
							}
						}
						else {
							glVertex2f(point->x, point->y);
						}
					}
					glEnd();
				}
			}
			//描绘多面边界
			glLineWidth(symbolStyle.strokeWidth);
			for (int m = 0; m < multiPly->polygons.size(); m++)
			{
				GeoPolygon *polygon = multiPly->polygons.at(m);
				glBegin(GL_LINE_STRIP);
				for (int j = 0; j < polygon->points.size(); j++) {
					GeoPoint *point = polygon->points[j];
					if (feature->isSelected) {
						//如果被查询后选中,边界变成蓝色
						glColor3f(0, 0, 1);
					}
					else {
						glColor3f(normalStrokeRed, normalStrokeGreen, normalStrokeBlue);
					}
					if (geoMap->mapPrj != NULL) {
						if (feature->isFirstProjeted) {
							//最后一个元素时改变
							if (j == polygon->points.size() - 1)
								feature->isFirstProjeted = false;
							double prjx, prjy;
							geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
							point->prjx = prjx;
							point->prjy = prjy;
							glVertex2f(prjx, prjx);
						}
						else {
							glVertex2f(point->prjx, point->prjy);
						}
					}
					else {
						glVertex2f(point->x, point->y);
					}
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
	/*
	QPointF blCenterPos = this->centerPos;
	double bb, ll;  //经纬度
	if (geoMap->mapPrj != NULL)
	{
		//有投影时先反算经纬度
		geoMap->mapPrj->getBL(this->centerPos.rx(),this->centerPos.ry(),&ll,&bb);
		blCenterPos = QPointF(ll,bb);
	}*/

	if (event->delta() > 0) {//如果滚轮往上滚，就放大
		
		QRectF vRange = mouseZoom.zoomIn(this->centerPos); 
		//if (geoMap->mapPrj != NULL){
		//	this->viewRange = geoMap->mapPrj->getPrjRange(vRange);
		//}
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