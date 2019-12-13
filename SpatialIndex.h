#pragma once
#include <vector>
#include "Grid.h"
#include"Layer.h"
typedef enum {
	GRID,QUADTREE
}SpatialIndexType;
class SpatialIndex
{
public:
	SpatialIndex();
	virtual ~SpatialIndex();
	bool isIndexCreated;  //�����Ƿ񴴽�
	virtual bool createIndex() = 0;
	QRectF mapRange;
	virtual SpatialIndexType getIndexType() = 0;//��ȡ��������
	virtual void addAllObjID(Layer *layer) =0;//�������Ŀ��
};

