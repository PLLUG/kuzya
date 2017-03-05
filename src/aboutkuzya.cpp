#include "aboutkuzya.h"
#include "ui_AboutKuzya.h"

#include <QDebug>
#include <QDir>

AboutKuzya::AboutKuzya(QWidget *parent, QVersionNumber *verKuzia, QDate *buildDate, QString *fileName) :
  QDialog(parent), mVerKuzia(verKuzia), mBuildDate(buildDate), mFileName(fileName),
  ui(new Ui::AboutKuzya)
{
  ui->setupUi(this);

  ui->layout = new QVBoxLayout(this);
  this->setLayout(ui->layout);
  ui->textInfLabel = new QLabel(this);
  ui->imgLabel = new QLabel(this);

  checkFile();
  readAuthors();
  setVerAndDate();

  exec();
}

//checks file
void AboutKuzya::checkFile()
{
  mFile = new QFile (*mFileName);

  if(!QFile::exists(*mFileName))
  {
      qCritical()<< "File doesn't exist"<<*mFileName<<endl;
  }
  if (!mFile->open(QIODevice::ReadOnly | QIODevice::Text))
  {
      qCritical()<<"couldn't open file"<<*mFileName;
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
