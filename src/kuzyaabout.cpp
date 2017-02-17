#include "kuzyaabout.hpp"
#include "ui_kuzyaabout.h"

#include <QVersionNumber>
#include <QDate>
#include <QDebug>
#include <QDir>
#include "version.h"

KuzyaAbout::KuzyaAbout(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::KuzyaAbout)
{
  ui->setupUi(this);

  ui->layout = new QVBoxLayout(this);
  this->setLayout(ui->layout);
  ui->textInfLabel = new QLabel(this);
  ui->imgLabel = new QLabel(this);
}

//checks file
void KuzyaAbout::checkFile()
{
  file = new QFile (*fileName);

  if(!QFile::exists(*fileName))
  {
      qCritical()<< "File doesn't exist"<<*fileName<<endl;
  }
  if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
  {
      qCritical()<<"couldn't open file"<<*fileName;
    }
}

//reading AUTHORS file
void KuzyaAbout::readAuthors()
{
  in = new QTextStream(file);
  textInf = new QString;
  *textInf  = in->readAll();
}

//set Kuzia version and build date
void KuzyaAbout::setVerAndDate()
{
  QVersionNumber verKuzia(MAJORVER, MINORVER, REVISION);
  QDate buildDate(BUILD_YEAR, BUILD_MONTH, BUILD_DAY);
  ui->textInfLabel->setText(textInf->arg(verKuzia.toString(), buildDate.toString("MMMM d yyyy")));
}

//fill Dialog window
void KuzyaAbout::fillWindow()
{
  ui->imgLabel->setPixmap(QPixmap(":/common/Kuzya_about.png"));
  ui->layout->addWidget(ui->imgLabel);
  ui->scrollAreaAuthors = new QScrollArea(this);
  ui->scrollAreaAuthors->setWidget(ui->textInfLabel);
  ui->layout->addWidget(ui->scrollAreaAuthors);
  this->setWindowIcon(QIcon(":/menu/images/kuzya.png"));

  this->exec();
}

KuzyaAbout::~KuzyaAbout()
{
  delete textInf;
  delete in;
  delete file;
  delete ui;
}
