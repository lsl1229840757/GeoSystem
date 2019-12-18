#include "myopenglwidget.h"
static vector<QColor> colorList = {QColor(144,238,144), QColor(32,124,202), QColor(49,255,0), QColor(248,255,0),QColor(255,5,0)};
MyOpenGLWidget::MyOpenGLWidget(GeoMap *geoMap, QWidget *parent):QOpenGLWidget(parent)
{
	ui.setupUi(this);
	this->geoMap = geoMap;
	//跟踪鼠标轨迹
	setMouseTracking(true);
	//获取地图Range
	QRectF normalRange = geoMap->getMapRange();
	//计算缩放比例
	this->viewRange = normalRange;
	//QPointF topRight = normalRange.topRight();
	//初始化mouseZoom
	mouseZoom = MouseZoomAction(this->viewRange);  //改成传入实际显示的range,避免投影坐标反算
	this->centerPos = geoMap->maxRange.center();
	isMouseMovement = false;
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
	if (!geoMap->mapPrjEmpty() && geoMap->mapPrj->mapPrjChanged == true)
	{
		geoMap->mapPrj->mapPrjChanged = false;
		geoMap->resetFeaturePrjStatus(); //重置feature投影状态
		this->viewRange = geoMap->getMapRange();
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
	
	
	//判断地图是否可见
	if(!geoMap->isVisible)
		return;
	for(int i=0;i<geoMap->layers.size();i++){
		Layer *layer = geoMap->layers[i];
		drawLayer(layer);
	}
	//判断索引情况,绘制索引边界
	drawIndex();
}

void MyOpenGLWidget::resizeGL(int width, int height){
	this->width = width;
	this->height = height;
	//计算比例
	this->whRatio = (float)width / this->height;
	glViewport(0, 0, this->width, this->height);
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
		if (feature->isSelected) {
			//构造一个全蓝色的symbol
			SymbolStyle *selectSymbol = new SymbolStyle;
			selectSymbol->fillColor = QColor(1, 254, 254);
			selectSymbol->strokeWidth = feature->symbolStyle.strokeWidth;
			selectSymbol->strokeColor = feature->symbolStyle.strokeColor;
			drawFeature(feature, selectSymbol);
		}
		else {
			drawFeature(feature);
		}
	}
}

void MyOpenGLWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		isMouseMovement = false;
		QPoint p = event->pos();
		//记录下来坐标点
		mouseDrag.begin(screenCd2worldCd(p));
	}
}

void MyOpenGLWidget::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		mouseDrag.finish();
		if (!isMouseMovement) {
			QPoint p = event->pos();
			searchByClick(p);
		}
	}
	isMouseMovement = false;
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
	isMouseMovement = true;
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
	/*double b, l;
	geoMap->mapPrj->getBL(currentPos.x(), currentPos.y(), &l, &b);
	emit sendCurrentPos(QPointF(l, b));*/
	emit sendCurrentPos(currentPos); //投影后坐标点
}

void MyOpenGLWidget::drawFeature(Feature * feature, SymbolStyle* symbolStyle)
{
	mgeo::Geometry *geometry = feature->geometry;
	SymbolStyle drawSymbol;
	if (symbolStyle != nullptr) {
		drawSymbol = *symbolStyle;
	}
	else {
		drawSymbol = feature->symbolStyle;
	}
	//开始画画
	if (GeometryType::GEOPOINT == geometry->getGeometryType()) {
		drawPoint(geometry, drawSymbol);
	}
	else if (GeometryType::GEOPOLYLINE == geometry->getGeometryType()) {
		drawPolyline(geometry, drawSymbol);
	}
	else if (GeometryType::GEOPOLYGON == geometry->getGeometryType()) {
		drawPolygon(geometry, drawSymbol);
	}
	else if (GeometryType::GEOMULTIPOLYGON == geometry->getGeometryType()) {
		drawMultiPolygon(geometry, drawSymbol);
	}
	else {
		//其他情况，暂不实现
	}
}

void MyOpenGLWidget::drawPoint(mgeo::Geometry *geometry, SymbolStyle symbolStyle)
{
	//规范化symbol
	vector<double> normalResult = normalizeSymbol(symbolStyle);
	GLfloat normalFillRed, normalFillGreen, normalFillBlue, normalStrokeRed, normalStrokeGreen, normalStrokeBlue;
	normalFillRed = normalResult[0];
	normalFillGreen = normalResult[1];
	normalFillBlue = normalResult[2];
	normalStrokeRed = normalResult[3];
	normalStrokeGreen = normalResult[4];
	normalStrokeBlue = normalResult[5];

	//点投影
	GeoPoint * point = (GeoPoint *)geometry;
	if (geoMap->mapPrj != NULL) {
		//如果是第一次投影就运算一次
		if (!point->isProjeted) {
			double prjx, prjy;
			geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
			point->prjx = prjx;
			point->prjy = prjy;
			//投影完成
			point->isProjeted = true;
		}
	}
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 0.0);
	//获取点坐标
	glVertex2f(point->getX(), point->getY());
	glEnd();
}

