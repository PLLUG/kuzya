#ifndef ABOUTKUZYA_H
#define ABOUTKUZYA_H

#include <QDialog>
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
};

#endif // ABOUTKUZYA_H
