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

#include <QList>
#include <QtGui>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpascal.h>
#include <Qsci/qscilexerfortran.h>
#include <Qsci/qscilexerjava.h>
#include <QShortcut>
#include <QVector>
#include <QSignalMapper>
#include <QKeyEvent>
#include <Qsci/qsciprinter.h>
#include <QPrintDialog>
#include <QColor>
#include <QDebug>
#include <QToolBar>
#include <QSplitter>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QTextStream>
#include <QFile>
#include <QVBoxLayout>
#include <QDialog>


#include "gotolinedialog.h"
#include "finddialog.h"
#include "replacedialog.h"
#include "compiler.h"
#include "kuzya.h"
#include "helpbrowser.h"
#include "translator.h"
#include "version.h"



Kuzya::Kuzya(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    setObjectName("Kuzya");
    setWindowTitle("Kuzya");
    languageComboBox = new QComboBox(this);
    toolBar = addToolBar("General");
    toolBar->setObjectName("KuzyaToolBar");
    toolBar->setIconSize(QSize(30,30));
    toolBar->addAction(actionNew);
    toolBar->addAction(actionOpen);
    toolBar->addAction(actionSave);
#ifdef Q_OS_MAC
#else
    toolBar->addSeparator();
#endif
    toolBar->addAction(actionUndo);
    toolBar->addAction(actionRedo);
    actionCut->setShortcuts(QKeySequence::Cut);
    toolBar->addAction(actionCut);
    actionCopy->setShortcuts(QKeySequence::Copy);
    toolBar->addAction(actionCopy);
    actionPaste->setShortcuts(QKeySequence::Paste);
    toolBar->addAction(actionPaste);
#ifdef Q_OS_MAC
#else
    toolBar->addSeparator();
#endif
    toolBar->addAction(actionNotificationList);
#ifdef Q_OS_MAC
#else
    toolBar->addSeparator();
#endif
    toolBar->addAction(actionCompile);
    toolBar->addAction(actionRun);
#ifdef Q_OS_MAC
#else
    toolBar->addSeparator();
#endif
    languageComboBoxAction = toolBar->addWidget(languageComboBox);
    languageComboBoxAction->setVisible(false);

    compilerModeGroup = new QActionGroup(this);
    compilerModeGroup->addAction(actionDefaultMode);
    compilerModeGroup->addAction(actionObjectMode);
    compilerModeGroup->addAction(actionStaticLibMode);
    compilerModeGroup->addAction(actionDynamicLibMode);
    compilerModeGroup->addAction(actionAlternativeMode);

    menuCompilation_Mode->setDisabled(true);
    menuTemplates->setDisabled(true);
    actionCompile->setDisabled(false);
    actionRun->setDisabled(true);

    statusLabel = new QLabel(this);
    statusBar()->addPermanentWidget(statusLabel);

    QGridLayout *gridLayout;
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

    textEditor = new QsciScintilla(this);
    textEditor->setEolMode(QsciScintilla::EolUnix);

    notificationList = new QListWidget(this);
    notificationList->setObjectName("notificationList");
    notificationList->setVisible(false);

    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(textEditor);
    splitter->addWidget(notificationList);
    splitter->setChildrenCollapsible(false);
    splitter->setStretchFactor(0, 5);
    splitter->setStretchFactor(1, 2);
    splitter->setHandleWidth(5);

    gridLayout->addWidget(splitter, 0, 0, 1, 1);

    textEditor->setCaretLineBackgroundColor(QColor(215, 215, 250));
    textEditor->setCaretLineVisible(true);

    QFont font("Courier", 10);

    textEditor->setFont(font);

    cppLexer = new QsciLexerCPP(this);
    cppLexer->setFont(font);
//    cppLexer->
    pascalLexer = new QsciLexerPascal(this);
    pascalLexer->setFont(font);
    fortranLexer = new QsciLexerFortran(this);
    fortranLexer->setFont(font);
    javaLexer = new QsciLexerJava(this);
    javaLexer->setFont(font);

    textEditor->setLexer();

    textEditor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
    textEditor->setMatchedBraceBackgroundColor(QColor(200, 100, 100));

    textEditor->setSelectionBackgroundColor(QColor(100, 100, 200));
    textEditor->setUtf8(true);


    warningMarker = textEditor->markerDefine(QPixmap(":/markers/warning_line","",Qt::AutoColor));
    errorMarker = textEditor->markerDefine(QPixmap(":/markers/bug_line","",Qt::AutoColor));
    currentMarker = textEditor->markerDefine(QPixmap(":/markers/current_line","",Qt::AutoColor));

    textEditor->setMarginMarkerMask(1,3);
    textEditor->setMarginMarkerMask(2,4);
    textEditor->setMarginWidth(1, 15);
    textEditor->setMarginWidth(2, 20);

    textEditor->setMarginSensitivity(1, true);
    //textEditor->setMarginsBackgroundColor(QColor(190, 178, 157,255));

    file = new QFile();
    goToLine = new GoToLineDialog(textEditor);
    compiler = new Compiler(this);
    translator = new Translator(this);
    settings = new OptionsDialog(this);

    findText = new FindDialog(textEditor);
    replaceText = new ReplaceDialog(textEditor);
    signalMapper= new QSignalMapper(this);

    RFileList = new QList<QString>();

    DefaultDir=DefaultDir;
    shortcut = new QShortcut(textEditor);
    shortcut->setKey(Qt::CTRL+Qt::Key_Space);


    settings->readODWSettings();
    settings->openLastProject();
    settings->readMainWindowState();
    if(settings->getIsFileReopenEnabled())
    {
        readTemporaryFileState();
    }
    ActOpenRecentFileVector.clear();

    srcRecompiled = false;

    fileDialog = new QFileDialog(this);
    QList<QUrl> list = fileDialog->sidebarUrls();
    list << QUrl::fromLocalFile(DefaultDir);
    fileDialog->setSidebarUrls(list);

    ///-------------------------------------------------------------------------------------------------------------------

    connect(actionNew,	SIGNAL(triggered()),this,		SLOT(slotNew()));
    connect(actionOpen, 	SIGNAL(triggered()),this,		SLOT(slotOpen()));
    connect(actionSave,	SIGNAL(triggered()),this,		SLOT(slotSave()));
    connect(actionSave_as,	SIGNAL(triggered()),this,		SLOT(slotSave_as()));
    connect(actionPrint,	SIGNAL(triggered()),this,		SLOT(slotPrint()));
    connect(actionExit, 	SIGNAL(triggered()), this,		SLOT(slotExit()));
    connect(actionUndo, 	SIGNAL(triggered()), textEditor,	SLOT(undo()));
    connect(actionRedo, 	SIGNAL(triggered()), textEditor,	SLOT(redo()));
    connect(actionCut, 	SIGNAL(triggered()), textEditor,	SLOT(cut()));
    connect(actionCopy, 	SIGNAL(triggered()), textEditor,	SLOT(copy()));
    connect(actionPaste,	SIGNAL(triggered()), textEditor,	SLOT(paste()));
    connect(actionSelect_all,SIGNAL(triggered()), textEditor,	SLOT(selectAll()));
    connect(actionGoToLine, SIGNAL(triggered()), goToLine, 		SLOT(slotGoToLine()));
    connect(actionGoToMatchingBracket, SIGNAL(triggered()),textEditor ,SLOT(moveToMatchingBrace()));
    connect(actionCompile, 	SIGNAL(triggered()), this, 		SLOT(slotCompile()));
    connect(actionRun, 	SIGNAL(triggered()),this,		SLOT(slotRun()));
    connect(actionCommon,	SIGNAL(triggered()), settings, 		SLOT(slotCommOptions()));
    connect(actionAbout, 	SIGNAL(triggered()),this,		SLOT(slotAbout()));
    connect(actionKuzya_Help,	SIGNAL(triggered()),this,	SLOT(slotHelpKuzya()));
    connect(actionFind,	SIGNAL(triggered()),findText,		SLOT(slotFindDialog()));
    connect(actionReplace,	SIGNAL(triggered()),replaceText,	SLOT(slotReplaceDialog()));
    connect(textEditor,	SIGNAL(cursorPositionChanged (int, int)),this,	SLOT(slotUpdateStatusLabel(int, int)));
    connect(textEditor,	SIGNAL(modificationChanged(bool)),this,	SLOT(slotUpdateWindowName(bool)));
    connect(textEditor,	SIGNAL(marginClicked (int, int, Qt::KeyboardModifiers)), this, SLOT(slotMarginClicked(int, int, Qt::KeyboardModifiers)));
    connect(compiler,	SIGNAL(compileEnded(int)),	this,	SLOT(slotAfterCompile(int)));

    connect(actionNotificationList, SIGNAL(toggled(bool)), this,    SLOT(slotShowNotificationList(bool)));
    connect(notificationList, SIGNAL(itemSelectionChanged()), this, SLOT(slotShowErrorFromList()));
    connect(notificationList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(slotGotoErrorLine(QListWidgetItem*)));

    connect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeTranslation(QString)));

    connect ( actionDefFontSize,	SIGNAL ( triggered() ),	this,	SLOT ( slotZoomDef() ) );
    connect ( actionEnlFont,	SIGNAL ( triggered() ),	this,	SLOT ( slotZoomIn() ) );
    connect ( actionShrinkFont,	SIGNAL ( triggered() ),	this,	SLOT ( slotZoomOut() ) );
    connect	(actionToggleFolds,	SIGNAL(triggered()),	this,	SLOT (slotToggleFolds()));
    connect ( actionAbout_Qt,	SIGNAL ( triggered() ),	qApp,	SLOT ( aboutQt() ) );

    connect(actionDefaultMode, SIGNAL(triggered()), this,           SLOT(slotDefaultMode()));
    connect(actionAlternativeMode, SIGNAL(triggered()), this,       SLOT(slotAlternativeMode()));
    connect(actionObjectMode, SIGNAL(triggered()), this,            SLOT(slotObjectMode()));
    connect(actionStaticLibMode, SIGNAL(triggered()), this,         SLOT(slotStaticLibMode()));
    connect(actionDynamicLibMode, SIGNAL(triggered()), this,        SLOT(slotDynamicLibMode()));





    connect(textEditor, SIGNAL(textChanged()), this, SLOT(setUndoRedoEnabled()));





    connect(textEditor,	SIGNAL(modificationChanged(bool)), this,SLOT(slotModificationChanged(bool)));

    connect(fileDialog, SIGNAL(filterSelected(QString)), this,      SLOT(slotSetFileSuffix(QString)));

    statusBar()->showMessage(tr("Ready"));

    QShortcut *notificationListShortcut = new QShortcut(textEditor);
    notificationListShortcut->setKey(Qt::CTRL+Qt::Key_Down);
    connect(notificationListShortcut, SIGNAL(activated()), notificationList, SLOT(setFocus()));

    QShortcut *textEditorShortcut = new QShortcut(textEditor);
    textEditorShortcut->setKey(Qt::CTRL+Qt::Key_Up);
    connect(textEditorShortcut, SIGNAL(activated()), textEditor, SLOT(setFocus()));