void MyOpenGLWidget::drawPolyline(mgeo::Geometry *geometry, SymbolStyle symbolStyle)
{
	//规范化symbol
	vector<double> normalResult = normalizeSymbol(symbolStyle);
	GLfloat normalFillRed, normalFillGreen, normalFillBlue, normalStrokeRed, normalStrokeGreen, normalStrokeBlue;
	normalFillRed = normalResult[0];
	normalFillGreen = normalResult[1];
	normalFillBlue = normalResult[2];
	normalStrokeRed = normalResult[3];
	normalStrokeGreen = normalResult[4];
	normalStrokeBlue = normalResult[5];
	//线投影
	GeoPolyline * polyLine = (GeoPolyline *)geometry;
	if (geoMap->mapPrj != NULL) {
		//如果是第一次投影就运算一次
		if (!geometry->isProjeted) {
			for (int j = 0; j < polyLine->points.size(); j++) {
				GeoPoint *point = polyLine->points[j];
				double prjx, prjy;
				geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
				point->prjx = prjx;
				point->prjy = prjy;
				point->isProjeted = true;
			}
			//投影完成
			geometry->isProjeted = true;
		}
	}

	//线绘制
	glLineWidth(symbolStyle.strokeWidth);//线宽
	//glLineStipple(1, 0xFFFF);  //点绘制实线
	glBegin(GL_LINE_STRIP);
	GeoPolyline *polyline = (GeoPolyline *)geometry;
	for (int j = 0; j < polyline->points.size(); j++) {
		GeoPoint *point = polyline->points[j];
		glColor3f(normalStrokeRed, normalStrokeGreen, normalStrokeBlue);
		//最后一个元素时改变isLastPt
		glVertex2f(point->getX(), point->getY());
	}
	glEnd();
}

void MyOpenGLWidget::drawPolygon(mgeo::Geometry *geometry, SymbolStyle symbolStyle)
{
	//规范化symbol
	vector<double> normalResult = normalizeSymbol(symbolStyle);
	GLfloat normalFillRed, normalFillGreen, normalFillBlue, normalStrokeRed, normalStrokeGreen, normalStrokeBlue;
	normalFillRed = normalResult[0];
	normalFillGreen = normalResult[1];
	normalFillBlue = normalResult[2];
	normalStrokeRed = normalResult[3];
	normalStrokeGreen = normalResult[4];
	normalStrokeBlue = normalResult[5];

	//面绘制
	GeoPolygon *polygon = (GeoPolygon *)geometry;
	//开始投影
	if (geoMap->mapPrj != NULL) {
		//如果是第一次投影就运算一次
		if (!geometry->isProjeted) {
			for (int j = 0; j < polygon->points.size(); j++) {
				GeoPoint *point = polygon->points[j];
				double prjx, prjy;
				geoMap->mapPrj->getXY(point->x, point->y, &prjx, &prjy);
				point->prjx = prjx;
				point->prjy = prjy;
				point->isProjeted = true;
			}
			//投影完成
			geometry->isProjeted = true;
		}
	}
	//开始三角剖分
	if (!polygon->isConvex()) {
		//不是凸多边形,开始剖分
		vector<GeoPolygon *> triangles = polygon->getTriangles();
		// 设置正面为填充模式
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < triangles.size(); j++) {
			for (int k = 0; k < triangles[j]->points.size(); k++) {
				GeoPoint *point = triangles[j]->points[k];
				glColor3f(normalFillRed, normalFillGreen, normalFillBlue);
				glVertex2f(point->getX(), point->getY());
			}
		}
		glEnd();
	}
	else {
		glBegin(GL_POLYGON);
		for (int j = 0; j < polygon->points.size(); j++) {
			GeoPoint *point = polygon->points[j];
			glColor3f(normalFillRed, normalFillGreen, normalFillBlue);
			glVertex2f(point->getX(), point->getY());
		}
		glEnd();
	}
	//描绘多边形边界
	glLineWidth(symbolStyle.strokeWidth);
	//glLineStipple(1, 0xFFFF);  //点绘制实线
	glBegin(GL_LINE_STRIP);
	for (int j = 0; j < polygon->points.size(); j++) {
		GeoPoint *point = polygon->points[j];
		glColor3f(normalStrokeRed, normalStrokeGreen, normalStrokeBlue);
		glVertex2f(point->getX(), point->getY());
	}
	glEnd();

}

