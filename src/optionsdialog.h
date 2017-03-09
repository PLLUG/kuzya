/******************************************************************************
 *   Copyright (C) 2008 by                                                    *
 *                     Volodymyr Shevchyk (volder@users.sourceforge.net),     *
 *                     Victor Sklyar (bouyantgrambler@users.sourceforge.net), *
 *                     Alex Chmykhalo (alexchmykhalo@users.sourceforge.net)   *
 *                                                                            *
 *   This program is free software: you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation, either version 3 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>     *
 ******************************************************************************/

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QObject>
#include <QDialog>
#include <QPalette>
#include <QSettings>
#include <QTranslator>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QStyleFactory>
#include "ui_optionsdialog.h"
#include "kuzya.h"

#define STYLESHEETS_RELATIVE_PATH "resources/qss/"
#define TRANSLATIONS_REALATIVE_PATH "resources/translations/"

class QListWidget;
class QsciScintilla;
class Kuzya;


class OptionsDialog : public QDialog, private Ui::optionsForm
{
    Q_OBJECT
public:
    OptionsDialog(QWidget *parent = 0);
    ~OptionsDialog();
    void writeSettings(void);
    void readODWSettings();
    void writeMainWindowState();
    void readMainWindowState();
    void openLastProject();
    void saveLastProjectName(QString);
    void retranslate(void);
    bool ukrIsCheked();
    QString readDefaultCompiler(QString lang);
    QString readCompilerLocation(QString lang, QString comp);
    QString readCompilerOptions(QString lang, QString comp);
    bool isFileReopenEnabled()const;
    QString getDefaultLanguage()const;

public slots:
    void slotCommOptions(void);

private slots:

    void slotClose(void);
    void slotApply(void);
    void slotOk(void);
    void slotDefaultAll(void);
    void slotChangeFormColor(void);
    void slotChangeDefDir(int);
    void slotChangeDefDir(QString);
    void slotChangeStyle(int);
    void slotChangeSkin(QString);
    //  void slotLoadCompilerSettings(void);
    void slotUpdateSkinsCBox(void);
    void slotUpdatelocalizationLanguageCBox();
    void slotUpdateCompilerCBox(QString);
    void slotLoadCompilerOptions(QString);
    void slotChangeCompilerLocation();
    void slotDefaultCompiler();
    void slotResetCompilerOptions();
    void slotChangsLocalizationLanguage(QString);
    void slotChangeOptionPage(int);

private:
    QTranslator* trans_ua;
    QTranslator* trans_en;
    Kuzya* mw;
    QFile file;
    QSettings *settings;
    QsciScintilla* textEditor;
    QTranslator translator;
    QFont font;
    QDir stylesDir;
    QDir localizationLanguageDir;
    QStringList styleFilters;
    QStringList localizationLanguageFilters;
    bool isReopenFile;
public:	
    bool isLineMarginVisible;
};

#endif