//    if (qApp->argc() > 1)
//    {
//        this->openFile(qApp->argv()[qApp->argc()-1]);
//    }

#ifdef Q_OS_MAC
    setAllIconsVisibleInMenu(false);
    setUnifiedTitleAndToolBarOnMac(true);
#endif
}
///___________________________________________________________________________________________________________________
/**
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**/
Kuzya::~Kuzya()
{

}

/**
________________________________________________________________________________________________________________________
PUBLIC		PUBLIC		PUBLIC		PUBLIC		PUBLIC		PUBLIC		PUBLIC		PUBLIC	|
________________________________________________________________________________________________________________________|
**/


void Kuzya::setAutoCompletionEnabled(bool b)
{
    if(b)
    {
        connect(shortcut,SIGNAL(activated()),this,SLOT(slotShowAutoComplete()));
    }
    else
    {
        try
        {
            disconnect(shortcut,SIGNAL(activated()),this,SLOT(slotShowAutoComplete()));
        }catch(int e)
        {
            Q_UNUSED(e)
        }
    }
}


///***********************************************************************************************************///

void Kuzya::loadPascalLexer(void)
{
    QsciLexerPascal *pascalLexer = new QsciLexerPascal(this);
    textEditor->setLexer(pascalLexer);
}
///***********************************************************************************************************///
void Kuzya::toggleFoldsActionEnabled(bool b)
{
    actionToggleFolds->setEnabled(b);
}
///***********************************************************************************************************///
QsciScintilla* Kuzya::getTextEditorPointer(void)
{
    return textEditor;
}
///***********************************************************************************************************///
void Kuzya::loadCPPLexer(void)
{
    cppLexer = new QsciLexerCPP(this);
    textEditor->setLexer(cppLexer);
}
///***********************************************************************************************************///
void Kuzya::setEditorFont(QFont font)
{
    cppLexer->setFont(font,-1);
}
///***********************************************************************************************************///
QFont Kuzya::getEditorFont(void)
{
    return cppLexer->font(-1);
}
///***********************************************************************************************************///
void Kuzya::retranslateAll(void)
{
    findText->retranslate();
    replaceText->retranslate();
    goToLine->retranslate();
    settings->retranslate();
    retranslateUi(this);
    int line;
    int index;
    textEditor->getCursorPosition(&line,&index);
    slotUpdateStatusLabel(line,index);

}
///***********************************************************************************************************///
void Kuzya::setDefaultDir(QString dir)
{
    DefaultDir=dir;
}
///***********************************************************************************************************///
void Kuzya::markerDeleteAll(void)
{
    textEditor->markerDeleteAll();
}
///***********************************************************************************************************///
void Kuzya::setText(QString text)
{
    textEditor->setText(text);
}
///***********************************************************************************************************///
void Kuzya::setMWTitle(QString title)
{
    fileName=title;
    slotUpdateWindowName(false);
}
///***********************************************************************************************************///
void Kuzya::setTEModified(bool status)
{
    textEditor->setModified(status);
}
///***********************************************************************************************************///
void Kuzya::setFont(QFont Font)
{
    textEditor->setFont(Font);
}
///***********************************************************************************************************///
void Kuzya::retranslate(void)
{
    retranslateUi(this);
}
/**
*********openFile*****************************************************************************************************
**/

