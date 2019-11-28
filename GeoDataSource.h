#pragma once
#include <vector>
#include "GeoMap.h"
using namespace std;
class GeoDataSource
{
public:
	GeoDataSource(void);
	~GeoDataSource(void);
	vector<GeoMap *> geoMaps;
};

