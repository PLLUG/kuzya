#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "version.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    QString lText = QString("\t  <big><b><centre> \t    The Kuzya %1 </centre> </b></big>  "
                            "\n  <p> Free Development Environment</p>\n\n"
                            "build on Jule 7 2010"
                            "<p> Kuzya is simple crossplatform IDE for people who study  programming."
                            "Main idea of it is to concentrate attention  of the users only on learning the programming \n"
                            "\t language  but not on usage of IDE. For more information visit our official web site "
                            "<a href= http://kuzya.sourceforge.net>http://kuzya.sourceforge.net</a> \n\n </p>").arg(KUZYA_VERSION);
    QString lAuthorsText = QString("<pre>"
                                   "<p align = \"center\"> <font size=\"4\" face=\"Verdana\" color=\"#000000\">Idea:</font> </p>"
                                   "<p><font size=\"4\" face=\"Tahoma\" color=\"#303030\">Grygoriy Zlobin</font>"
                                   " <a href= zlobin@electronics.wups.lviv.ua>zlobin@electronics.wups.lviv.ua</a>"
                                   "</p>"
                                   "<p align = \"center\"> <font size=\"4\" face=\"Verdana\" color=\"#000000\">Team leader:</font> </p>"
                                   "<p><font size=\"4\" face=\"Tahoma\" color=\"#303030\">Andriy Shevchyk</font>"
                                   " <a href= shevchyk@users.sourceforge.net>shevchyk@users.sourceforge.net</a>"
                                   "</p>"
                                   "<p align = \"center\"> <font size=\"4\" face=\"Verdana\" color=\"#000000\">Developers:</font> </p>"
                                   "<p><font size=\"4\" face=\"Tahoma\" color=\"#303030\">Volodymyr Shevchyk</font>"
                                   " <a href= volder@users.sourceforge.net>volder@users.sourceforge.net</a>"
                                   "</p>"
                                   "<p><font size=\"4\" face=\"Tahoma\" color=\"#303030\">Victor Sklyar</font>"
                                   " <a href= bouyantgrambler@users.sourceforge.net>bouyantgrambler@users.sourceforge.net</a>"
                                   "</p>"
                                   "<p><font size=\"4\" face=\"Tahoma\" color=\"#303030\">Alex Chmykhalo</font>"
                                   " <a href= alexchmykhalo@users.sourceforge.net>alexchmykhalo@users.sourceforge.net</a>"
                                   "</p></pre>");

    ui->aboutLabel->setText(lText);
    ui->aboutLabel->setWordWrap(true);
    ui->aboutLabel->setOpenExternalLinks(true);
    ui->aboutLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);

    ui->authorsLabel->setText(lAuthorsText);
    ui->authorsLabel->setWordWrap(true);
    ui->authorsLabel->setOpenExternalLinks(true);
    ui->authorsLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);

    ui->TitleLabel->setText(QString("Kuzya %1").arg(KUZYA_VERSION));

}

AboutDialog::~AboutDialog()
{
    delete ui;
}
