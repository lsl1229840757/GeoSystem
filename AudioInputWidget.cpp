#include "AudioInputWidget.h"

AudioInputWidget::AudioInputWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(startRecord()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(endRecord()));
	flag = 0;
	time = VOICETIME;
	//��ʼ��ʱ
	timer = new QTimer(this); //this Ϊparent��, ��ʾ��ǰ����
	connect(timer, SIGNAL(timeout()), this, SLOT(timeMinus())); // SLOT����һ���ۺ���
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
		//���
		log += QString::fromLocal8Bit("����ʶ��ʼ, 10s���Զ�ֹͣ~")+"\n";
		refreshCursor();
		ui.textBrowser->setText(log);
		timer->start(1000); // 1000����, ���� 1 ��
	}
}



void AudioInputWidget::refreshCursor()
{			//���Ź����
	QTextCursor textCursor = ui.textBrowser->textCursor();//�õ���ǰ�Ĺ��
	textCursor.movePosition(QTextCursor::End);
	if (textCursor.hasSelection())//�����ѡ�У���ȡ������������Ӱ��
		textCursor.clearSelection();
	textCursor.deletePreviousChar();//ɾ��ǰһ���ַ�
	ui.textBrowser->setTextCursor(textCursor);
}

void AudioInputWidget::endRecord() {
	if (flag == 1) {
		//��ʼ��������
		QByteArray data = audioUtil.endRecord();
		QBuffer *buf = new QBuffer;
		buf->setData(data);
		buf->open(QIODevice::ReadWrite);
		//��ʼ����,  http����
		QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
		QHttpPart voicePart;
		voicePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\"test.pcm\"")); //�����ļ�����
		voicePart.setBodyDevice(buf);
		buf->setParent(multiPart);
		multiPart->append(voicePart);
		//����url����
		QUrl url("http://www.cartovision.cn/LuceneDemo/voiceSearch");
		QNetworkRequest request(url);
		QNetworkAccessManager* manager = new QNetworkAccessManager(this);
		//��������
		reply = manager->post(request, multiPart);
		multiPart->setParent(reply);
		QEventLoop eventLoop;
		connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
		eventLoop.exec();
		QString result = reply->readAll();
		reply->deleteLater();//�����ɾ����������
		//���»ָ��������ڵı���
		flag = 0;
		time = 10;
		log.clear();
		emit sendResult(result);//������Ϣ
	}
}

void AudioInputWidget::timeMinus()
{
	if (flag == 1) {
		//��ʼ��ʱ��
		if (time > 0) {
			time--;
			//�����Ϣ
			log += QString::fromLocal8Bit("����") + QString::number(time) + "s\n";
			ui.textBrowser->setText(log);
			refreshCursor();
		}
		else {
			//�����Ϣ
			log += QString::fromLocal8Bit("ʶ�����~") + "\n";
			ui.textBrowser->setText(log);
			refreshCursor();
			//����0���Զ�ֹͣ
			endRecord();
		}
	}
}