void Kuzya::openFile(QString FileName)
{
    fileName=FileName;
    if (FileName.isEmpty()) return;

    file->setFileName(FileName);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QTextStream stream(file);
    textEditor->setText(stream.readAll());
    file->close();

    if(settings->isLineMarginVisible) textEditor->setMarginWidth(3,QVariant(textEditor->lines()).toString());

    textEditor->setModified(false);

    statusBar()->showMessage(tr("Opened"), 2000);

    slotUpdateWindowName(false);

    addFileNameToList(FileName);
    settings->saveLastProjectName(fileName);

    refreshProfileSettings();
    removeAllNotifications();
}

/**
*******************************************************************************************************
**/
void Kuzya::cleanMemory_ActOpenRecentFileVector(void)
{
    disconnect(signalMapper, SIGNAL(mapped(QString)),this, SLOT(slotOpenRecentFile(QString)));
    if(!ActOpenRecentFileVector.isEmpty())
    {
        for(int i=getFileListCount()-1;i>=0;i--)
        {
            delete ActOpenRecentFileVector[i];
            signalMapper->removeMappings(ActOpenRecentFileVector[i]);
        }

    }
}
/**
*******************************************************************************************************
**/
void Kuzya::slotUpdateRecentFileList(void)
{
    ActOpenRecentFileVector.clear();
    menuRecent_Files->clear();
    for(int i=getFileListCount()-1;i>=0;i--)
    {
        ActOpenRecentFileVector.push_front(new QAction(RFileList->at(i),this));
    }
    for(int i=getFileListCount()-1;i>=0;i--)
    {
        menuRecent_Files->addAction(ActOpenRecentFileVector[i]);
        connect(ActOpenRecentFileVector[i],SIGNAL(triggered()),signalMapper,SLOT(map()));
        signalMapper->setMapping(ActOpenRecentFileVector[i],ActOpenRecentFileVector[i]->text());
    }
    connect(signalMapper,SIGNAL(mapped(QString)),this,SLOT(slotOpenRecentFile(QString)));
}



/**
***********getFileListCount****************************************************************************************
**/
int Kuzya::getFileListCount(void)
{
    return RFileList->count();
}
/**
*******addToRecentFilesMenu****************************************************************************************
**/
void Kuzya::addFileNameToList(QString FN)
{
    cleanMemory_ActOpenRecentFileVector();
    if(MaxCount_RFileList!=0)
    {
        if(!menuRecent_Files->isEnabled()) menuRecent_Files->setEnabled(true);
        if(RFileList->indexOf(FN)!=-1)
        {
            RFileList->move(RFileList->indexOf(FN),RFileList->count()-1);
        }
        else
        {
            if(RFileList->count()<MaxCount_RFileList)
            {
                RFileList->push_back(FN);
            }
            else
            {
                RFileList->removeFirst();
                RFileList->push_back(FN);
            }
        }
        slotUpdateRecentFileList();
    }
    else
    {
        if(menuRecent_Files->isEnabled()) menuRecent_Files->setEnabled(false);
    }
}
/**
**********resizeRFileList*****************************************************************************************
**/
void Kuzya::resizeRFileList(int NewCount)
{
    if(NewCount<RFileList->count())
    {
        for(int i=RFileList->count()-NewCount;i>0;i--)
        {
            RFileList->removeFirst();//changing size of list
        }

    }
    if(NewCount==0)
    {
        menuRecent_Files->setEnabled (false);
    }
    cleanMemory_ActOpenRecentFileVector();
    slotUpdateRecentFileList();
}
/**
**************getRFileListAt****************************************************************************************
**/
QString Kuzya::getRFileListAt(int i)
{
    return RFileList->at(i);
}
/**
********setMaxCount_RFileList****************************************************************************************
**/
void Kuzya::setMaxCount_RFileList(int count)
{
    MaxCount_RFileList=count;
}
/**
*******getMaxCount_RFileList****************************************************************************************
**/
int Kuzya::getMaxCount_RFileList(void)
{
    return MaxCount_RFileList;
}

/**
*******************************************************************************************************
**/
/**
________________________________________________________________________________________________________________________
PRIVATE 	PRIVATE		PRIVATE		PRIVATE		PRIVATE		PRIVATE		PRIVATE		PRIVATE	|
________________________________________________________________________________________________________________________|
**/

void Kuzya::slotNew(void)
{
    if(slotSaveChangesNotifier()==false) return;

    textEditor->markerDeleteAll();
    notificationList->clear();
    languageComboBox->clear();
    textEditor->setLexer(0);

    fileName = QString::null;
    translatedFileName = QString::null;

    textEditor->clear();
    textEditor->setMarginWidth(3,"12");
    setWindowTitle("Kuzya");
    statusBar()->showMessage(tr("Created new file"), 2000);
    textEditor->setModified(false);

    menuCompilation_Mode->setDisabled(true);
    menuTemplates->setDisabled(true);
    actionCompile->setDisabled(false);
    actionRun->setDisabled(false);
    languageComboBoxAction->setVisible(false);

    srcRecompiled  = false;
}

