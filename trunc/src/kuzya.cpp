/***************************************************************************
 *   Copyright (C) 2008 by Volodymyr Shevchyk                              *
 *   i'mnotageycom.ua                                                      *
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

#include <QList>
#include <QtGui>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerpascal.h>
#include <QShortcut>
#include <QVector>
#include <QSignalMapper>
#include <QKeyEvent>
#include <Qsci/qsciprinter.h>
#include <QPrintDialog>
#include <QColor>

#include "gotolinedialog.h"
#include "finddialog.h"
#include "replacedialog.h"
#include "compiler.h"
#include "kuzya.h"
#include "helpbrowser.h"


Kuzya::Kuzya(QWidget *parent)
        : QMainWindow(parent)
{
        setupUi(this);
        setObjectName("Kuzya");
        setWindowTitle("Kuzya");

        toolBar->addAction(actionNew);
        toolBar->addAction(actionOpen);
        toolBar->addAction(actionSave);
        toolBar->addSeparator();
        toolBar->addAction(actionUndo);
        toolBar->addAction(actionRedo);
        toolBar->addAction(actionCut);
        toolBar->addAction(actionCopy);
        toolBar->addAction(actionPaste);
        toolBar->addSeparator();
        toolBar->addAction(actionNotificationList);
        toolBar->addSeparator();
        toolBar->addAction(actionRun);
        toolBar->addAction(actionCompile);
        toolBar->addSeparator();
        toolBar->addAction(actionCode_language);

        statusLabel = new QLabel(this);
        statusBar()->addPermanentWidget(statusLabel);

        QGridLayout *gridLayout;
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        textEditor = new QsciScintilla(this);
        gridLayout->addWidget(textEditor, 0, 0, 1, 1);

        notificationList = new QListWidget(this);
        notificationList->setVisible(false);


        QSplitter *splitter = new QSplitter(this);
        splitter->setOrientation(Qt::Vertical);
        splitter->addWidget(textEditor);
        splitter->addWidget(notificationList);
        splitter->setChildrenCollapsible(false);
        splitter->setHandleWidth(5);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);


        textEditor->setCaretLineBackgroundColor(QColor(215, 215, 250));
        textEditor->setCaretLineVisible(true);

        //QsciLexerCPP *
        cppLexer = new QsciLexerCPP(this);
        //cppLexer->setAutoIndentStyle(QsciScintilla::AiClosing);
        textEditor->setLexer(cppLexer);

   //     textEditor->setAutoIndent(true);///!!!!!!!!!!!!!!!!!!!!!!!
/*
        QsciLexerPascal *pascalLexer = new QsciLexerPascal(this);
        textEditor->setLexer(pascalLexer);
*/
        textEditor->setBraceMatching(QsciScintilla::SloppyBraceMatch);
        textEditor->setMatchedBraceBackgroundColor(QColor(200, 100, 100));

        textEditor->setSelectionBackgroundColor(QColor(100, 100, 200));

        textEditor->setMarginSensitivity(1, true);
        textEditor->setMarginLineNumbers(2, true);
        textEditor->setMarginWidth(2, 40);

        textEditor->setUtf8(true);

        errorMarker = textEditor->markerDefine(QPixmap(":/markers/bug_line","",Qt::AutoColor));
        warningMarker = textEditor->markerDefine(QPixmap(":/markers/warning_line","",Qt::AutoColor));
        currentMarker = textEditor->markerDefine(QPixmap(":/markers/current_line","",Qt::AutoColor));

        file = new QFile();
        goToLine = new GoToLineDialog(textEditor);
        compiler = new Compiler(this);
        settings = new OptionsDialog(this);

        findText = new FindDialog(textEditor);
        replaceText = new ReplaceDialog(textEditor);
        signalMapper= new QSignalMapper(this);

        RFileList = new QList<QString>();

        DefaultDir=DefaultDir;
        shortcut = new QShortcut(textEditor);
        shortcut->setKey(Qt::CTRL+Qt::Key_Space);

        //textEditor->setIndentationGuides(true);
        //textEditor->setIndentationsUseTabs(true);
        //textEditor->setIndentation(7,20);

        settings->readODWSettings();
        settings->openLastProject();
        settings->openLastProject();
        ActOpenRecentFileVector.clear();

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
        connect(actionAbout, 	SIGNAL(triggered()),this,		SLOT(slotAbout()));
        connect(actionVoid, 	SIGNAL(triggered()),this,		SLOT(slotVoid()));
        connect(actionChar, 	SIGNAL(triggered()),this,		SLOT(slotChar()));
        connect(actionWchar_t, 	SIGNAL(triggered()),this,		SLOT(slotWchar_t()));
        connect(actionShort, 	SIGNAL(triggered()),this,		SLOT(slotShort()));
        connect(actionInt, 	SIGNAL(triggered()),this,		SLOT(slotInt()));
        connect(actionDouble, 	SIGNAL(triggered()),this,		SLOT(slotDouble()));
        connect(actionFloat, 	SIGNAL(triggered()),this,		SLOT(slotFloat()));
        connect(actionLong, 	SIGNAL(triggered()),this,		SLOT(slotLong()));
        connect(actionBool, 	SIGNAL(triggered()),this,		SLOT(slotBool()));
        connect(actionSigned, 	SIGNAL(triggered()),this,		SLOT(slotSigned()));
        connect(actionUnsigned, SIGNAL(triggered()),this,		SLOT(slotUnsigned()));
        connect(actionConst, 	SIGNAL(triggered()),this,		SLOT(slotConst()));
        connect(actionTypedef, 	SIGNAL(triggered()),this,		SLOT(slotTypedef()));
        connect(actionClass, 	SIGNAL(triggered()),this,		SLOT(slotClass()));
        connect(actionStruct, 	SIGNAL(triggered()),this,		SLOT(slotStruct()));
        connect(actionEnum,	SIGNAL(triggered()),this,		SLOT(slotEnum()));
        connect(actionUnion, 	SIGNAL(triggered()),this,		SLOT(slotUnion()));
        connect(actionNewMemory,SIGNAL(triggered()),this,		SLOT(slotNewMemory()));
        connect(actionDelete, 	SIGNAL(triggered()),this,		SLOT(slotDelete()));
        connect(actionConst_cast,SIGNAL(triggered()),this,		SLOT(slotConst_cast()));
        connect(actionReinterpret_cast,SIGNAL(triggered()),this,	SLOT(slotReinterpret_cast()));
        connect(actionStatic_cast,SIGNAL(triggered()),this,		SLOT(slotStatic_cast()));
        connect(actionAnd, 	SIGNAL(triggered()),this,		SLOT(slotAnd()));
        connect(actionAnd_eq, 	SIGNAL(triggered()),this,		SLOT(slotAnd_eq()));
        connect(actionBitand, 	SIGNAL(triggered()),this,		SLOT(slotBitand()));
        connect(actionBitor, 	SIGNAL(triggered()),this,		SLOT(slotBitor()));
        connect(actionCompl,	SIGNAL(triggered()),this,		SLOT(slotCompl()));
        connect(actionNot, 	SIGNAL(triggered()),this,		SLOT(slotNot()));
        connect(actionNot_eq,	SIGNAL(triggered()),this,		SLOT(slotNot_eq()));
        connect(actionOr, 	SIGNAL(triggered()),this,		SLOT(slotOr()));
        connect(actionOr_eq,	SIGNAL(triggered()),this,		SLOT(slotOr_eq()));
        connect(actionXor,	SIGNAL(triggered()),this,		SLOT(slotXor()));
        connect(actionXor_eq,	SIGNAL(triggered()),this,		SLOT(slotXor_eq()));
        connect(actionTrue,	SIGNAL(triggered()),this,		SLOT(slotTrue()));
        connect(actionFalse,	SIGNAL(triggered()),this,		SLOT(slotFalse()));
        connect(actionPrivate,	SIGNAL(triggered()),this,		SLOT(slotPrivate()));
        connect(actionProtected,SIGNAL(triggered()),this,		SLOT(slotProtected()));
        connect(actionPublic, 	SIGNAL(triggered()),this,		SLOT(slotPublic()));
        connect(actionFriend,	SIGNAL(triggered()),this,		SLOT(slotFriend()));
        connect(actionVirtual,	SIGNAL(triggered()),this,		SLOT(slotVirtual()));
        connect(actionExtern,	SIGNAL(triggered()),this,		SLOT(slotExtern()));
        connect(actionInline,	SIGNAL(triggered()),this,		SLOT(slotInline()));
        connect(actionStatic,	SIGNAL(triggered()),this,		SLOT(slotStatic()));
        connect(actionCase,	SIGNAL(triggered()),this,		SLOT(slotCase()));
        connect(actionBreak,	SIGNAL(triggered()),this,		SLOT(slotBreak()));
        connect(actionContinue,	SIGNAL(triggered()),this,		SLOT(slotContinue()));
        connect(actionDo_while,	SIGNAL(triggered()),this,		SLOT(slotDo_while()));
        connect(actionElse,	SIGNAL(triggered()),this,		SLOT(slotElse()));
        connect(actionFor, 	SIGNAL(triggered()),this,		SLOT(slotFor()));
        connect(actionGoto,	SIGNAL(triggered()),this,		SLOT(slotGoto()));
        connect(actionIf,	SIGNAL(triggered()),this,		SLOT(slotIf()));
        connect(actionReturn,	SIGNAL(triggered()),this,		SLOT(slotReturn()));
        connect(actionSwitch,	SIGNAL(triggered()),this,		SLOT(slotSwitch()));
        connect(actionWhile,	SIGNAL(triggered()),this,		SLOT(slotWhile()));
        connect(actionTry,	SIGNAL(triggered()),this,		SLOT(slotTry()));
        connect(actionCatch,	SIGNAL(triggered()),this,		SLOT(slotCatch()));
        connect(actionThrow,	SIGNAL(triggered()),this,		SLOT(slotThrow()));
        connect(actionCommon,	SIGNAL(triggered()), settings, 		SLOT(slotCommOptions()));
        connect(actionRGB,	SIGNAL(triggered()),this,		SLOT(slotRGB()));
        connect(actionKuzya_Help,	SIGNAL(triggered()),this,		SLOT(slotHelpKuzya()));
        connect(actionFind,	SIGNAL(triggered()),findText,		SLOT(slotFindDialog()));
        connect(actionReplace,	SIGNAL(triggered()),replaceText,	SLOT(slotReplaceDialog()));
        connect(textEditor,	SIGNAL(cursorPositionChanged (int, int)),this,	SLOT(slotUpdateStatusLabel(int, int)));
        connect(textEditor,	SIGNAL(modificationChanged(bool)),	 this,	SLOT(slotUpdateWindowName(bool)));
        connect(textEditor,	SIGNAL(marginClicked (int, int, Qt::KeyboardModifiers)), this, SLOT(slotMarginClicked(int, int, Qt::KeyboardModifiers)));
        connect(compiler,	SIGNAL(compileEnded(int)),		 this,	SLOT(slotAfterCompile(int)));

        connect(actionNotificationList, SIGNAL(toggled(bool)), this, SLOT(slotShowNotificationList(bool)));
        connect(notificationList, SIGNAL(itemSelectionChanged()), this, SLOT(slotShowErrorFromList()));
        connect(notificationList, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(slotGotoErrorLine(QListWidgetItem*)));
        connect(actionCode_language, SIGNAL(toggled(bool)), this, SLOT(slotUseNativeMode(bool)));

        connect ( actionDefFontSize,	SIGNAL ( triggered() ),	this,	SLOT ( slotZoomDef() ) );
        connect ( actionEnlFont,	SIGNAL ( triggered() ),	this,	SLOT ( slotZoomIn() ) );
        connect ( actionShrinkFont,	SIGNAL ( triggered() ),	this,	SLOT ( slotZoomOut() ) );
        connect	(actionToggleFolds,	SIGNAL(triggered()),	this,	SLOT (slotToggleFolds()));
        connect ( actionAbout_Qt,	SIGNAL ( triggered() ),	qApp,	SLOT ( aboutQt() ) );

        statusBar()->showMessage(tr("Ready"));


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
           }catch(int e){}
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

        textEditor->setModified(false);

        statusBar()->showMessage(tr("Opened"), 2000);

        slotUpdateWindowName(false);

        addFileNameToList(FileName);
        settings->saveLastProjectName(fileName);
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

        fileName = QString::null;
        translatedFileName = QString::null;

        textEditor->clear();
        setWindowTitle("Kuzya");
        statusBar()->showMessage(tr("Created new file"), 2000);
