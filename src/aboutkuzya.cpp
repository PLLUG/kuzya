#include "aboutkuzya.h"
#include "ui_AboutKuzya.h"

#include <QDebug>
#include <QDir>

AboutKuzya::AboutKuzya(QVersionNumber verKuzia, QDate buildDate, QString fileName, QWidget *parent) :
  mVerKuzia(&verKuzia), mBuildDate(&buildDate), mFileName(&fileName), QDialog(parent),
  ui(new Ui::AboutKuzya)
{
  ui->setupUi(this);

  ui->textInfLabel = new QLabel(this);

  checkFile();
  readAuthors();
  setVerAndDate();
}

//checks file
void AboutKuzya::checkFile()
{
  mFile = new QFile (*mFileName);

  if(!QFile::exists(*mFileName))
  {
      return;
  }
  if (!mFile->open(QIODevice::ReadOnly | QIODevice::Text))
  {
      return;
    }
}

//reading AUTHORS file
void AboutKuzya::readAuthors()
{
  mIn = new QTextStream(mFile);
  mTextInf = new QString;
  *mTextInf  = mIn->readAll();
}

//set Kuzia version and build date
void AboutKuzya::setVerAndDate()
{
  ui->textInfLabel->setText(mTextInf->arg(mVerKuzia->toString(), mBuildDate->toString("MMMM d yyyy")));
  ui->scrollAreaAuthors->setWidget(ui->textInfLabel);
}

AboutKuzya::~AboutKuzya()
{
  delete mTextInf;
  delete mIn;
  delete mFile;
  delete ui;
}