/**
*******************************************************************************************************
**/
void Kuzya::slotOpen(void)
{
    if(!slotSaveChangesNotifier())
    {
        return;
    }
    refreshDialogSettings();
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setDirectory(DefaultDir);
    QString openedFileName;
    if (QDialog::Accepted == fileDialog->exec())
    {
        openedFileName = fileDialog->selectedFiles().at(0);

        if ("" != openedFileName)
        {
            removeAllNotifications();
            openFile(openedFileName);
            setUndoRedoEnabled();
        }
    }
}

/**
*******************************************************************************************************
**/

void Kuzya::refreshCompileModes()
{
    actionDefaultMode->setChecked(true);
    compiler->setCompilerMode(Compiler::DEFAULT);
    actionAlternativeMode->setVisible(compiler->isModeAvailable(Compiler::ALTERNATIVE));
    actionObjectMode->setVisible(compiler->isModeAvailable(Compiler::OBJECT));
    actionStaticLibMode->setVisible(compiler->isModeAvailable(Compiler::STATIC_LIB));
    actionDynamicLibMode->setVisible(compiler->isModeAvailable(Compiler::DYNAMIC_LIB));
}

void Kuzya::slotDefaultMode()
{
    compiler->setCompilerMode(Compiler::DEFAULT);
}

void Kuzya::slotAlternativeMode()
{
    compiler->setCompilerMode(Compiler::ALTERNATIVE);
}

void Kuzya::slotObjectMode()
{
    compiler->setCompilerMode(Compiler::OBJECT);
}

void Kuzya::slotStaticLibMode()
{
    compiler->setCompilerMode(Compiler::STATIC_LIB);
}

void Kuzya::slotDynamicLibMode()
{
    compiler->setCompilerMode(Compiler::DYNAMIC_LIB);
}

void Kuzya::slotSetFileSuffix(QStringList filter)
{
    if (filter.count() > 0) {
        fileDialog->setDefaultSuffix(filter.at(0));
    }
}

void Kuzya::setUndoRedoEnabled()
{
    actionUndo->setEnabled(textEditor->isUndoAvailable());
    actionRedo->setEnabled(textEditor->isRedoAvailable());
}



void Kuzya::refreshDialogSettings()
{
    QString filter;
    QStringList supportedList = compiler->getSupportedLanguages();
    supportedList.sort();
    foreach (QString lang, supportedList)
    {
        filter = filter+lang+" ("+compiler->getSupportedExtensions(lang)+");;";
    }
    filter = filter+"All Files (*)";

    fileDialog->setNameFilter(filter);
    QList<QUrl> list = fileDialog->sidebarUrls();
    if(!list.isEmpty())
    {
        list.removeLast();
    }
    list << QUrl::fromLocalFile(DefaultDir);
    fileDialog->setSidebarUrls(list);
    QString currentFilter;
    if (!language.isEmpty()) currentFilter = language + " ("+compiler->getSupportedExtensions(language)+")";
    else currentFilter = "";
    fileDialog->selectFile(currentFilter);
    slotSetFileSuffix(fileDialog->selectedFiles());
}

void Kuzya::refreshProfileSettings()
{
    languageComboBoxAction->setVisible(false);

    if (fileName.isEmpty()) return;

    QStringList supportedList = compiler->getSupportedLanguages();
    QString ex(fileName);
    ex = ex.remove(0, ex.lastIndexOf("."));
    ex = ex.toLower();

    foreach (QString lang, supportedList)
    {
        if (compiler->getSupportedExtensions(lang).contains(ex))
        {
            language = lang;
            break;
        }
    }

    if (!language.isEmpty())
    {
        QString comp = settings->readDefaultCompiler(language);
        if (comp.isEmpty())
        {
            QStringList supportedCompilers = compiler->getSupportedCompilers(language);
            if (!supportedCompilers.isEmpty())
                comp = compiler->getSupportedCompilers(language).at(0);
            else return;
        }
        compiler->loadProfile(language, comp);
        compiler->setCompilerDir(settings->readCompilerLocation(language, comp));
        compiler->setOptions(settings->readCompilerOptions(language, comp));

        menuCompilation_Mode->setDisabled(false);
        refreshCompileModes();
        actionCompile->setDisabled(false);
        actionRun->setDisabled(false);

        srcRecompiled = false;

#ifdef WIN32
        QString path = QApplication::applicationDirPath();
        path.truncate(path.lastIndexOf("/", -1));
        path = path+"/profiles/";
#else
        //        QString path = QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/profiles/");
        QString path = QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/profiles");
        if (false == QDir(path).exists())
        {
            path = QDir::cleanPath(QApplication::applicationDirPath() + "/../profiles");
            if (false == QDir(path).exists())
            {
                QMessageBox msgBox;
                msgBox.setText("There is some problem with loading of templates");
                msgBox.exec();
            }
        }
#endif
        unloadTemplates();
        loadTemplates(path+"/"+language+"/"+language+".ini");

        if ("pascal" == language) currentLexer = pascalLexer;
        else if ("c++" == language || "obj-c" == language) currentLexer = cppLexer;
        else if ("fortran" == language) currentLexer = fortranLexer;
        else if ("java" == language) currentLexer = javaLexer;
        else currentLexer = 0;
        textEditor->setLexer(currentLexer);
    }
    else compiler->loadProfile("","");


#ifdef WIN32
    QString path = QApplication::applicationDirPath();
    path.truncate(path.lastIndexOf("/", -1));
    path = path+"/resources/";
#else
    QString path = QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/resources/");
#endif

    translator->openFile(fileName, language);
    compiler->openFile(translator->codeFile());

    QStringList supportedTranslations = translator->getSupportedTranslations(language);
    if (supportedTranslations.isEmpty())
    {
        return;
    }
    else
    {
        disconnect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeTranslation(QString)));
        languageComboBox->clear();
        languageComboBox->addItem(QIcon(path+"english.png"), "english (default)");
        foreach (QString trans, supportedTranslations)
        {
            languageComboBox->addItem(QIcon(path+trans+".png"), trans);
        }
        QString codeTranslation = translator->translation();
        languageComboBox->setCurrentIndex(supportedTranslations.indexOf(codeTranslation)+1);
        connect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeTranslation(QString)));
        languageComboBoxAction->setVisible(true);
    }
}

