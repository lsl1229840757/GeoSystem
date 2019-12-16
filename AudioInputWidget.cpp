#include "AudioInputWidget.h"

AudioInputWidget::AudioInputWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(startRecord()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(endRecord()));
	flag = 0;
	time = VOICETIME;
	//开始计时
	timer = new QTimer(this); //this 为parent类, 表示当前窗口
	connect(timer, SIGNAL(timeout()), this, SLOT(timeMinus())); // SLOT填入一个槽函数
	this->setFixedSize(400, 146);
}

AudioInputWidget::~AudioInputWidget()
{
}

void AudioInputWidget::startRecord() {
	if (flag == 0) {
		audioUtil.startRecord();
		flag++;
		time = 10;
		//输出
		log += QString::fromLocal8Bit("语音识别开始, 10s后自动停止~")+"\n";
		refreshCursor();
		ui.textBrowser->setText(log);
		timer->start(1000); // 1000毫秒, 等于 1 秒
	}
}



void AudioInputWidget::refreshCursor()
{			//跟着光标走
	QTextCursor textCursor = ui.textBrowser->textCursor();//得到当前的光标
	textCursor.movePosition(QTextCursor::End);
	if (textCursor.hasSelection())//如果有选中，则取消，以免受受影响
		textCursor.clearSelection();
	textCursor.deletePreviousChar();//删除前一个字符
	ui.textBrowser->setTextCursor(textCursor);
}

void AudioInputWidget::endRecord() {
	if (flag == 1) {
		//开始网络请求
		QByteArray data = audioUtil.endRecord();
		QBuffer *buf = new QBuffer;
		buf->setData(data);
		buf->open(QIODevice::ReadWrite);
		//开始测试,  http请求
		QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
		QHttpPart voicePart;
		voicePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"test.pcm\"")); //设置文件名字
		voicePart.setBodyDevice(buf);
		buf->setParent(multiPart);
		multiPart->append(voicePart);
		//设置url连接
		QUrl url("http://www.cartovision.cn/LuceneDemo/voiceSearch");
		QNetworkRequest request(url);
		QNetworkAccessManager* manager = new QNetworkAccessManager(this);
		//发送请求
		reply = manager->post(request, multiPart);
		multiPart->setParent(reply);
		QEventLoop eventLoop;
		connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
		eventLoop.exec();
		QString result = reply->readAll();
		reply->deleteLater();//用完后删除所有内容
		//重新恢复生命周期的变量
		flag = 0;
		time = 10;
		log.clear();
		emit sendResult(result);//发送信息
	}
}

void AudioInputWidget::timeMinus()
{
	if (flag == 1) {
		//开始减时间
		if (time > 0) {
			time--;
			//输出信息
			log += QString::fromLocal8Bit("还有") + QString::number(time) + "s\n";
			ui.textBrowser->setText(log);
			refreshCursor();
		}
		else {
			//输出信息
			log += QString::fromLocal8Bit("识别结束~") + "\n";
			ui.textBrowser->setText(log);
			refreshCursor();
			//减到0就自动停止
			endRecord();
		}
	}
}
