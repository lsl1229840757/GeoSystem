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
	//QXmlStreamReader找不到，所以改用DOM
	//考虑到dom的内存消耗，返回一个xml文档指针
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