/**
*******************************************************************************************************
**/
bool Kuzya::slotSave(void)
{
    QString newFileName;
    if (fileName.isEmpty())
    {
        // fileName = fileDialog->getSaveFileName(this, tr("Save as..."), DefaultDir, filter, &currentFilter);
        refreshDialogSettings();
        fileDialog->setAcceptMode(QFileDialog::AcceptSave);
        fileDialog->setFileMode(QFileDialog::AnyFile);

        if (fileDialog->exec())
            newFileName = fileDialog->selectedFiles().at(0);

        if (newFileName.isEmpty()) return false;
        else fileName = newFileName;
    }


    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox *msgBox= new QMessageBox();
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setWindowTitle(tr("File cannot be saved "));
        msgBox->setText(tr("Do you have permision to access data in this folder? Select another place to save this file"));
        QAbstractButton *OkBtn = msgBox->addButton(tr("Ok"),QMessageBox::ActionRole);
        msgBox->exec();
        if (msgBox->clickedButton()==(OkBtn))
        {
            //slotSave_as();
        }
        delete msgBox;
        return false ;
    }
    QTextStream stream(&file);
    stream << textEditor->text();
    textEditor->setModified(false);
    file.close();
    statusBar()->showMessage(tr("Saved"), 2000);
    addFileNameToList(file.fileName());
    refreshProfileSettings();
    slotUpdateWindowName(false);
    if(settings->isLineMarginVisible) textEditor->setMarginWidth(3,QVariant(textEditor->lines()).toString());
    return true;
}

/**
*******************************************************************************************************
**/
void Kuzya::slotSave_as(void)
{
    QString oldFileName(fileName);
    QString newFileName;
    //        newFileName = fileDialog->getSaveFileName(this, tr("Save as..."),
    //                                           DefaultDir , filter, &currentFilter);

    refreshDialogSettings();
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setFileMode(QFileDialog::AnyFile);

    if (fileDialog->exec())
        newFileName = fileDialog->selectedFiles().at(0);

    if (newFileName.isEmpty()) return;

    fileName = newFileName;
    if (!slotSave()) fileName = oldFileName;
}
/**
*******************************************************************************************************
**/
void Kuzya::slotPrint(void)
{
    printer = new QsciPrinter(QPrinter::ScreenResolution);
    //  QsciScintillaBase* qsb;
    // qsb=new QsciScintillaBase(textEditor);
    //formatPage(QPainter&painter,bool drawing,QPrinter::setFullPage(),int   pagenr );
    //  printer->printRange(qsb,-1,-1);
    QPrintDialog printDialog(printer,textEditor);

    if(printDialog.exec()==QDialog::Accepted)
    {
        QTextDocument doc;
        doc.setPlainText(textEditor->text());
        doc.print(printer);
        /*     QPainter painter(printer);
                QRect rect(painter.viewport());
                painter.drawText(rect,textEditor->text());
              */

    }
    delete printer;
}
/**
*******************************************************************************************************
**/
void Kuzya::slotExit(void)
{
    settings->writeSettings();
    settings->writeMainWindowState();


    //        if (!fileName.isEmpty())
    //            settings->saveLastProjectName(fileName);

    close();
}
/**
*******************************************************************************************************
**/
void Kuzya::slotRun(void)
{
    if (fileName.isEmpty())
    {
        addNotification(NTYPE_FAILING, tr("No binary to run"));
        return;
    }
    if (!srcRecompiled)
    {
        slotCompile();
        compiler->waitForFinished(15000);
    }
    compiler->run();
}

/**
*******************************************************************************************************
**/
void Kuzya::slotCompile(void)
{
    if (textEditor->isModified()) slotSave();

    removeAllNotifications();
    textEditor->markerDeleteAll(currentMarker);
    if (fileName.isEmpty())
    {
        addNotification(NTYPE_FAILING, tr("No source to compile"));
        return;
    }

    textEditor->setReadOnly(true);
    if (compiler->isReady())
    {
        addNotification(NTYPE_WAIT, tr("Compilling..."));
        textEditor->setReadOnly(true);
        translator->retranslate();
        compiler->compile();
        srcRecompiled = true;
    }
    else addNotification(NTYPE_FAILING, tr("Could not open compiler profile"));
}

Compiler* Kuzya::getCurrentCompiler(void)
{
    return compiler;
}

/**
*******************************************************************************************************
**/
void Kuzya::slotAfterCompile(int status)
{
    textEditor->setReadOnly(false);
    notificationList->clear();
    if (Compiler::STATUS_NOERROR == status)
    {
        paintWarningMarkers(compiler->getLastWarnings());
        addNotification(NTYPE_SUCCESS, tr("Compiled successfuly!"));
    }
    else
    {
        if (Compiler::STATUS_FAILED_TO_START == status)
        {
            addNotification(NTYPE_FAILING, tr("Unable to start compiler"));
        }
        else
        {
            paintErrorMarkers(compiler->getLastErrors());
            paintWarningMarkers(compiler->getLastWarnings());
            addNotification(NTYPE_FAILING, tr("Compilation failed!"));
        }
    }

}

/**
*******************************************************************************************************
**/
void Kuzya::paintErrorMarkers(QList<Compiler::compilerError>* errorList)
{
    if (errorList->empty()) return;
    int errCount = 0;
    int firstAttached = 0;
    for (int i = 0; i < errorList->size(); ++i)
    {
        if (0 != errorList->at(i).line)
        {
            addNotification(NTYPE_ERROR, errorList->at(i).description, true, errorList->at(i).line);
            if (0 == firstAttached) firstAttached = i;
            errCount++;
        }
        else addNotification(NTYPE_COMPILER, errorList->at(i).description);
    }

    QString str;
    if (0 != errCount) addNotification(NTYPE_INFO, tr("Found errors (%2) in file %1").arg(fileName).arg(errCount));

    notificationList->setCurrentItem(notificationList->item(firstAttached));
    notificationList->setFocus();
}
/**
*******************************************************************************************************
**/
void Kuzya::paintWarningMarkers(QList<Compiler::compilerWarning>* warningList)
{
    if (warningList->empty()) return;
    int waCount = 0;
    for (int i = 0; i < warningList->size(); ++i)
    {
        if (0 != warningList->at(i).line)
        {
            addNotification(NTYPE_WARNING, warningList->at(i).description, true, warningList->at(i).line);
            waCount++;
        }
        else addNotification(NTYPE_COMPILER, warningList->at(i).description);
    }
    if (0 != waCount) addNotification(NTYPE_INFO, tr("Found warnings (%2) in file %1").arg(fileName).arg(waCount));
}

