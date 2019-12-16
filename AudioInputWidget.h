#pragma once
#include <QHttpMultiPart>
#include <QWidget>
#include "ui_AudioInputWidget.h"
#include "AudioUtil.h"
#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
class AudioInputWidget : public QWidget
{
	Q_OBJECT

public:
	AudioInputWidget(QWidget *parent = Q_NULLPTR);
	~AudioInputWidget();
	AudioUtil audioUtil;
	QNetworkReply* reply;
	int flag;//���ƿ�������
	int time;//¼��ʱ��
	QString log;
	void refreshCursor();
	QTimer *timer;
signals:
	void sendResult(QString result);
public slots:
	void startRecord();
	void endRecord();
	void timeMinus();
private:
	Ui::AudioInputWidget ui;
};
static int VOICETIME = 10;//�10s
