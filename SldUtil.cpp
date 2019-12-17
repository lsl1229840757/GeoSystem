#include "SldUtil.h"



SldUtil::SldUtil()
{
}


SldUtil::~SldUtil()
{
}


QDomDocument* SldUtil::sldRead(QString filePath)
{
	// TODO: 在此处添加实现代码.
	QDomDocument* doc = new QDomDocument;
	QFile loadFile(filePath);
	if (!loadFile.open(QIODevice::ReadOnly|QIODevice::Text))
	{
		qDebug()<<"could't open this SLD file"<<endl;
		//QMessageBox::warning(NULL, QString("Error"), QString(r1.what()));
		return nullptr;
	}
	QString errStr;
	int errLine, errCol;
	//指定文档内容
	if (!doc->setContent(&loadFile, true, &errStr, &errLine, &errCol))
	{
		//打开失败的原因
		qDebug() << errStr << "Line:" << errLine << "Col:" << errCol << endl;
		loadFile.close();
		return nullptr;
	}
	//xmlReader.setDevice(&loadFile);
	//parseSldXML(layer);
	loadFile.close();
	return doc;
}


void SldUtil::parseSldDom(QDomDocument* sldDoc, Layer* layer)
{
	// TODO: 在此处添加实现代码.
	if (sldDoc != NULL)
	{
		//获取元素
		QDomElement docElem = sldDoc->documentElement();//根节点

		bool ruleFlag0 = false; //判断是否进入Rule标签
		parseElement(docElem, layer, ruleFlag0);
	}
	else
	{
		qDebug() << "Could not parse the DOM";
		return;
	}

	//nodeName是标签的全名，tagName是标签sld:之后的名称，qPrintable()去除引号
	//qDebug() << docElem.nodeName() << endl<<docElem.tagName()<<endl;
	/*
	//解析XML直到结束
	while (!xmlReader.atEnd())
	{
		//读取下一个元素，获得类型
		QXmlStreamReader::TokenType nType = xmlReader.readNext();
		if (nType == QXmlStreamReader::StartElement) //开始元素标签 isStartElement()
		{
			//标签名称
			QString startElementName = xmlReader.name().toString();
			
		}
	}
	*/
}


void SldUtil::parseElement(QDomElement docElem, Layer* layer,bool ruleFlag)
{
	// TODO: 在此处添加实现代码.

	//利用递归，可读取各种内容的SLD
	if (!docElem.hasChildNodes())
	{
		qDebug() << "no child" << endl;
		return;
	}
	else
	{
		//获取元素所有子节点链表
		QDomNodeList childNodeList = docElem.childNodes();
		SymbolStyle* tempstyle = NULL;
		vector<Feature*>* pselectFeatures = NULL;  //存储filter选出的feature
		bool filterFlag = false;//判断是否存在filter
		for (int i = 0; i < childNodeList.size(); i++)
		{
			QDomNode childNode = childNodeList.at(i);
			if (childNode.isElement())
			{	//如果子节点为元素
				QDomElement childElem = childNode.toElement();
				qDebug() << qPrintable(childElem.tagName())<<endl;
				if (childElem.tagName().compare("Rule")==0)
				{
					//递归调用
					parseElement(childElem, layer, true);  //进入rule时更换flag
				}
				else if ((childElem.tagName().compare("Filter")==0)&&ruleFlag==true)
				{
					//手动解析Filter，选择出符合条件的feature
					//未实现
					qDebug() << "Filter" << endl;
					filterFlag = true;
					QDomNodeList filterList = childElem.childNodes();
					//读取filter内容,筛选feature
					pselectFeatures = parseFilterList(&filterList, layer);
				}
				else if ((childElem.tagName().compare("PolygonSymbolizer") == 0) && ruleFlag == true)
				{
					//暂时先解析面状
					qDebug() << "Polygon" << endl;
					//获取子节点
					QDomNodeList styleList = childElem.childNodes();
					//tempstyle = new SymbolStyle;
					//读取所有style
					tempstyle = parseStyleList(&styleList);
					//记录style
					if (true == filterFlag)
					{
						filterFlag = false;
						//按选择的feature赋SymbolStyle
						if (!pselectFeatures->empty())
						{
							for (int k = 0; k < pselectFeatures->size(); k++)
							{
								pselectFeatures->at(k)->symbolStyle = *tempstyle;
							}
						}
					}
					else
					{
						//所有feature赋同一个style
						int feasize = layer->features.size();
						for (int k = 0; k < feasize; k++)
						{
							layer->features.at(k)->symbolStyle = *tempstyle;
						}
					}
				}
				else if ((childElem.tagName().compare("LineSymbolizer") == 0) && ruleFlag == true)
				{
					qDebug() << "Polyline" << endl;
					//获取子节点
					QDomNodeList styleList = childElem.childNodes();
					//tempstyle = new SymbolStyle;
					//读取所有style
					tempstyle = parseStyleList(&styleList);
					//记录style
					if (true == filterFlag)
					{
						filterFlag = false;
						//按选择的feature赋SymbolStyle
						if (!pselectFeatures->empty())
						{
							for (int k = 0; k < pselectFeatures->size(); k++)
							{
								pselectFeatures->at(k)->symbolStyle = *tempstyle;
							}
						}
					}
					else
					{
						//所有feature赋同一个style
						int feasize = layer->features.size();
						for (int k = 0; k < feasize; k++)
						{
							layer->features.at(k)->symbolStyle = *tempstyle;
						}
					}
				}
				else if ((childElem.tagName().compare("PointSymbolizer") == 0) && ruleFlag == true)
				{
					qDebug() << "Point" << endl;
					//获取子节点
					QDomNodeList ptSymbolList = childElem.childNodes();  //Graphic
					//
					if (true == filterFlag)
					{
						filterFlag = false;
					}
				}
				else
				{
					//递归调用
					parseElement(childElem, layer, ruleFlag);
				}
			}
			else
			{
				if (childNode.isText())
				{
					QDomText text = childNode.toText();
					qDebug() << "text" << endl;//qPrintable(text);
				}
				return;
			}
		}
	}
	
}