/**
*******************************************************************************************************
**/
void Kuzya::slotAbout(void)
{
    const QString fileName = ":/AUTHORS.txt";
    QFile file (fileName);
    //checks file
    if(!QFile::exists(fileName))
    {
        qCritical()<< "File doesn't exist"<<fileName<<endl;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qCritical()<<"couldn't open file"<<fileName;
    }

    QString textInf;
    QDialog* information {new QDialog};
    QVBoxLayout* layout {new QVBoxLayout(information)};
    information->setLayout(layout);
    QLabel* textInfLabel {new QLabel(information)};
    QLabel* imgLabel {new QLabel(information)};

    QTextStream in(&file);
    textInf  = in.readAll();

//set Kuzia version and build date
    QVersionNumber verKuzia(MAJORVER, MINORVER, REVISION);
    QDate buildDate(BUILD_YEAR, BUILD_MONTH, BUILD_DAY);
    textInfLabel->setText(textInf.arg(verKuzia.toString(), buildDate.toString("MMMM d yyyy")));


    imgLabel->setPixmap(QPixmap(":/common/Kuzya_about.png"));
    layout->addWidget(imgLabel);
    QScrollArea* scrollAreaAuthors = new QScrollArea(information);
    scrollAreaAuthors->setWidget(textInfLabel);
    layout->addWidget(scrollAreaAuthors);
    information->setWindowIcon(QIcon(QDir::currentPath()+":/../../src/images/kuzya.png"));

    information->exec();

    delete information;
}

/**
*******************************************************************************************************
**/
void Kuzya::slotUpdateStatusLabel(int line, int pos)
{
    statusLabel->setText(QString(tr("Line: %1 Pos: %2")).arg(line + 1).arg(pos));
}

/**
*******************************************************************************************************
**/
void Kuzya::slotUpdateWindowName(bool m)
{
    if (!fileName.isEmpty())
    {
        if (m) setWindowTitle(fileName.section('/', -1) + " - " + fileName + tr(" [modified] - kuzya"));
        else setWindowTitle(fileName.section('/', -1) + " - " + fileName + tr(" - kuzya"));
    }
}

/**
*******************************************************************************************************
**/
void Kuzya::slotMarginClicked(int margin, int line, Qt::KeyboardModifiers)
{
    if ((0 != textEditor->markersAtLine(line)) && (1 == margin))
    {
        QListWidgetItem *item = notificationList->findItems(QString(" %1)").arg(line+1), Qt::MatchContains).at(0);
        textEditor->markerDeleteAll(currentMarker);
        textEditor->markerAdd(line,currentMarker);
        notificationList->setCurrentItem(item);
        notificationList->setFocus();
        statusBar()->showMessage(item->data(Kuzya::descriptionRole).toString());
    }
}