//        QTextStream stream(file);
//        textEditor->setText(stream.readAll());
//        file->close();
        textEditor->setModified(false);
}

/**
*******************************************************************************************************
**/
void Kuzya::slotOpen(void)
{
        if(slotSaveChangesNotifier()==false) return;
        textEditor->markerDeleteAll();

        QString filter;
        QStringList supportedList = compiler->getSupportedLanguages();
        foreach (QString lang, supportedList)
        {
            filter = filter+lang+" ("+compiler->getSupportedExtensions(lang)+");;";
        }

        filter = filter+"All Files (*)";

        QString openedFileName = QFileDialog::getOpenFileName(this, tr("Open File"), DefaultDir, filter/*tr("C/CPP Source-Files (*.c *.cpp *.cxx *.h);;Pascal Source-Files (*.fpc *.pas *.pp);;All Files (*)")*/);

        if ("" != openedFileName)
        {
                openFile(openedFileName);
        }

}

/**
*******************************************************************************************************
**/
bool Kuzya::slotSave(void)
{
        if (fileName.isEmpty())
        {
                fileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                           "", tr("C/CPP Source-Files (*.c *.cpp *.cxx *.h);;Pascal Source-Files (*.fpc *.pas *.pp);;All Files (*)"));
                slotUpdateWindowName(false);
        }
        if (fileName.isEmpty())
        {
                return false;
        }
        QString name;
        if (nativeMode)
        {
                translateCode(fromCode);
                name = translatedFileName;
        }
        else
        {
                translateCode(fromNative);
                name = fileName;
        }

        QFile file(name);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
                return false ;
        }
        QTextStream stream(&file);
        stream << textEditor->text();
        textEditor->setModified(false);
        file.close();
        statusBar()->showMessage(tr("Saved"), 2000);
        addFileNameToList(file.fileName());

        return true;
}

