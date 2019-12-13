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
	bool isIndexCreated;  //索引是否创建
	virtual bool createIndex() = 0;
	QRectF mapRange;
	virtual SpatialIndexType getIndexType() = 0;//获取索引类型
	virtual void addAllObjID(Layer *layer) =0;//添加索引目标
};

