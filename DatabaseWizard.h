#pragma once
#include <QWizard>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <qdebug.h>
#include "GdalUtil.h"
#include <qmessagebox.h>
#include "gdal.h"
#include "ogr_api.h"
#include "ogrsf_frmts.h"
class DatabaseWizard:public QWizard
{
	Q_OBJECT
public:
	DatabaseWizard::DatabaseWizard(QWidget *parentWidget);
	~DatabaseWizard(void);
	OGRDataSource *poDS;
	QLineEdit *dbnameLineEdit;
	QLineEdit *tableLineEdit;
	QLineEdit *addrLineEdit;
	QLineEdit *portLineEdit;
	QLineEdit *usernameLineEdit;
	QLineEdit *passwordLineEdit;
	virtual bool validateCurrentPage();
private:
	QWizardPage *createIntroPage();
    QWizardPage *createRegistrationPage();
    QWizardPage *createConclusionPage();
};