/**
*******************************************************************************************************
**/
void Kuzya::slotSave_as(void)
{
        //QFileDialog::setDirectory(DefaultDir);
        newFileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                           DefaultDir , tr("Source-Files (*.c *.cpp *.cxx *.h);;All Files (*)"));
        if (fileName.isEmpty()) return;

        if (nativeMode) translatedFileName = newFileName;
        else fileName = newFileName;

        slotSave();
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
//        settings->saveLastProjectName(fileName);
        close();
}
/**
*******************************************************************************************************
**/
void Kuzya::slotRun(void)
{
    if (fileName.isEmpty())
        {
                statusBar()->showMessage(tr("No binary to run"), 2000);
                return;
        }
    if (true == settings->ukrIsCheked())
    {
       // translateCode(fromNative);
        qDebug() << newFileName;
    }
        compiler->run();
       // translateCode(fromCode);
}

/**
*******************************************************************************************************
**/
void Kuzya::slotCompile(void)
{
        if (textEditor->isModified()) slotSave();

        textEditor->markerDeleteAll();
        if (fileName.isEmpty())
        {
                statusBar()->showMessage(tr("No source to compile"), 2000);
                return;
        }

        statusBar()->showMessage(tr("Compilling..."));
        textEditor->setReadOnly(true);
        if (compiler->isReady() && compiler->isModeAvailable(Compiler::DEFAULT))
        {
                statusBar()->showMessage(tr("Compilling..."));
                textEditor->setReadOnly(true);
                compiler->compile(fileName,Compiler::DEFAULT);
        }
        else statusBar()->showMessage(tr("ERROR : Could not find compiler profile or compile mode is not available."));
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
        if (Compiler::NOERROR == status)
        {
                notificationList->clear();
                QListWidgetItem *newItem = new QListWidgetItem(tr("Compiled successfuly!"));
                newItem->setData(Kuzya::attachedRole, QVariant(false));
                newItem->setData(Kuzya::descriptionRole,QVariant(tr("Compiled successfuly!")));
                newItem->setIcon(QIcon(":/notifications/accept"));
                notificationList->addItem(newItem);
                statusBar()->showMessage(tr("Compiled successfuly!"));
        }
        else
        {
                notificationList->clear();
                QListWidgetItem *newItem = new QListWidgetItem(tr("Compilation failed!!!"));
                newItem->setData(Kuzya::attachedRole, QVariant(false));
                newItem->setData(Kuzya::descriptionRole,QVariant(tr("Compilation failed!!!")));
                newItem->setIcon(QIcon(":/notifications/failed"));
                notificationList->addItem(newItem);
                emit slotShowNotificationList(true);
                paintErrorMarkers(compiler->getLastErrors());
        }
//        translateCode(fromCode);
}