SymbolStyle* SldUtil::parseStyleList(QDomNodeList* styleList)
{
	// TODO: 在此处添加实现代码.
	SymbolStyle* tempstyle = new SymbolStyle;
	for (int j = 0; j < styleList->size(); j++)
	{
		QDomNode style = styleList->at(j);
		if (style.isElement())
		{
			QDomElement styleElem = style.toElement();
			//QDomElement fstChildElem = styleElem.firstChildElement;
			QDomNodeList styleChildList = styleElem.childNodes();
			//读取style
			if (styleElem.tagName().compare("Fill") == 0)
			{
				qDebug() << "Fill";
				//读取面状填充数据
				for (int k = 0; k < styleChildList.size(); k++)
				{
					if (styleChildList.at(k).toElement().attribute("name") == "fill")
					{
						qDebug() << qPrintable(styleChildList.at(k).toElement().text());
						tempstyle->fillColor = QColor(styleChildList.at(k).toElement().text());
					}
					else if (styleChildList.at(k).toElement().attribute("name") == "fill-opacity")
					{
						qDebug() << qPrintable(styleChildList.at(k).toElement().text());
						tempstyle->fillOpacity = styleChildList.at(k).toElement().text().toFloat();
					}
				}
			}
			else if (styleElem.tagName().compare("Stroke") == 0)
			{
				qDebug() << "Stroke";
				//读取边界信息
				for (int k = 0; k < styleChildList.size(); k++)
				{
					if (styleChildList.at(k).toElement().attribute("name") == "stroke")
					{
						qDebug() << qPrintable(styleChildList.at(k).toElement().text());
						tempstyle->strokeColor = QColor(styleChildList.at(k).toElement().text());
					}
					else if (styleChildList.at(k).toElement().attribute("name") == "stroke-opacity")
					{
						qDebug() << qPrintable(styleChildList.at(k).toElement().text());
						tempstyle->strokeOpacity = styleChildList.at(k).toElement().text().toFloat();
					}
					else if (styleChildList.at(k).toElement().attribute("name") == "stroke-width")
					{
						qDebug() << qPrintable(styleChildList.at(k).toElement().text());
						tempstyle->strokeWidth = styleChildList.at(k).toElement().text().toFloat();
					}
				}
			}
		}
		else
		{
			continue;
		}
	}
	return tempstyle;
}


