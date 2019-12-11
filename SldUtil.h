#pragma once
#include <QtCore\qxmlstream.h>
#include <qfile.h>
#include <Layer.h>
#include <SymbolStyle.h>
#include <qdebug.h>
#include <qdom.h>
#include <qxml.h>
using namespace std;
class SldUtil
{
public:
	SldUtil();
	~SldUtil();
	//QXmlStreamReader�Ҳ��������Ը���DOM
	//���ǵ�dom���ڴ����ģ�����һ��xml�ĵ�ָ��
	static QDomDocument* sldRead(QString filePath);
	static void parseSldDom(QDomDocument* xmlDoc,Layer* layer);
	//static QXmlStreamReader xmlReader;
private:
	static void parseElement(QDomElement docElem, Layer* layer,bool ruleFlag);
public:
	static SymbolStyle* parseStyleList(QDomNodeList* styleList);
private:
	static vector<Feature*>* parseFilterList(QDomNodeList* filterList,Layer* layer);
public:
	static void parseSldDomFromName(QDomDocument* sldDoc, Layer* layer, QString lyname);
};

