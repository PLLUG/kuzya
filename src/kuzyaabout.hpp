#ifndef KUZYAABOUT_HPP
#define KUZYAABOUT_HPP

#include <QDialog>
#include <QString>
#include <QFile>
#include <QTextStream>

namespace Ui {
  class KuzyaAbout;
}

class KuzyaAbout : public QDialog
{
  Q_OBJECT

public:
  explicit KuzyaAbout(QWidget *parent = 0);
  void checkFile();
  void readAuthors();
  void setVerAndDate();
  void fillWindow();
  ~KuzyaAbout();

private:
  Ui::KuzyaAbout *ui;
  const QString *fileName = new QString(":/AUTHORS.txt");
  QFile *file;
  QString *textInf;
  QTextStream *in;
};

#endif // KUZYAABOUT_HPP