void MyOpenGLWidget::drawMultiPolygon(mgeo::Geometry *geometry, SymbolStyle symbolStyle)
{
	//多面绘制
	GeoMultiPolygon* multiPly = (GeoMultiPolygon*)geometry;
	for (int m = 0; m < multiPly->polygons.size(); m++)
	{
		drawPolygon(multiPly->polygons[m], symbolStyle);
	}
}

vector<double> MyOpenGLWidget::normalizeSymbol(SymbolStyle symbolStyle)
{
	float maxColorComponent = 255.0;
	GLfloat normalFillRed, normalFillGreen, normalFillBlue, normalStrokeRed, normalStrokeGreen, normalStrokeBlue;
	//规范化fill颜色
	if (symbolStyle.fillColor.isValid()) {//判断fill颜色是否储存
		normalFillRed = symbolStyle.fillColor.red() / maxColorComponent;
		normalFillGreen = symbolStyle.fillColor.green() / maxColorComponent;
		normalFillBlue = symbolStyle.fillColor.blue() / maxColorComponent;
	}
	else {
		normalFillRed = 1.0;
		normalFillGreen = 0.0;
		normalFillBlue = 0.0;
	}

	//规范化stroke颜色
	if (symbolStyle.strokeColor.isValid()) {
		normalStrokeRed = symbolStyle.strokeColor.red() / maxColorComponent;
		normalStrokeGreen = symbolStyle.strokeColor.green() / maxColorComponent;
		normalStrokeBlue = symbolStyle.strokeColor.blue() / maxColorComponent;
	}
	else {
		normalStrokeRed = 0.0;
		normalStrokeGreen = 1.0;
		normalStrokeBlue = 1.0;
	}
	//结果
	vector<double> normalResult;
	normalResult.push_back(normalFillRed);
	normalResult.push_back(normalFillGreen);
	normalResult.push_back(normalFillBlue);
	normalResult.push_back(normalStrokeRed);
	normalResult.push_back(normalStrokeGreen);
	normalResult.push_back(normalStrokeBlue);
	return normalResult;
}

void MyOpenGLWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		bool isFind = searchByClick(event->pos());
		if (!isFind) {
			//把所有要素的选中状态全部清空
			for (int i = 0; i < selectedFeature.size(); i++) {
				selectedFeature[i]->isSelected = false;
			}
			selectedFeature.clear();
			update();
		}
	}
}

bool MyOpenGLWidget::searchByClick(QPoint screenPoint)
{
	//使用空间索引搜索要素
	if (geoMap->index != NULL && geoMap->index->isIndexCreated) {
		if (geoMap->index->getIndexType() == SpatialIndexType::GRID) {
			GridIndex* gridIndex = (GridIndex*)geoMap->index;
			geos::io::WKTReader wktReader;
			QPointF worldPoint = screenCd2worldCd(screenPoint);
			double b, l;
			if (geoMap->mapPrj != NULL) {
				geoMap->mapPrj->getBL(worldPoint.x(), worldPoint.y(), &l, &b);
			}
			else {
				b = worldPoint.x();
				l = worldPoint.y();
			}
			Geometry* mousePoint = wktReader.read("Point (" + QString::number(l).toStdString() + " " + QString::number(b).toStdString() + ")");
			for (int i = 0; i < gridIndex->grids.size(); i++) {
				Grid* grid = gridIndex->grids[i];
				if (!grid->geosBound->disjoint(mousePoint)) {
					//判断grid中是否有要素和其相交
					for (int j = 0; j < grid->pfeatures.size(); j++) {
						Feature * feature = grid->pfeatures[j];
						if (!feature->geosGeom->disjoint(mousePoint)) {
							//不分离即相交
							feature->isSelected = true;
							selectedFeature.push_back(feature);
							update();
							return true;
						}
					}
				}
			}
		}
		else if (geoMap->index->getIndexType() == SpatialIndexType::QUADTREE) {
			QuadTreeIndex* quadIndex = (QuadTreeIndex*)geoMap->index;
			geos::io::WKTReader wktReader;
			QPointF worldPoint = screenCd2worldCd(screenPoint);
			double b, l;
			if (geoMap->mapPrj != NULL) {
				geoMap->mapPrj->getBL(worldPoint.x(), worldPoint.y(), &l, &b);
			}
			else {
				b = worldPoint.x();
				l = worldPoint.y();
			}
			Geometry* mousePoint = wktReader.read("Point (" + QString::number(l).toStdString() + " " + QString::number(b).toStdString() + ")");
			searchByQuad(mousePoint, quadIndex->quadTree->root);
		}
	}
	return false;
}

