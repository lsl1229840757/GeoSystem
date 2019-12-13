#pragma once
#include <qaction.h>
class MyAction :
	public QAction
{
	Q_OBJECT
public:
	int mapIndex;
	int layerIndex;
	MyAction(int mapIndex, int layerIndex, const QString &text, QObject *parent);
	~MyAction();
signals:
	void sendIndex(int mapIndex, int layerIndex);
public slots:
	void mtriggle();
};
