#include "aboutkuzya.h"
#include "ui_aboutkuzya.h"

#include <QDebug>
#include <QDir>

AboutKuzya::AboutKuzya(QVersionNumber verKuzia, QDate buildDate, QString fileName, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutKuzya)
{
    ui->setupUi(this);

    ui->textInfLabel = new QLabel(this);

    QFile mFile(fileName);

    //checks file
    if(QFile::exists(fileName) && mFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //reading AUTHORS file
        QTextStream mIn(&mFile);
        QString mTextInf;
        mTextInf  = mIn.readAll();

        //set Kuzia version and build date
        ui->textInfLabel->setText(mTextInf.arg(verKuzia.toString(), buildDate.toString("MMMM d yyyy")));
        ui->scrollAreaAuthors->setWidget(ui->textInfLabel);
    }
    else
    {
        exit(-1);
    }
}

AboutKuzya::~AboutKuzya()
{
    delete ui;
}