vector<Feature*>* SldUtil::parseFilterList(QDomNodeList* filterList,Layer* layer)
{
	// TODO: 在此处添加实现代码.
	vector<Feature*>* pselectFeatures = new vector<Feature*>;
	for (int j = 0; j < filterList->size(); j++)
	{
		if (j != 0)continue;//一个filter只能有一种property规则
		QDomNode filter = filterList->at(j);
		if (filter.isElement())
		{
			QDomElement filterElem = filter.toElement();  //PropertyIsBetween etc.
			QDomNodeList filterChildList = filterElem.childNodes();
			if (filterElem.tagName() == "PropertyIsBetween")
			{
				if (filterElem.firstChild().toElement().tagName() == "PropertyName")
				{
					//获取属性名
					QString propertyName = filterElem.firstChild().toElement().text();
					QString lowerpropt, upperpropt;//属性上下界
					QDomNode lowerNode = filterElem.childNodes().at(1);
					QDomNode upperNode = filterElem.childNodes().at(2);
					if (lowerNode.toElement().tagName() == "LowerBoundary") {
						lowerpropt = lowerNode.firstChild().toElement().text();
					}
					if (upperNode.toElement().tagName() == "UpperBoundary") {
						upperpropt = upperNode.firstChild().toElement().text();
					}
					//按属性类型转换上下界，判断符合条件的feature
					if (!layer->features.empty()) {
						QVariant::Type type = layer->features.back()->attributes.value(propertyName).type();
						for (int k=0; k < layer->features.size(); k++)
						{
							//遍历图层layer
							if (QVariant::Type::Int == type ) {
								int propt = layer->features.at(k)->attributes.value(propertyName).toInt();
								//判断是否位于范围内
								if (propt >= lowerpropt.toInt() && propt <= upperpropt.toInt()) {
									pselectFeatures->push_back(layer->features.at(k));
								}
							}else if (QVariant::Type::Double == type) {
								double propt = layer->features.at(k)->attributes.value(propertyName).toReal();
								//判断是否位于范围内
								if (propt >= lowerpropt.toDouble() && propt <= upperpropt.toDouble()) {
									pselectFeatures->push_back(layer->features.at(k));
								}
							}
						}
					}					
				}
			}
			else if (filterElem.tagName() == "PropertyIsEqualTo")
			{
				if (filterElem.firstChild().toElement().tagName() == "PropertyName")
				{
					//获取属性名
					QString propertyName = filterElem.firstChild().toElement().text();
					QString equalpropt;//相等的属性
					QDomNode equalLiteralNode = filterElem.lastChild();
					if (equalLiteralNode.toElement().tagName() == "Literal") {
						equalpropt = equalLiteralNode.toElement().text();
					}
					//按属性类型转换值，判断符合条件的feature
					if (!layer->features.empty()) {
						QVariant::Type type = layer->features.back()->attributes.value(propertyName).type();
						for (int k=0; k < layer->features.size(); k++)
						{
							//遍历图层layer
							if (QVariant::Type::Int == type) {
								int propt = layer->features.at(k)->attributes.value(propertyName).toInt();
								//判断是否相等
								if (equalpropt.toInt()==propt) {
									pselectFeatures->push_back(layer->features.at(k));
								}
							}
							else if (QVariant::Type::Double == type) {
								//除了ARM平台(嵌入式)相当于float，QReal在windows上相当于double
								double propt = layer->features.at(k)->attributes.value(propertyName).toReal();
								//判断double是否相等
								if (fabs(fabs(propt)-fabs(equalpropt.toDouble()))< 1e-15) {
									pselectFeatures->push_back(layer->features.at(k));
								}
							}
						}
					}
				}
			}
			else
			{

			}
		}
	}
	return pselectFeatures;
}


void SldUtil::parseSldDomFromName(QDomDocument* sldDoc, Layer* layer, QString lyname)
{
	// TODO: 在此处添加实现代码.
	if (sldDoc != NULL)
	{
		//获取元素
		QDomElement docElem = sldDoc->documentElement();//根节点
		if (docElem.hasChildNodes())
		{
			QDomNodeList rootChilds = docElem.childNodes();
			for (int i = 0; i < rootChilds.size(); i++)
			{
				QString elemtxt = rootChilds.at(i).firstChild().toElement().text();
				//判断是否当前图层是否有对应的style
				if (elemtxt == lyname)
				{
					bool ruleFlag0 = false; //判断是否进入Rule标签
					parseElement(rootChilds.at(i).toElement(), layer, ruleFlag0);
				}
				else continue;
			}
		}
		
	}
	else
	{
		qDebug() << "Could not parse the DOM";
		return;
	}
}