/**
*******************************************************************************************************
**/
/**
***********slotSaveChangesNotifier*************************************************************************************
**/
bool Kuzya::slotSaveChangesNotifier(void)
{
    if (textEditor->isModified())
    {
        QMessageBox *msgBox= new QMessageBox();
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setWindowTitle(tr("File was modified"));
        msgBox->setText(tr("Do you want to save changes?"));
        QAbstractButton *yesBtn = msgBox->addButton(tr("Yes"),QMessageBox::ActionRole);
        QAbstractButton *noBtn = msgBox->addButton(tr("No"),QMessageBox::ActionRole);
        QAbstractButton *cancelBtn = msgBox->addButton(tr("Cancel"),QMessageBox::ActionRole);
        msgBox->setDefaultButton((QPushButton*)cancelBtn);
        /*int ret = */msgBox->exec();
        if (msgBox->clickedButton() ==cancelBtn)
        {
            return false;	// false - cancelbtn pressed
        }
        if (msgBox->clickedButton() ==yesBtn)
        {
            if(slotSave()==true)
            {
                addFileNameToList(fileName);
                delete msgBox;
                return true;
            }
            else
            {
                delete msgBox;
                return false;///false - Saving was canceled
            }
        }
        if (msgBox->clickedButton() ==noBtn)
        {
            delete msgBox;
            return true;
        }
    }
    return true;/// true - File was saved

}
/**
*******************************************************************************************************
**/
void Kuzya :: slotOpenRecentFile(QString FileName)
{
    if(slotSaveChangesNotifier()==false) return;
    if(QFile::exists(FileName))
    {
        openFile(FileName);
        //        refreshProfileSettings();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Sorry"));
        msgBox.setText(tr("This file wasn't founded"));
        msgBox.exec();
        RFileList->removeAt(RFileList->indexOf(FileName));
        slotUpdateRecentFileList();
    }
    setUndoRedoEnabled();
}
/**
*******************************************************************************************************
**/
void Kuzya::closeEvent(QCloseEvent *event)
{
    settings->writeSettings();
    settings->writeMainWindowState();
    if(!settings->getIsFileReopenEnabled())
    {
        if(slotSaveChangesNotifier()==false)
        {
            event->ignore();
        }
    }
    else
    {
        writeTemporaryFileState();
    }
    //        if (!fileName.isEmpty())
    //            settings->saveLastProjectName(fileName);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotShowAutoComplete()
{
    //textEditor->autoCompletionSource();
    textEditor->autoCompleteFromAll();

}
/**
*******************************************************************************************************
**/
void Kuzya::slotHelpKuzya()
{
#ifdef WIN32
    HelpBrowser* helpBrowser = new HelpBrowser(QApplication::applicationDirPath()+"/../doc/Kuzya_Help","main.htm");
#else
    //    HelpBrowser* helpBrowser = new HelpBrowser(QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/doc/"),"main.htm");
    QDir lHelpDir = QDir(QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/doc/"));
    if (lHelpDir.exists() == false)
    {
        lHelpDir = QDir(QDir::cleanPath(QApplication::applicationDirPath() + "/../doc/Kuzya_Help/"));
        if (lHelpDir.exists() == false)
        {
            QMessageBox msgBox;
            msgBox.setText("There is some problem with loading of help dicumentation");
            msgBox.exec();
        }
    }
    HelpBrowser* helpBrowser = new HelpBrowser(lHelpDir.absolutePath(),"main.htm");
#endif
    helpBrowser->resize(800,600);
    helpBrowser->show();
    //HelpAssistant::showPage("main.htm");
}
///***********************************************************************************************************///
void Kuzya::slotToggleFolds(void)
{
    textEditor->foldAll();
}

///***********************************************************************************************************///

void Kuzya::slotZoomDef(void)
{
    textEditor->zoomTo(0);
}
///***********************************************************************************************************///
void Kuzya::slotZoomOut(void)
{
    textEditor->zoomOut();
}
///***********************************************************************************************************///
void Kuzya::slotZoomIn(void)
{
    textEditor->zoomIn();
}
///***********************************************************************************************************///
void Kuzya::addNotification(int type, QString descr, bool attached, int line)
{
    QString str;
    QIcon icon;

    switch (type) {
    case NTYPE_ERROR:
        str = tr("Compilation error (line ")+QVariant(line).toString()+") "+descr;
        textEditor->markerAdd(line-1, errorMarker);
        icon.addFile(":/notifications/error");
        break;
    case NTYPE_WARNING:
        str = tr("Compilation warning (line ")+QVariant(line).toString()+") "+descr;
        icon.addFile(":/notifications/warning");
        textEditor->markerAdd(line-1, warningMarker);
        break;
    case NTYPE_SUCCESS:
        icon.addFile(":/notifications/success");
        statusBar()->showMessage(descr, 3000);
        break;
    case NTYPE_FAILING:
        icon.addFile(":/notifications/failing");
        statusBar()->showMessage(descr, 3000);
        slotShowNotificationList(true);
        break;
    case NTYPE_WAIT:
        icon.addFile(":/notifications/wait");
        statusBar()->showMessage(descr, 3000);
        break;
    case NTYPE_INFO:
        icon.addFile(":/notifications/info");
        break;
    case NTYPE_COMPILER:
        icon.addFile(":/notifications/comment");
        break;
    }

    if (str.isEmpty()) str = descr;

    QListWidgetItem *newItem = new QListWidgetItem(str);
    newItem->setData(Kuzya::attachedRole, QVariant(attached));
    newItem->setData(Kuzya::lineRole, QVariant(line));
    newItem->setData(Kuzya::descriptionRole,QVariant(descr));
    newItem->setIcon(icon);
    notificationList->addItem(newItem);
}
///***********************************************************************************************************///
void Kuzya::removeAllNotifications()
{
    textEditor->markerDeleteAll();
    notificationList->clear();
}
///***********************************************************************************************************///
void Kuzya::slotShowNotificationList(bool visible)
{
    notificationList->setVisible(visible);
    actionNotificationList->setChecked(visible);
}
///***********************************************************************************************************///
void Kuzya::slotShowErrorFromList()
{
    QListWidgetItem * item = notificationList->currentItem();
    textEditor->markerDeleteAll(currentMarker);
    if (item->data(Kuzya::attachedRole).toBool())
    {
        textEditor->setCursorPosition(item->data(Kuzya::lineRole).toInt()-1,1);
        textEditor->ensureCursorVisible();
        textEditor->markerAdd(item->data(Kuzya::lineRole).toInt()-1, currentMarker);
    }
}
///***********************************************************************************************************///
void Kuzya::slotGotoErrorLine(QListWidgetItem * item)
{
    textEditor->markerDeleteAll(currentMarker);
    if (item->data(Kuzya::attachedRole).toBool())
    {
        textEditor->setCursorPosition(item->data(Kuzya::lineRole).toInt()-1,1);
        textEditor->setFocus();
    }
}
/**
******************************************************************************************
***************** translate program code **************************************
*/
void Kuzya::slotChangeTranslation(QString translation)
{
    if (textEditor->isModified())
    {
        QMessageBox *msgBox= new QMessageBox();
        msgBox->setIcon(QMessageBox::Question);
        msgBox->setWindowTitle( tr("The file has been modified"));
        msgBox->setText(tr("Save your changes before translation?"));
        QAbstractButton *yesBtn = msgBox->addButton(tr("Yes"),QMessageBox::ActionRole);
        QAbstractButton *noBtn = msgBox->addButton(tr("No"),QMessageBox::ActionRole);
        Q_UNUSED(noBtn);
        QAbstractButton *cancelBtn = msgBox->addButton(tr("Cancel"),QMessageBox::ActionRole);
        msgBox->setDefaultButton((QPushButton*)cancelBtn);
        msgBox->exec();

        if (msgBox->clickedButton() == cancelBtn)
        {
            disconnect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeTranslation(QString)));
            int index = languageComboBox->findText(translator->translation());
            if (-1 != index) languageComboBox->setCurrentIndex(index);
            else languageComboBox->setCurrentIndex(0);
            connect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeTranslation(QString)));
            return;
        }
        else if (msgBox->clickedButton() ==yesBtn)
        {
            slotSave();
            disconnect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeTranslation(QString)));
            int index = languageComboBox->findText(translation);
            if (-1 != index) languageComboBox->setCurrentIndex(index);
            connect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeTranslation(QString)));
        }
    }

    if ("english (default)" == translation)
    {
        translation = "src";
    }

    translator->setTranslation(translation);
    qDebug() << "SET TRANSLATION: " << translation;

    openFile(translator->translatedCodeFile());
    qDebug() << "TR CODE: " << translator->translatedCodeFile();

#ifdef WIN32
    QString path = QApplication::applicationDirPath();
    path.truncate(path.lastIndexOf("/", -1));
    path = path+"/profiles/";
#else
    //    QString path = QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/profiles/");
    QString path = QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/profiles");
    if (false == QDir(path).exists())
    {
        path = QDir::cleanPath(QApplication::applicationDirPath() + "/../profiles");
        if (false == QDir(path).exists())
        {
            QMessageBox msgBox;
            msgBox.setText("There is some problem with loading of templates");
            msgBox.exec();
        }
    }
