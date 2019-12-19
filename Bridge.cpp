#include "Bridge.h"
static Bridge s_obj;

Bridge::Bridge(QObject *parent)
	: QObject(parent)
{
}

Bridge::~Bridge()
{
}

Bridge* Bridge::instance()
{
	return &s_obj;
}
void Bridge::showMsgBox() {
	m_webView->close();
	GeoJsonParese *w = new GeoJsonParese;
	w->show();
}
