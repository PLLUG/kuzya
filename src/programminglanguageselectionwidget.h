#ifndef PROGRAMMINGLANGUAGESELECTIONWIDGET_H
#define PROGRAMMINGLANGUAGESELECTIONWIDGET_H

#include <QWidget>
#include <QList>
#include <QPair>

namespace Ui {
class ProgrammingLanguageSelectionWidget;
}

class QAction;
class QSignalMapper;

class ProgrammingLanguageSelectionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProgrammingLanguageSelectionWidget(QWidget *parent = 0);
    ~ProgrammingLanguageSelectionWidget();

    void setLogo(const QString &logoPath);

    void addProgrammingLanguage(const QString &id,
                                const QString &name,
                                const QString &icon);

    void clear();

signals:
    void languageSelected(QString);

private slots:
    void slotLanguageMapped(QString languageId);

private:
    void updateActionsForButtons();
    void disconnectUi();

private:
    QList<QPair<QString, QAction *>> mLanguageActionById;
    QSignalMapper *mLanguageMapper;

    Ui::ProgrammingLanguageSelectionWidget *ui;
};

#endif // PROGRAMMINGLANGUAGESELECTIONWIDGET_H
