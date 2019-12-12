#include "MyAction.h"



MyAction::MyAction(int mapIndex, int layerIndex, const QString &text, QObject *parent= nullptr):QAction(text, parent)
{
	this->mapIndex = mapIndex;
	this->layerIndex = layerIndex;
}


MyAction::~MyAction()
{

}
void MyAction::mtriggle()
{
	emit sendIndex(this->mapIndex, this->layerIndex);
}
