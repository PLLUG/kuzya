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



#ifndef KUZYA_H
#define KUZYA_H

#include <QMainWindow>
#include <QProcess>

#include "optionsdialog.h"
#include "ui_kuzya.h"
#include "compiler.h"
#include "variable.h"
#include <map>

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
class QsciPrinter;
class QsciLexerCPP;
class QsciLexerPascal;
class QsciLexerFortran;
class QsciLexerJava;
template <typename T> class QVector;
template <typename T> class QList;
class Translator;
class QsciLexer;
class QFileDialog;
class Gdb;
class QTabWidget;
class QTreeWidget;
class QTreeWidgetItem;

class Kuzya: public QMainWindow, private Ui::kuzyaForm
{
    Q_OBJECT

public:

    enum notificationItemDataRoleEnum {attachedRole = Qt::UserRole+1,
                                       lineRole = Qt::UserRole+2,
                                       descriptionRole = Qt::UserRole+3};

    enum notificationTypeEnum {NTYPE_ERROR, NTYPE_WARNING, NTYPE_SUCCESS,
                               NTYPE_FAILING, NTYPE_WAIT, NTYPE_INFO, NTYPE_COMPILER};
    enum marginMarkerMaskTypeEnum {WARNING_MARK = 0x1,
                                ERROR_MARK = 0x2,
                                CURRENT_MARK = 0x4,
                                BREAKPOINT_MARK = 0x8};

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

        void refreshProfileSettings();
        void LoadTemplates(QString);
        void updateDebugger(const QString& debuggerLocation);

protected:

private slots:
        void slotUpdateStatusLabel(int line, int pos);
        void slotUpdateWindowName(bool m);
        void slotNew(void);
        void slotOpen(void);
        bool slotSave(void);
        void slotSave_as(void);
        void slotPrint(void);
        void slotExit(void);
        void slotRun(void);
        void slotCompile(void);
        void slotAbout(void);
        void slotAfterCompile(int status);
        void slotMarginClicked(int margin,int line,Qt::KeyboardModifiers);
        void slotHelpKuzya(void);
        void slotOpenRecentFile(QString);
        bool slotSaveChangesNotifier(void);
        void slotShowAutoComplete(void);
        void slotShowNotificationList(bool);
        void slotShowErrorFromList(void);
        void slotGotoErrorLine(QListWidgetItem *);
        void slotChangeTranslation(QString);

        void slotPasteTemplate(QString);
        void slotToggleFolds(void);
        void slotZoomDef(void);
        void slotZoomIn(void);
        void slotZoomOut(void);

        void slotDefaultMode(void);
        void slotAlternativeMode(void);
        void slotObjectMode(void);
        void slotStaticLibMode(void);
        void slotDynamicLibMode(void);

        void slotModificationChanged(bool);

        void slotSetFileSuffix(QStringList);

        void setUndoRedoEnabled();
        void slotRunDebugMode();
        void slotDebuggerHitBreakpoint(int line);
        void slotDebuggerUpdated();
        /* update watch section */
        void addTreeRoot(Variable var);
        void addTreeChild(QTreeWidgetItem *parent,
                          Variable var, QString prefix, bool internal);
        void addTreeChildren(QTreeWidgetItem* parrent,
                          Variable var, QString prefix, bool drfPointer = false);

        void moidifyTreeItemPointer(QTreeWidgetItem* itemPointer);
        /* end section */

protected:
        //*DRAG AND DROP
        void dragEnterEvent(QDragEnterEvent *event);
        void dropEvent(QDropEvent *event);
        void closeEvent(QCloseEvent *event);
//	void keyPressEvent(QKeyEvent *event);
private:
        void paintErrorMarkers(QList<Compiler::compilerError>* errorList);
        void paintWarningMarkers(QList<Compiler::compilerWarning>* warningList);
        void addNotification(int type, QString descr, bool attached = false, int line = -1);
        void removeAllNotifications();
        void loadTemplates(QString);
        void unloadTemplates();
        void refreshCompileModes();
        void refreshDialogSettings();
        void setAllIconsVisibleInMenu(bool isVisible);
        bool recompile();

private:
        QFile *file;
        QString language; //curren programing language
        QsciScintilla* textEditor;
        QTabWidget *mOutputTabWidget;
        QTreeWidget* mWatchLocalsWidget;
        QListWidget* notificationList;
        QString fileName;
        QString DefaultDir;
        //QString CurrentDir;
        GoToLineDialog* goToLine;
        OptionsDialog* settings;
        Compiler *compiler;
        QLabel* statusLabel;
        int errorMarker, warningMarker, currentMarker;
        int breakpointMarker;
        QVector <QAction*> ActOpenRecentFileVector;
        QSignalMapper *signalMapper;
        int MaxCount_RFileList;
        QList<QString>* RFileList;
        QsciPrinter* printer;
        FindDialog* findText;
        ReplaceDialog* replaceText;
        QShortcut* shortcut;
        QsciLexerCPP *cppLexer;
        QsciLexerPascal *pascalLexer;
        QsciLexerFortran *fortranLexer;
        QsciLexerJava *javaLexer;
        QString translatedFileName;
        Translator *translator;
        QComboBox *languageComboBox;
        QActionGroup *compilerModeGroup;
        QsciLexer *currentLexer;
        QAction *languageComboBoxAction;

        QSettings *tlist;
        QVector <QAction*> templlateAct;
        QSignalMapper *templatesSignalMapper;
        QStringList templatesCroupsList;

        bool srcRecompiled;
        QFileDialog *fileDialog;

        QToolBar *toolBar;
        Gdb* mGdbDebugger;
        std::map<QTreeWidgetItem*, Variable> mPointersName;
};

#endif
