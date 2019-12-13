#include "GeoDataSource.h"


GeoDataSource::GeoDataSource(void)
{

}


GeoDataSource::~GeoDataSource(void)
{

}

GeoMap * GeoDataSource::getGeoMapByName(QString name)
{
	for (int i = 0; i < geoMaps.size(); i++) {
		if (name.compare(QString::fromStdString(geoMaps[i]->name))==0) {
			return geoMaps[i];
		}
	}
	return nullptr;
}
