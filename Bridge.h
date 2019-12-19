#pragma once

#include <QObject>
#include <qmessagebox.h>
#include "geojsonparese.h"
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebChannel/qwebchannel.h>
class Bridge : public QObject
{
	Q_OBJECT

public:
	QWebEngineView* m_webView;
	static Bridge* instance();
	Bridge(QObject *parent = 0);
	~Bridge();
public slots:
	void showMsgBox();
};
