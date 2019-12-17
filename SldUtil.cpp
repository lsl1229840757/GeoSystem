#include "SldUtil.h"



SldUtil::SldUtil()
{
}


SldUtil::~SldUtil()
{
}


QDomDocument* SldUtil::sldRead(QString filePath)
{
	// TODO: �ڴ˴����ʵ�ִ���.
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
	//ָ���ĵ�����
	if (!doc->setContent(&loadFile, true, &errStr, &errLine, &errCol))
	{
		//��ʧ�ܵ�ԭ��
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
	// TODO: �ڴ˴����ʵ�ִ���.
	if (sldDoc != NULL)
	{
		//��ȡԪ��
		QDomElement docElem = sldDoc->documentElement();//���ڵ�

		bool ruleFlag0 = false; //�ж��Ƿ����Rule��ǩ
		parseElement(docElem, layer, ruleFlag0);
	}
	else
	{
		qDebug() << "Could not parse the DOM";
		return;
	}

	//nodeName�Ǳ�ǩ��ȫ����tagName�Ǳ�ǩsld:֮������ƣ�qPrintable()ȥ������
	//qDebug() << docElem.nodeName() << endl<<docElem.tagName()<<endl;
	/*
	//����XMLֱ������
	while (!xmlReader.atEnd())
	{
		//��ȡ��һ��Ԫ�أ��������
		QXmlStreamReader::TokenType nType = xmlReader.readNext();
		if (nType == QXmlStreamReader::StartElement) //��ʼԪ�ر�ǩ isStartElement()
		{
			//��ǩ����
			QString startElementName = xmlReader.name().toString();
			
		}
	}
	*/
}


void SldUtil::parseElement(QDomElement docElem, Layer* layer,bool ruleFlag)
{
	// TODO: �ڴ˴����ʵ�ִ���.

	//���õݹ飬�ɶ�ȡ�������ݵ�SLD
	if (!docElem.hasChildNodes())
	{
		qDebug() << "no child" << endl;
		return;
	}
	else
	{
		//��ȡԪ�������ӽڵ�����
		QDomNodeList childNodeList = docElem.childNodes();
		SymbolStyle* tempstyle = NULL;
		vector<Feature*>* pselectFeatures = NULL;  //�洢filterѡ����feature
		bool filterFlag = false;//�ж��Ƿ����filter
		for (int i = 0; i < childNodeList.size(); i++)
		{
			QDomNode childNode = childNodeList.at(i);
			if (childNode.isElement())
			{	//����ӽڵ�ΪԪ��
				QDomElement childElem = childNode.toElement();
				qDebug() << qPrintable(childElem.tagName())<<endl;
				if (childElem.tagName().compare("Rule")==0)
				{
					//�ݹ����
					parseElement(childElem, layer, true);  //����ruleʱ����flag
				}
				else if ((childElem.tagName().compare("Filter")==0)&&ruleFlag==true)
				{
					//�ֶ�����Filter��ѡ�������������feature
					//δʵ��
					qDebug() << "Filter" << endl;
					filterFlag = true;
					QDomNodeList filterList = childElem.childNodes();
					//��ȡfilter����,ɸѡfeature
					pselectFeatures = parseFilterList(&filterList, layer);
				}
				else if ((childElem.tagName().compare("PolygonSymbolizer") == 0) && ruleFlag == true)
				{
					//��ʱ�Ƚ�����״
					qDebug() << "Polygon" << endl;
					//��ȡ�ӽڵ�
					QDomNodeList styleList = childElem.childNodes();
					//tempstyle = new SymbolStyle;
					//��ȡ����style
					tempstyle = parseStyleList(&styleList);
					//��¼style
					if (true == filterFlag)
					{
						filterFlag = false;
						//��ѡ���feature��SymbolStyle
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
						//����feature��ͬһ��style
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
					//��ȡ�ӽڵ�
					QDomNodeList styleList = childElem.childNodes();
					//tempstyle = new SymbolStyle;
					//��ȡ����style
					tempstyle = parseStyleList(&styleList);
					//��¼style
					if (true == filterFlag)
					{
						filterFlag = false;
						//��ѡ���feature��SymbolStyle
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
						//����feature��ͬһ��style
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
					//��ȡ�ӽڵ�
					QDomNodeList ptSymbolList = childElem.childNodes();  //Graphic
					//
					if (true == filterFlag)
					{
						filterFlag = false;
					}
				}
				else
				{
					//�ݹ����
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
	// TODO: �ڴ˴����ʵ�ִ���.
	SymbolStyle* tempstyle = new SymbolStyle;
	for (int j = 0; j < styleList->size(); j++)
	{
		QDomNode style = styleList->at(j);
		if (style.isElement())
		{
			QDomElement styleElem = style.toElement();
			//QDomElement fstChildElem = styleElem.firstChildElement;
			QDomNodeList styleChildList = styleElem.childNodes();
			//��ȡstyle
			if (styleElem.tagName().compare("Fill") == 0)
			{
				qDebug() << "Fill";
				//��ȡ��״�������
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
				//��ȡ�߽���Ϣ
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
	// TODO: �ڴ˴����ʵ�ִ���.
	vector<Feature*>* pselectFeatures = new vector<Feature*>;
	for (int j = 0; j < filterList->size(); j++)
	{
		if (j != 0)continue;//һ��filterֻ����һ��property����
		QDomNode filter = filterList->at(j);
		if (filter.isElement())
		{
			QDomElement filterElem = filter.toElement();  //PropertyIsBetween etc.
			QDomNodeList filterChildList = filterElem.childNodes();
			if (filterElem.tagName() == "PropertyIsBetween")
			{
				if (filterElem.firstChild().toElement().tagName() == "PropertyName")
				{
					//��ȡ������
					QString propertyName = filterElem.firstChild().toElement().text();
					QString lowerpropt, upperpropt;//�������½�
					QDomNode lowerNode = filterElem.childNodes().at(1);
					QDomNode upperNode = filterElem.childNodes().at(2);
					if (lowerNode.toElement().tagName() == "LowerBoundary") {
						lowerpropt = lowerNode.firstChild().toElement().text();
					}
					if (upperNode.toElement().tagName() == "UpperBoundary") {
						upperpropt = upperNode.firstChild().toElement().text();
					}
					//����������ת�����½磬�жϷ���������feature
					if (!layer->features.empty()) {
						QVariant::Type type = layer->features.back()->attributes.value(propertyName).type();
						for (int k=0; k < layer->features.size(); k++)
						{
							//����ͼ��layer
							if (QVariant::Type::Int == type ) {
								int propt = layer->features.at(k)->attributes.value(propertyName).toInt();
								//�ж��Ƿ�λ�ڷ�Χ��
								if (propt >= lowerpropt.toInt() && propt <= upperpropt.toInt()) {
									pselectFeatures->push_back(layer->features.at(k));
								}
							}else if (QVariant::Type::Double == type) {
								double propt = layer->features.at(k)->attributes.value(propertyName).toReal();
								//�ж��Ƿ�λ�ڷ�Χ��
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
					//��ȡ������
					QString propertyName = filterElem.firstChild().toElement().text();
					QString equalpropt;//��ȵ�����
					QDomNode equalLiteralNode = filterElem.lastChild();
					if (equalLiteralNode.toElement().tagName() == "Literal") {
						equalpropt = equalLiteralNode.toElement().text();
					}
					//����������ת��ֵ���жϷ���������feature
					if (!layer->features.empty()) {
						QVariant::Type type = layer->features.back()->attributes.value(propertyName).type();
						for (int k=0; k < layer->features.size(); k++)
						{
							//����ͼ��layer
							if (QVariant::Type::Int == type) {
								int propt = layer->features.at(k)->attributes.value(propertyName).toInt();
								//�ж��Ƿ����
								if (equalpropt.toInt()==propt) {
									pselectFeatures->push_back(layer->features.at(k));
								}
							}
							else if (QVariant::Type::Double == type) {
								//����ARMƽ̨(Ƕ��ʽ)�൱��float��QReal��windows���൱��double
								double propt = layer->features.at(k)->attributes.value(propertyName).toReal();
								//�ж�double�Ƿ����
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
	// TODO: �ڴ˴����ʵ�ִ���.
	if (sldDoc != NULL)
	{
		//��ȡԪ��
		QDomElement docElem = sldDoc->documentElement();//���ڵ�
		if (docElem.hasChildNodes())
		{
			QDomNodeList rootChilds = docElem.childNodes();
			for (int i = 0; i < rootChilds.size(); i++)
			{
				QString elemtxt = rootChilds.at(i).firstChild().toElement().text();
				//�ж��Ƿ�ǰͼ���Ƿ��ж�Ӧ��style
				if (elemtxt == lyname)
				{
					bool ruleFlag0 = false; //�ж��Ƿ����Rule��ǩ
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
