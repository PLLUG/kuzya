#include "programminglanguageselectionwidget.h"
#include "ui_programminglanguageselectionwidget.h"

#include <QMenu>
#include <QSignalMapper>

#include <QDebug>

ProgrammingLanguageSelectionWidget::ProgrammingLanguageSelectionWidget(QWidget *parent) :
    QWidget(parent),
    mLanguageMapper(new QSignalMapper(this)),
    ui(new Ui::ProgrammingLanguageSelectionWidget)
{
    ui->setupUi(this);

    ui->additionalLanguageSelectionToolButton->setVisible(false);
    ui->languageSelectionButton1->setVisible(false);
    ui->languageSelectionButton2->setVisible(false);

    ui->additionalLanguageSelectionToolButton->setMenu(new QMenu(this));
    connect(mLanguageMapper, SIGNAL(mapped(QString)), this, SLOT(slotLanguageMapped(QString)), Qt::UniqueConnection);
}

ProgrammingLanguageSelectionWidget::~ProgrammingLanguageSelectionWidget()
{
    delete ui;
}

void ProgrammingLanguageSelectionWidget::setLogo(const QString &logoPath)
{
    ui->logoLabel->setPixmap(QPixmap(logoPath));
}

void ProgrammingLanguageSelectionWidget::addProgrammingLanguage(const QString &id, const QString &name, const QString &icon)
{
    QAction *action = new QAction(QIcon(icon), name);
    mLanguageActionById.append(qMakePair(id, action));
    mLanguageMapper->setMapping(action, id);
    connect(action, SIGNAL(triggered()), mLanguageMapper, SLOT(map()), Qt::UniqueConnection);
    updateActionsForButtons();
}

void ProgrammingLanguageSelectionWidget::clear()
{
    disconnectUi();

    for (auto actionByIdPair: mLanguageActionById)
    {
        auto nextAction = actionByIdPair.second;
        delete nextAction;
    }

    mLanguageActionById.clear();
}

void ProgrammingLanguageSelectionWidget::slotLanguageMapped(QString languageId)
{
    emit languageSelected(languageId);
}

void ProgrammingLanguageSelectionWidget::updateActionsForButtons()
{
    disconnectUi();

    auto actionsIterator = mLanguageActionById.constBegin();

    // Use first action for languageSelectionButton1
    if (actionsIterator != mLanguageActionById.constEnd())
    {
        ui->languageSelectionButton1->setVisible(true);
        ui->languageSelectionButton1->setDefaultAction((*actionsIterator).second);
        actionsIterator++;
    }

    // Use second action for languageSelectionButton2
    if (actionsIterator != mLanguageActionById.constEnd())
    {
        ui->languageSelectionButton2->setVisible(true);
        ui->languageSelectionButton2->setDefaultAction((*actionsIterator).second);
        actionsIterator++;
    }

    QList<QAction *> listActions;
    for (auto it = actionsIterator; it != mLanguageActionById.constEnd(); it++)
    {
        ui->additionalLanguageSelectionToolButton->setVisible(true);
        auto nextAction = (*actionsIterator).second;
        listActions.append(nextAction);
        actionsIterator++;
    }


    QMenu *additionalMenu = ui->additionalLanguageSelectionToolButton->menu();
    additionalMenu->addActions(listActions);

}



void ProgrammingLanguageSelectionWidget::disconnectUi()
{
    ui->languageSelectionButton1->setDefaultAction(nullptr);
    ui->languageSelectionButton2->setDefaultAction(nullptr);
    ui->additionalLanguageSelectionToolButton->menu()->clear();
}
