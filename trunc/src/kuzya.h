/***************************************************************************
 *   Copyright (C) 2008 by Volodymyr Shevchyk                              *
 *                                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef KUZYA_H
#define KUZYA_H

#include <QMainWindow>
#include <QProcess>

#include "optionsdialog.h"
#include "ui_kuzya.h"
#include "compiler.h"

class QListWidget;
class QSplitter;
class FindDialog;
class ReplaceDialog;
class QSignalMapper;
class QsciScintilla;
class GoToLineDialog;
class OptionsDialog;
class QLabel;
class QShortcut;
class QListWidgetItem;
//class QPrinter;
class QsciPrinter;
class QsciLexerCPP;
template <typename T> class QVector;
template <typename T> class QList;

class Kuzya: public QMainWindow, private Ui::kuzyaForm
{
    Q_OBJECT

public:

    enum notificationItemDataRoleEnum {attachedRole = Qt::UserRole+1,
                                       lineRole = Qt::UserRole+2,
                                       descriptionRole = Qt::UserRole+3};

    enum translationEnum { fromCode, fromNative };

    Kuzya(QWidget *parent = 0);
    ~Kuzya();
    
        void openFile(QString);
        void markerDeleteAll(void);
        void setText(QString);
        void setMWTitle(QString);
        void setTEModified(bool);
        void slotUpdateRecentFileList(void);
        void cleanMemory_ActOpenRecentFileVector(void);

        void addFileNameToList(QString);
        int getFileListCount(void);
        void resizeRFileList(int);
        QString getRFileListAt(int);
        void setMaxCount_RFileList(int);
        int getMaxCount_RFileList(void);

        void setFont(QFont);
        void retranslate(void);

        void setDefaultDir(QString);
        void retranslateAll();
        void setEditorFont(QFont);

        void loadPascalLexer(void);
        void loadCPPLexer(void);
        QFont getEditorFont(void);

        QsciScintilla* getTextEditorPointer(void);
        void toggleFoldsActionEnabled(bool b);
        void setAutoCompletionEnabled(bool b);
        Compiler* getCurrentCompiler();

protected:

private slots:
        void slotUpdateStatusLabel(int line, int pos);
        void slotUpdateWindowName(bool m);
        void slotNew(void);
        void slotOpen(void);
///void slotSave(void);
        bool slotSave(void);
        void slotSave_as(void);
        void slotPrint(void);
        void slotExit(void);
        void slotRun(void);
        void slotCompile(void);
        void slotAbout(void);
        void slotAfterCompile(int status);
        void slotMarginClicked(int margin, int line, Qt::KeyboardModifiers state);
        void slotVoid(void);
        void slotChar(void);
        void slotWchar_t(void);
        void slotShort(void);
        void slotInt(void);
        void slotDouble(void);
        void slotFloat(void);
        void slotLong(void);
        void slotBool(void);
        void slotSigned(void);
        void slotUnsigned(void);
        void slotConst(void);
        void slotTypedef(void);
        void slotClass(void);
        void slotStruct(void);
        void slotEnum(void);
        void slotUnion(void);
        void slotNewMemory(void);
        void slotDelete(void);
        void slotConst_cast(void);
        void slotReinterpret_cast(void);
        void slotStatic_cast(void);
        void slotAnd(void);
        void slotAnd_eq(void);
        void slotBitand(void);
        void slotBitor(void);
        void slotCompl(void);
        void slotNot(void);
        void slotNot_eq(void);
        void slotOr(void);
        void slotOr_eq(void);
        void slotXor(void);
        void slotXor_eq(void);
        void slotTrue(void);
        void slotFalse(void);
        void slotPrivate(void);
        void slotProtected(void);
        void slotPublic(void);
        void slotFriend(void);
        void slotVirtual(void);
        void slotExtern(void);
        void slotInline(void);
        void slotStatic(void);
        void slotCase(void);
        void slotBreak(void);
        void slotContinue(void);
        void slotDo_while(void);
        void slotElse(void);
        void slotFor(void);
        void slotGoto(void);
        void slotIf(void);
        void slotReturn(void);
        void slotSwitch(void);
        void slotWhile(void);
        void slotTry(void);
        void slotCatch(void);
        void slotThrow(void);
        void slotRGB(void);
        void slotHelpKuzya(void);
        void slotOpenRecentFile(QString);
        bool slotSaveChangesNotifier(void);
        void slotShowAutoComplete(void);
        void slotShowNotificationList(bool);
        void slotShowErrorFromList();
        void slotGotoErrorLine(QListWidgetItem * item);
        void slotUseNativeMode(bool);

        void slotToggleFolds(void);
        void slotZoomDef(void);
        void slotZoomIn(void);
        void slotZoomOut(void);
protected:
        void closeEvent(QCloseEvent *event);
//	void keyPressEvent(QKeyEvent *event);
private:
        void paintErrorMarkers(QList<Compiler::compilerError>* errorList);
        void translateCode(translationEnum);


private:
        QFile *file;
        QsciScintilla* textEditor;
        QListWidget* notificationList;
        QString fileName;
        QString newFileName;
        QString DefaultDir;
        //QString CurrentDir;
        GoToLineDialog* goToLine;
        OptionsDialog* settings;
        Compiler *compiler;
        QLabel* statusLabel;
        int errorMarker, warningMarker, currentMarker;
        QVector <QAction*> ActOpenRecentFileVector;
        QSignalMapper *signalMapper;
        int MaxCount_RFileList;
        QList<QString>* RFileList;
        QsciPrinter* printer;
        FindDialog* findText;
        ReplaceDialog* replaceText;
        QShortcut* shortcut;
        QsciLexerCPP *cppLexer;
        QString translatedFileName;
        bool nativeMode;
public:


};

#endif