void MyOpenGLWidget::searchByQuad(Geometry* worldPoint, QuadNode* node)
{
	if (!node->geosBound->disjoint(worldPoint)) {
		//相交后
		if (!node->isLeaf()) {
			//不是叶节点
			for (int i = 0; i < 4; i++) {
				searchByQuad(worldPoint, node->childNodes[i]);
			}
		}
		else {
			//是叶节点
			for (int j = 0; j < node->pfeatures.size(); j++) {
				Feature * feature = node->pfeatures[j];
				if (!feature->geosGeom->disjoint(worldPoint)) {
					//不分离即相交
					feature->isSelected = true;
					selectedFeature.push_back(feature);
					update();
					return;
				}
			}
		}
	}
}

void MyOpenGLWidget::setStyleByProperties(Layer * layer, QString propertyName)
{
	vector<double> propertyList;
	double min = std::numeric_limits<float>::max();
	double max = std::numeric_limits<float>::lowest();
	//分层设色
	for (int i = 0; i < layer->features.size(); i++) {
		double num = layer->features[i]->properties[propertyName].toDouble();
		propertyList.push_back(num);
		//存入最大和最小值
		if (num > max) {
			max = num;
		}
		if (num < min) {
			min = num;
		}
	}
	//归一化0到1
	for (int i = 0; i < propertyList.size(); i++) {
		propertyList[i] = (propertyList[i] - min) / (max - min);
		double ratio = 1.0 / colorList.size();
		int colorIndex = propertyList[i] / ratio;
		if (colorIndex == colorList.size()) {
			colorIndex--;
		}
		layer->features[i]->symbolStyle.fillColor = colorList[colorIndex];
		layer->features[i]->symbolStyle.strokeColor = colorList[colorIndex];
	}
	update();
}


QPointF MyOpenGLWidget::screenCd2normalCd(double x, double y)
{
	return QPointF((2*x-this->width)/this->width, (this->height-2*y)/this->height);
}

QPointF MyOpenGLWidget::normalCd2worldCd(double x, double y)
{
	double dx = viewRange.width();
	double dy = viewRange.height();
	QPointF p = viewRange.center();
	x = (dx * x) / 2 + viewRange.center().x();
	y = (dy * y) / 2 + viewRange.center().y();
	return QPointF(x, y);

}

QPointF MyOpenGLWidget::screenCd2worldCd(QPointF screenPoint)
{
	//反解标准坐标
	QPointF p = screenCd2normalCd(screenPoint.x(), screenPoint.y());
	double x = p.x();
	double y = p.y();
	// 这里的标准化坐标是上面-1，下面+1
	return normalCd2worldCd(x, y);
}

//绘制索引
void MyOpenGLWidget::drawIndex()
{
	// TODO: 在此处添加实现代码.
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
				glBegin(GL_LINE_LOOP);
				glColor3f(1, 1, 1);
				glVertex2f(gridBound.left(), gridBound.top());
				glVertex2f(gridBound.right(), gridBound.top());
				glVertex2f(gridBound.right(), gridBound.bottom());
				glVertex2f(gridBound.left(), gridBound.bottom());
				glEnd();

			}
		}
		else if(SpatialIndexType::QUADTREE == geoMap->index->getIndexType())
		{
			drawQuadTreeIndexNode(((QuadTreeIndex*)geoMap->index)->quadTree->root);
		}
	}
}


void MyOpenGLWidget::drawQuadTreeIndexNode(QuadNode* node)
{
	// TODO: 在此处添加实现代码.
	if (true == node->isLeaf())
	{
		//叶节点绘制矩形框
		QRectF gridBound = node->box;
		if (geoMap->mapPrj != NULL)
			gridBound = geoMap->mapPrj->getPrjRange(gridBound);
		glBegin(GL_LINE_LOOP);
		glColor3f(1, 1, 1);
		glVertex2f(gridBound.left(), gridBound.top());
		glVertex2f(gridBound.right(), gridBound.top());
		glVertex2f(gridBound.right(), gridBound.bottom());
		glVertex2f(gridBound.left(), gridBound.bottom());
		glEnd();
		/*qDebug() << "depth:" << node->depth;
		for (int i = 0; i < node->pfeatures.size(); i++)
		{
			qDebug() << "     objname:" << node->pfeatures[i]->attributes.value("NAME");
		}*/	
	}
	else
	{
		//非叶结点遍历其子节点
		for (int i = 0; i < 4; i++)
		{
			drawQuadTreeIndexNode(node->childNodes[i]);
		}
	}
}
