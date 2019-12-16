#include "AudioUtil.h"
AudioUtil::AudioUtil(QObject *parent)
	: QObject(parent)
{

}

AudioUtil::~AudioUtil()
{
}
//开始录音
void AudioUtil::startRecord()
{
	QAudioFormat audioFormat;
	audioFormat.setByteOrder(QAudioFormat::LittleEndian);
	audioFormat.setSampleType(QAudioFormat::SignedInt);
	audioFormat.setChannelCount(1);
	audioFormat.setCodec("audio/pcm");
	audioFormat.setSampleRate(16000);
	audioFormat.setSampleSize(16);
	//判断设备，查看是否存在
	QAudioDeviceInfo devInfo = QAudioDeviceInfo::defaultInputDevice();
	if (devInfo.isNull()) {
		qDebug() << "未找到录音设备";
	}
	//不支持格式，使用最接近格式
	if (!devInfo.isFormatSupported(audioFormat)) { //当前使用设备是否支持
		audioFormat = devInfo.nearestFormat(audioFormat); //转换为最接近格式
	}
	audioInput = new QAudioInput(audioFormat, NULL);
	//内存的IO对象
	bufDevice.setBuffer(&voiceData);
	////开始录音
	bufDevice.open(QIODevice::WriteOnly | QIODevice::Truncate);
	//audioInput->start(&bufDevice);
	//文件方式
	//outFile.setFileName("C:\\Users\\Administrator\\Desktop\\test.wav"); //语音原始文件
	//outFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
	int len = bufDevice.data().length();
	qDebug() << "begin:" << len;
	audioInput->start(&bufDevice);
}

QByteArray AudioUtil::endRecord()
{
	//写到IO设备头
	//outFile.close();
	audioInput->stop();
	bufDevice.close();
	return bufDevice.data();
	//device->close();
}