/**
*******************************************************************************************************
**/
void Kuzya::paintErrorMarkers(QList<Compiler::compilerError>* errorList)
{
        textEditor->markerDeleteAll();
        if (errorList->empty()) return;

        statusBar()->showMessage(tr("Creating error list..."));

        QString str;
        for (int i = 0; i < errorList->size(); ++i)
        {
                textEditor->markerAdd(errorList->at(i).line-1, errorMarker);

                str = "Compilation error (line "+QVariant(errorList->at(i).line).toString()+") : "+errorList->at(i).description;

                QListWidgetItem *newItem = new QListWidgetItem(str);
                newItem->setData(Kuzya::attachedRole, QVariant(true));
                newItem->setData(Kuzya::lineRole, QVariant(errorList->at(i).line));
                newItem->setData(Kuzya::descriptionRole,QVariant(errorList->at(i).description));
                newItem->setIcon(QIcon(":/notifications/bug"));
                notificationList->addItem(newItem);
        }

        if (errorList->size()==1) str = QString(tr("1 error found in file %1")).arg(fileName);
            else str = QString(tr("%1 errors found in file %2")).arg(errorList->size()).arg(fileName);

        QListWidgetItem *newItem = new QListWidgetItem(str);
        newItem->setData(Kuzya::attachedRole, QVariant(false));
        newItem->setData(Kuzya::descriptionRole, str);
        newItem->setIcon(QIcon(":/notifications/failed"));
        notificationList->addItem(newItem);

        textEditor->setCursorPosition(notificationList->item(1)->data(Kuzya::lineRole).toInt(), 0);

        statusBar()->showMessage(notificationList->item(1)->data(Kuzya::descriptionRole).toString());
}

