#pragma once

#include <QObject>
#include <QPushButton>
#include <QAudioInput>
#include <QBuffer>
#include <qdebug.h>
#include "qfile.h"
#include <QTimer>
class AudioUtil : public QObject
{
	Q_OBJECT

public:
	AudioUtil(QObject *parent = 0);
	~AudioUtil();

	QAudioInput *audioInput; //¼������
	QByteArray voiceData;
	QBuffer bufDevice;
	QFile outFile;
	void startRecord();
	//��������
	QByteArray endRecord();

};
