#include "DatabaseWizard.h"


DatabaseWizard::DatabaseWizard(QWidget *parentWidget):QWizard(parentWidget)
{
	//���������Ի���
    this->addPage(createIntroPage());
    this->addPage(createRegistrationPage());
    this->addPage(createConclusionPage());
    this->setWindowTitle("Database Connectivity Wizard");
	poDS = NULL;
}


DatabaseWizard::~DatabaseWizard(void)
{

}


bool DatabaseWizard::validateCurrentPage()
{
    qDebug()<<this->currentId();
	if(this->currentId()==1){
		//��ʼ������ݲ���
		QString dbname = dbnameLineEdit->text();
		QString table = tableLineEdit->text();
		QString addr = addrLineEdit->text();
		QString port = portLineEdit->text();
		QString username = usernameLineEdit->text();
		QString password = passwordLineEdit->text();
		if (this->rememberInputCkb->isChecked())
		{
			//�洢��һ������
			QJsonObject pJsonObj = JsonUtil::storeDbParams(table, dbname, addr, port, username, password);
			JsonUtil::jsonWrite((QDir::currentPath() + "/LastDbParams.json"), &pJsonObj);
		}
		poDS = GdalUtil::readFromPgsql(table, dbname, addr, port, username, password);
		if(poDS==NULL){
			QMessageBox::information(NULL, "failure",
											"Please check your parameters",
											QMessageBox::Yes,
											QMessageBox::Yes);
			return false;
		}
		return true;		
	}

    //�����ǰ��ť��idֵ �õ�����  ��OK �˰� ������Ϳ��Ը������ֵд�Լ��Ĵ�����
    if(-1 == this->currentPage()->nextId())//���� finish��ťʱ�����
    {
        //�����Լ�������������
    };
    return true;
}

//�򵼴���ʵ��
QWizardPage * DatabaseWizard::createIntroPage(){
	QWizardPage *page = new QWizardPage;
    page->setTitle("Introduction");
    QLabel *label = new QLabel("This wizard will help you connect Database ");
    label->setWordWrap(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    page->setLayout(layout);
    return page;
}

QWizardPage * DatabaseWizard::createRegistrationPage(){
	
	QString table="", dbname="", addr="", port="", username="", password="";  //�����ٷ�װһ��
	//�ж��ļ��Ƿ����
	QString filePath = QDir::currentPath() + "/LastDbParams.json";
	if (QFileInfo(filePath).exists() == true)
	{
		QJsonObject jsonObj = JsonUtil::JsonRead(filePath);
		JsonUtil::loadDbParams(&jsonObj, table, dbname, addr, port, username, password);
	}

	QWizardPage *page = new QWizardPage;
    page->setTitle("Connection Configuration");
    page->setSubTitle("Please fill both fields.");

    QLabel *dbnameLabel = new QLabel("Database Name:");
    dbnameLineEdit = new QLineEdit;
	dbnameLineEdit->setText(dbname);
	
	QLabel *tableLabel = new QLabel("Table Name:");
    tableLineEdit = new QLineEdit;
	tableLineEdit->setText(table);

	QLabel *addrLabel = new QLabel("Address:");
	addrLineEdit = new QLineEdit;
	addrLineEdit->setText(addr);
	
	QLabel *portLabel = new QLabel("Port:");
    portLineEdit = new QLineEdit;
	portLineEdit->setText(port);

	QLabel *usernameLabel = new QLabel("username:");
    usernameLineEdit = new QLineEdit;
	usernameLineEdit->setText(username);

	QLabel *passwordLabel = new QLabel("password:");
    passwordLineEdit = new QLineEdit;
	passwordLineEdit->setText(password);

	rememberInputCkb = new QCheckBox(page);
	rememberInputCkb->setText(QString::fromLocal8Bit("��ס��������"));  //��ֹ��������

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(dbnameLabel, 0, 0);
    layout->addWidget(dbnameLineEdit, 0, 1);

	layout->addWidget(tableLabel, 1, 0);
    layout->addWidget(tableLineEdit, 1, 1);

    layout->addWidget(addrLabel, 2, 0);
    layout->addWidget(addrLineEdit, 2, 1);
	
	layout->addWidget(portLabel, 3, 0);
    layout->addWidget(portLineEdit, 3, 1);
    
	layout->addWidget(usernameLabel, 4, 0);
    layout->addWidget(usernameLineEdit, 4, 1);
    
	layout->addWidget(passwordLabel, 5, 0);
    layout->addWidget(passwordLineEdit, 5, 1);
	
	layout->addWidget(rememberInputCkb, 6, 1);

    page->setLayout(layout);

    return page;
}

QWizardPage * DatabaseWizard::createConclusionPage(){
	QWizardPage *page = new QWizardPage;
    page->setTitle("Conclusion");

    QLabel *label = new QLabel("Database is now successfully connected.");
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    page->setLayout(layout);

    return page;

}