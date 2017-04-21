#include "debugform.h"
#include "ui_debugform.h"

#include "kuzya.h"
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpascal.h>
#include <Qsci/qscilexerfortran.h>
#include <Qsci/qscilexerjava.h>
#include <QDebug>

DebugForm::DebugForm(QWidget *parent, Kuzya* _mw) :
    QWidget(parent),
    ui(new Ui::DebugForm),
    mw(_mw)
{
    ui->setupUi(this);
    connect(ui->butAddMarker, SIGNAL(clicked(bool)), this, SLOT(slotAddMarker()), Qt::UniqueConnection);
    connect(ui->lineMarker, SIGNAL(returnPressed()), this, SLOT(slotAddMarker()), Qt::UniqueConnection);

}

DebugForm::~DebugForm()
{
    delete ui;
}

void DebugForm::slotAddMarker()
{
    mw->textEditor->markerAdd(ui->lineMarker->text().toInt(), mw->breakpointMarker);
}

void DebugForm::slotMarginClicked(int, int, Qt::KeyboardModifiers)
{
    qDebug() << "emitted";
}