#endif
    unloadTemplates();
    loadTemplates(path+"/"+language+"/"+language+".ini");
}
///***********************************************************************************************************///
void Kuzya::slotPasteTemplate(QString keyStr)
{
    QStringList tempStrList;
    tlist->beginGroup("templates");
    tlist->beginReadArray(keyStr.left(keyStr.indexOf("/")));
    tlist->setArrayIndex((keyStr.right(keyStr.length()-keyStr.indexOf("/")-1)).toUInt());
    tempStrList<<tlist->value("template").toString();
    translator->translateList(&tempStrList);
    textEditor->removeSelectedText();
    textEditor->insert(tempStrList.at(0));
    tlist->endArray();
    tlist->endGroup();
}
///***********************************************************************************************************///
void Kuzya::loadTemplates(QString templatesPath)
{
    QStringList tempStrList1,tempStrList2;
    tlist=new QSettings(templatesPath,QSettings::IniFormat);
    tlist->beginGroup("templates");
    int size;
    templatesCroupsList=tlist->childGroups();
    tempStrList1=templatesCroupsList;
    if (!templatesCroupsList.isEmpty())
        menuTemplates->setDisabled(false);
    translator->translateList(&tempStrList1);
    templatesSignalMapper = new QSignalMapper(this);
    QMenu *menu;
    for(int j=0; j<templatesCroupsList.count();++j)
    {
        menu=menuTemplates->addMenu(QIcon(":/menu/templates_menu"), tempStrList1.at(j));
        size=tlist->beginReadArray(templatesCroupsList.at(j));
        for(int i=0;i<size;++i)
        {
            tlist->setArrayIndex(i);
            tempStrList2<<tlist->value("label").toString();
        }
        tlist->endArray();
        translator->translateList(&tempStrList2);
        size=tlist->beginReadArray(templatesCroupsList.at(j));
        for(int i=0;i<size;++i)
        {

            tlist->setArrayIndex(i);
            templlateAct.push_back(new QAction(QIcon(":/menu/template"),tempStrList2.at(i),this));
            menu->addAction(templlateAct[i]);
            connect(templlateAct[i],SIGNAL(triggered()),templatesSignalMapper,SLOT(map()));
            templatesSignalMapper->setMapping(templlateAct[i],templatesCroupsList.at(j)+"/"+QVariant(i).toString());
        }
        tlist->endArray();
        tempStrList2.clear();
        templlateAct.clear();
    }
    tlist->endGroup();
    connect(templatesSignalMapper,SIGNAL(mapped(QString)),this,SLOT(slotPasteTemplate(QString)));

}
///***********************************************************************************************************///
void Kuzya::unloadTemplates()
{
    menuTemplates->clear();
    menuTemplates->setDisabled(true);
    if(templatesSignalMapper==0) disconnect(templatesSignalMapper,SIGNAL(mapped(QString)),this,SLOT(slotPasteTemplate(QString)));
}
void Kuzya::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}
void Kuzya::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty()) return;
    QString fileName=urls.first().toLocalFile();

    textEditor->markerDeleteAll();
    notificationList->clear();
    openFile(fileName);
    //    refreshProfileSettings();
}

void Kuzya::slotModificationChanged(bool modified)
{
    if (modified) srcRecompiled = false;
}

void Kuzya::setAllIconsVisibleInMenu(bool isVisible)
{
    actionAbout->setIconVisibleInMenu(isVisible);
    actionAbout_Qt->setIconVisibleInMenu(isVisible);
    actionAlternativeMode->setIconVisibleInMenu(isVisible);
    actionCommon->setIconVisibleInMenu(isVisible);
    actionCompile->setIconVisibleInMenu(isVisible);
    actionCopy->setIconVisibleInMenu(isVisible);
    actionCut->setIconVisibleInMenu(isVisible);
    actionDefaultMode->setIconVisibleInMenu(isVisible);
    actionDefFontSize->setIconVisibleInMenu(isVisible);
    actionDynamicLibMode->setIconVisibleInMenu(isVisible);
    actionEnlFont->setIconVisibleInMenu(isVisible);
    actionExit->setIconVisibleInMenu(isVisible);
    actionFind->setIconVisibleInMenu(isVisible);
    actionGoToLine->setIconVisibleInMenu(isVisible);
    actionGoToMatchingBracket->setIconVisibleInMenu(isVisible);
    actionKuzya_Help->setIconVisibleInMenu(isVisible);
    actionNew->setIconVisibleInMenu(isVisible);
    actionNotificationList->setIconVisibleInMenu(isVisible);
    actionObjectMode->setIconVisibleInMenu(isVisible);
    actionOpen->setIconVisibleInMenu(isVisible);
    actionPaste->setIconVisibleInMenu(isVisible);
    actionPrint->setIconVisibleInMenu(isVisible);
    actionRedo->setIconVisibleInMenu(isVisible);
    actionReplace->setIconVisibleInMenu(isVisible);
    actionRun->setIconVisibleInMenu(isVisible);
    actionSave->setIconVisibleInMenu(isVisible);
    actionSave_as->setIconVisibleInMenu(isVisible);
    actionSelect_all->setIconVisibleInMenu(isVisible);
    actionShrinkFont->setIconVisibleInMenu(isVisible);
    actionStaticLibMode->setIconVisibleInMenu(isVisible);
    actionToggleFolds->setIconVisibleInMenu(isVisible);
    actionUndo->setIconVisibleInMenu(isVisible);




}

void Kuzya::writeTemporaryFileState()
{
    QSettings* settings = new QSettings(QApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat);
    QTemporaryFile tFile;
    settings->beginGroup("temp_file");
    QString tFileName = settings->value("file").toString();
    if(!tFileName.isEmpty())
    {
        tFile.setFileName(tFileName);
    }
    QString fileExtenstion = compiler->getSupportedExtensions(languageComboBox->currentText());
    if(!fileExtenstion.isEmpty())
    tFile.setFileName(tFile.fileName().append(".").append(fileExtenstion));
    tFile.setAutoRemove(false);
    tFile.open();
    QTextStream stream(&tFile);
    stream << textEditor->text();
    tFile.close();

    int line;
    int index;
    textEditor->getCursorPosition(&line, &index);

    settings->setValue("file", tFile.fileName());
    settings->setValue("cursor_pos/line", line);
    settings->setValue("cursor_pos/index", index);
    settings->endGroup();
    settings->sync();
}

void Kuzya::readTemporaryFileState()
{
    QSettings* settings = new QSettings(QApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat);
    settings->beginGroup("temp_file");
    QFile tFile;
    tFile.setFileName(settings->value("file").toString());
    if(tFile.open(QIODevice::ReadOnly))
    {
        openFile(tFile.fileName());
        int line = settings->value("cursor_pos/line").toInt();
        int index = settings->value("cursor_pos/index").toInt();
        textEditor->setCursorPosition(line,index);
    }
    settings->endGroup();
}