/**
*******************************************************************************************************
**/
void Kuzya::slotAbout(void)
{
        QMessageBox::about(this, tr("About"),tr("\t\t The Kuzya\n"
                                              "\t          Free Development Environment\n\n"

                                              "     Kuzya is simple crossplatform IDE for people who study  programming.Main\n"
                                              "idea of it is to concentrate attention  of the users only on learning the programming \n"
                                              "\t          language  but not on usage of IDE\n\n"

                                              "Idea:             Grygoriy Zlobin <zlobin@electronics.wups.lviv.ua>"
                                              "\nTeam leader: Andriy Shevchyk <shevchyk@users.sourceforge.net> "
                                              "\nAuthors:       Volodymyr Shevchyk <volder@users.sourceforge.net>"
                                              "\n                     Viktor Sklyar <bouyantgrambler@users.sourceforge.net>"
                                              "\n                     Alex Chmykhalo <alexchmykhalo@users.sourceforge.net>"));
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
void Kuzya::slotMarginClicked(int margin, int line, Qt::KeyboardModifiers state)
{
        if (1 == textEditor->markersAtLine(line))
        {
            QListWidgetItem *item = notificationList->findItems(QString("Compilation error (line %1)").arg(line+1), Qt::MatchContains).at(0);
            notificationList->setCurrentItem(item);
            notificationList->setFocus();
            textEditor->markerDeleteAll(currentMarker);
            statusBar()->showMessage(item->data(Kuzya::descriptionRole).toString());
        }
}

/**
*******************************************************************************************************
**/
void Kuzya::slotVoid()
{
        textEditor->insert("void ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotChar()
{
        textEditor->insert("char ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotWchar_t()
{
        textEditor->insert("wchar_t ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotShort()
{
        textEditor->insert("short ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotInt()
{
        textEditor->insert("int ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 4);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotDouble()
{
        textEditor->insert("double ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotFloat()
{
        textEditor->insert("float ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotLong()
{
        textEditor->insert("long ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotBool()
{
        textEditor->insert("bool ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotSigned()
{
        textEditor->insert("signed ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotUnsigned()
{
        textEditor->insert("unsigned ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 9);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotConst()
{
        textEditor->insert("const ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotTypedef()
{
        textEditor->insert("typedef ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotClass()
{
        textEditor->insert("class\n{\n};\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotStruct()
{
        textEditor->insert("struct\n{\n};\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotEnum()
{
        textEditor->insert("enum\n{\n};\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotUnion()
{
        textEditor->insert("union\n{\n};\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotNewMemory()
{
        textEditor->insert("new ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index +5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotDelete()
{
        textEditor->insert("delete ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotConst_cast()
{
        textEditor->insert("const_cast ();\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 12);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotReinterpret_cast()
{
        textEditor->insert("reinterpret_cast ();\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 18);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotStatic_cast()
{
        textEditor->insert("static_cast ();\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 13);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotAnd()
{
        textEditor->insert("and ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 4);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotAnd_eq()
{
        textEditor->insert("and_eq ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotBitand()
{
        textEditor->insert("bitand ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotBitor()
{
        textEditor->insert("bitor ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotCompl()
{
        textEditor->insert("compl ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotNot()
{
        textEditor->insert("not ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 4);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotNot_eq()
{
        textEditor->insert("not_eq ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index +7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotOr()
{
        textEditor->insert("or ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 3);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotOr_eq()
{
        textEditor->insert("or_eq ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotXor()
{
        textEditor->insert("xor ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 4);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotXor_eq()
{
        textEditor->insert("xor_eq ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotTrue()
{
        textEditor->insert("true ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotFalse()
{
        textEditor->insert("false ");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotPrivate()
{
        textEditor->insert("private:\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 9);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotProtected()
{
        textEditor->insert("protected:\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 11);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotPublic()
{
        textEditor->insert("public:\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotFriend()
{
        textEditor->insert("friend ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotVirtual()
{
        textEditor->insert("virtual ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotExtern()
{
        textEditor->insert("extern ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotInline()
{
        textEditor->insert("inline ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotStatic()
{
        textEditor->insert("static ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotCase()
{
        textEditor->insert("case : ;\n\tbreak;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotBreak()
{
        textEditor->insert("break ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotContinue()
{
        textEditor->insert("continue ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 9);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotDo_while()
{
        textEditor->insert("do\n{\n}\nwhile();\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 13);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotElse()
{
        textEditor->insert("else\n{\n}\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotFor()
{
        textEditor->insert("for (; ; )\n{\n}\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotGoto()
{
        textEditor->insert("goto ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 5);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotIf()
{
        textEditor->insert("if ()\n{\n}\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 4);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotReturn()
{
        textEditor->insert("return ;\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotSwitch()
{
        textEditor->insert("switch ()\n{\n}\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 8);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotWhile()
{
        textEditor->insert("while ()\n{\n}\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 7);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotTry()
{
        textEditor->insert("try\n{\n\t\n}\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotCatch()
{
        textEditor->insert("catch()\n{\n}\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotThrow()
{
        textEditor->insert("throw ();\n");
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index + 6);
}
/**
*******************************************************************************************************
**/
void Kuzya::slotRGB()
{
        QColor color;
        color = QColorDialog::getColor();
        QVariant vr,vg,vb;
        vr.setValue(color.red());
        vg.setValue(color.green());
        vb.setValue(color.blue());
        QString s="("+vr.toString()+","+vg.toString()+","+vb.toString()+")";
        textEditor->insert(s);
        int line, index;
        textEditor->getCursorPosition(&line, &index);
        textEditor->setCursorPosition(line, index+s.length());
}

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
                                return true;
                        }
                        else return false;///false - Saving was canceled
                }
                if (msgBox->clickedButton() ==noBtn)
                {
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
}
/**
*******************************************************************************************************
**/
 void Kuzya::closeEvent(QCloseEvent *event)
 {
        if(slotSaveChangesNotifier()==false)
        {
                event->ignore();
        }
        settings->writeSettings();
//        settings->saveLastProjectName(fileName);

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
        HelpBrowser* helpBrowser = new HelpBrowser(QApplication::applicationDirPath()+"/../trunc/doc/Kuzya_Help","main.htm");
        helpBrowser->resize(800,600);
        helpBrowser->show();
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
            qDebug() << item->data(Kuzya::descriptionRole).toString();
            textEditor->markerDeleteAll(currentMarker);
            if (item->data(Kuzya::attachedRole).toBool())
            {
                textEditor->setCursorPosition(item->data(Kuzya::lineRole).toInt()-1,1);
            }
}
/**
******************************************************************************************
*****************translate program code to English**************************************
*/
void Kuzya::translateCode(translationEnum direction)
{

    qDebug() << fileName;
    QFile fileTrans("d:/Work/ukranian.tr");
    if(!fileTrans.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << tr("Can't find translation for the code");
        statusBar()->showMessage(tr("Can't find translation for the code"));
        return;
    }
    QTextStream trStream(&fileTrans);
    translatedFileName = QString::null;
    QString name;
    if (fromCode == direction)
    {
        if (translatedFileName.isEmpty())
            translatedFileName = fileName+"t" ;
        name = translatedFileName;
    }
    else
    {
        if (fileName.isEmpty())
            fileName = translatedFileName+"s";
        name = fileName;
    }

    QString text = textEditor->text();

    QString trLine;
    QString key;
    QString translation;
    while (!trStream.atEnd())
    {
        trLine = trStream.readLine();
        key = trLine;
        key.truncate(key.lastIndexOf('='));
        translation = trLine.section('=', 1);
//        translation.truncate(translation.lastIndexOf(';'));
        if (fromCode == direction)
        {
            text.replace(key, translation);
        }
        else
        {
            qDebug() << "key " << key;
            qDebug() << "tr " << translation;
            qDebug() << "line " << trLine;
            text.replace(translation, key);
        }
    }
    fileTrans.close();

    QFile outFile(name);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
         qDebug() << tr("Can't open out file    ") << name;
         return ;
    }
    QTextStream outStream(&outFile);
    outStream << text;
    outFile.close();

}

void Kuzya::slotUseNativeMode(bool isNative)
{
    nativeMode = isNative;

//    if (nativeMode)
//        textEditor->setUtf8(true);
//    else textEditor->setUtf8(false);

   if (!fileName.isEmpty())
    {
        if (textEditor->isModified())
        {
            slotSave();
        }
        if (nativeMode)
        {
            openFile(translatedFileName);
        }
        else
        {
            openFile(fileName);
        }
    }
}
