/********************************************************************************
** Form generated from reading ui file 'finddialog.ui'
**
** Created: Tue 7. Jul 00:38:24 2009
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_FINDDIALOG_H
#define UI_FINDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_findForm
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QFrame *frame;
    QGridLayout *gridLayout1;
    QVBoxLayout *vboxLayout;
    QLabel *label_2;
    QComboBox *findComboBox;
    QHBoxLayout *hboxLayout;
    QCheckBox *regExprCB;
    QSpacerItem *spacerItem;
    QPushButton *regExprEditBtn;
    QLabel *label_3;
    QFrame *frame_2;
    QGridLayout *gridLayout2;
    QCheckBox *caseSensCB;
    QGroupBox *findBox;
    QGridLayout *gridLayout3;
    QVBoxLayout *vboxLayout1;
    QRadioButton *forwardRBtn;
    QRadioButton *radioButton_2;
    QCheckBox *wWOnlyCB;
    QCheckBox *fromcurCB;
    QCheckBox *wrapCB;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacerItem1;
    QPushButton *findBtn;
    QSpacerItem *spacerItem2;
    QPushButton *closeBtn;

    void setupUi(QDialog *findForm)
    {
    if (findForm->objectName().isEmpty())
        findForm->setObjectName(QString::fromUtf8("findForm"));
    findForm->setWindowModality(Qt::ApplicationModal);
    findForm->resize(292, 347);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(findForm->sizePolicy().hasHeightForWidth());
    findForm->setSizePolicy(sizePolicy);
    findForm->setMinimumSize(QSize(0, 0));
    findForm->setMaximumSize(QSize(3342, 3665));
    QPalette palette;
    QBrush brush(QColor(0, 0, 0, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    QBrush brush1(QColor(211, 206, 197, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Button, brush1);
    QBrush brush2(QColor(255, 255, 255, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Light, brush2);
    QBrush brush3(QColor(233, 230, 226, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
    QBrush brush4(QColor(105, 103, 98, 255));
    brush4.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
    QBrush brush5(QColor(141, 137, 131, 255));
    brush5.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
    palette.setBrush(QPalette::Active, QPalette::Text, brush);
    palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
    palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Active, QPalette::Base, brush2);
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
    palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush3);
    QBrush brush6(QColor(255, 255, 220, 255));
    brush6.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush6);
    palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
    palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
    palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
    palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
    palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
    palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush3);
    palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush6);
    palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
    palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
    palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
    palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush6);
    palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
    findForm->setPalette(palette);
    gridLayout = new QGridLayout(findForm);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    label = new QLabel(findForm);
    label->setObjectName(QString::fromUtf8("label"));
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy1);
    QFont font;
    font.setPointSize(12);
    label->setFont(font);

    gridLayout->addWidget(label, 0, 0, 1, 1);

    frame = new QFrame(findForm);
    frame->setObjectName(QString::fromUtf8("frame"));
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    gridLayout1 = new QGridLayout(frame);
    gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
    vboxLayout = new QVBoxLayout();
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    label_2 = new QLabel(frame);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy1);

    vboxLayout->addWidget(label_2);

    findComboBox = new QComboBox(frame);
    findComboBox->setObjectName(QString::fromUtf8("findComboBox"));
    findComboBox->setEditable(true);

    vboxLayout->addWidget(findComboBox);


    gridLayout1->addLayout(vboxLayout, 0, 0, 1, 1);

    hboxLayout = new QHBoxLayout();
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    regExprCB = new QCheckBox(frame);
    regExprCB->setObjectName(QString::fromUtf8("regExprCB"));
    regExprCB->setEnabled(false);

    hboxLayout->addWidget(regExprCB);

    spacerItem = new QSpacerItem(44, 30, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacerItem);

    regExprEditBtn = new QPushButton(frame);
    regExprEditBtn->setObjectName(QString::fromUtf8("regExprEditBtn"));
    regExprEditBtn->setEnabled(false);

    hboxLayout->addWidget(regExprEditBtn);


    gridLayout1->addLayout(hboxLayout, 1, 0, 1, 1);


    gridLayout->addWidget(frame, 1, 0, 1, 1);

    label_3 = new QLabel(findForm);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
    label_3->setSizePolicy(sizePolicy1);
    label_3->setFont(font);

    gridLayout->addWidget(label_3, 2, 0, 1, 1);

    frame_2 = new QFrame(findForm);
    frame_2->setObjectName(QString::fromUtf8("frame_2"));
    frame_2->setFrameShape(QFrame::Box);
    frame_2->setFrameShadow(QFrame::Plain);
    gridLayout2 = new QGridLayout(frame_2);
    gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
    caseSensCB = new QCheckBox(frame_2);
    caseSensCB->setObjectName(QString::fromUtf8("caseSensCB"));

    gridLayout2->addWidget(caseSensCB, 0, 0, 1, 1);

    findBox = new QGroupBox(frame_2);
    findBox->setObjectName(QString::fromUtf8("findBox"));
    gridLayout3 = new QGridLayout(findBox);
    gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    forwardRBtn = new QRadioButton(findBox);
    forwardRBtn->setObjectName(QString::fromUtf8("forwardRBtn"));
    forwardRBtn->setChecked(true);

    vboxLayout1->addWidget(forwardRBtn);

    radioButton_2 = new QRadioButton(findBox);
    radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

    vboxLayout1->addWidget(radioButton_2);


    gridLayout3->addLayout(vboxLayout1, 0, 0, 1, 1);


    gridLayout2->addWidget(findBox, 0, 1, 4, 1);

    wWOnlyCB = new QCheckBox(frame_2);
    wWOnlyCB->setObjectName(QString::fromUtf8("wWOnlyCB"));

    gridLayout2->addWidget(wWOnlyCB, 1, 0, 1, 1);

    fromcurCB = new QCheckBox(frame_2);
    fromcurCB->setObjectName(QString::fromUtf8("fromcurCB"));
    fromcurCB->setEnabled(true);
    fromcurCB->setChecked(true);

    gridLayout2->addWidget(fromcurCB, 2, 0, 1, 1);

    wrapCB = new QCheckBox(frame_2);
    wrapCB->setObjectName(QString::fromUtf8("wrapCB"));
    wrapCB->setChecked(true);
    wrapCB->setTristate(false);

    gridLayout2->addWidget(wrapCB, 3, 0, 1, 1);


    gridLayout->addWidget(frame_2, 3, 0, 1, 1);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem1);

    findBtn = new QPushButton(findForm);
    findBtn->setObjectName(QString::fromUtf8("findBtn"));

    hboxLayout1->addWidget(findBtn);

    spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout1->addItem(spacerItem2);

    closeBtn = new QPushButton(findForm);
    closeBtn->setObjectName(QString::fromUtf8("closeBtn"));

    hboxLayout1->addWidget(closeBtn);


    gridLayout->addLayout(hboxLayout1, 4, 0, 1, 1);


    retranslateUi(findForm);
    QObject::connect(regExprCB, SIGNAL(clicked(bool)), regExprEditBtn, SLOT(setEnabled(bool)));

    QMetaObject::connectSlotsByName(findForm);
    } // setupUi

    void retranslateUi(QDialog *findForm)
    {
    findForm->setWindowTitle(QApplication::translate("findForm", "Find Text", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("findForm", "Find", 0, QApplication::UnicodeUTF8));
    label_2->setText(QApplication::translate("findForm", "Text to find", 0, QApplication::UnicodeUTF8));
    regExprCB->setText(QApplication::translate("findForm", "Regular Expression", 0, QApplication::UnicodeUTF8));
    regExprEditBtn->setText(QApplication::translate("findForm", "Edit...", 0, QApplication::UnicodeUTF8));
    label_3->setText(QApplication::translate("findForm", "Options", 0, QApplication::UnicodeUTF8));
    caseSensCB->setText(QApplication::translate("findForm", "Case sensitive", 0, QApplication::UnicodeUTF8));
    findBox->setTitle(QApplication::translate("findForm", "Find", 0, QApplication::UnicodeUTF8));
    forwardRBtn->setText(QApplication::translate("findForm", "Forward", 0, QApplication::UnicodeUTF8));
    radioButton_2->setText(QApplication::translate("findForm", "Backward", 0, QApplication::UnicodeUTF8));
    wWOnlyCB->setText(QApplication::translate("findForm", "Whole word only", 0, QApplication::UnicodeUTF8));
    fromcurCB->setText(QApplication::translate("findForm", "From cursor", 0, QApplication::UnicodeUTF8));
    wrapCB->setText(QApplication::translate("findForm", "Wrap arround the text", 0, QApplication::UnicodeUTF8));
    findBtn->setText(QApplication::translate("findForm", "Find next", 0, QApplication::UnicodeUTF8));
    closeBtn->setText(QApplication::translate("findForm", "Close", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(findForm);
    } // retranslateUi

};

namespace Ui {
    class findForm: public Ui_findForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDDIALOG_H
