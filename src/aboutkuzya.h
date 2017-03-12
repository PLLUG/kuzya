#ifndef ABOUTKUZYA_H
#define ABOUTKUZYA_H

#include <QDialog>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QVersionNumber>
#include <QDate>

namespace Ui {
  class AboutKuzya;
}

class AboutKuzya : public QDialog
{
  Q_OBJECT

public:
  AboutKuzya(QVersionNumber verKuzia, QDate buildDate, QString fileName, QWidget *parent = 0);
  ~AboutKuzya();

private:
  Ui::AboutKuzya *ui;
  QString *mFileName;
  QFile *mFile;
  QString *mTextInf;
  QTextStream *mIn;
  QVersionNumber *mVerKuzia;
  QDate *mBuildDate;

  void checkFile();
  void readAuthors();
  void setVerAndDate();
};

#endif // ABOUTKUZYA_H
