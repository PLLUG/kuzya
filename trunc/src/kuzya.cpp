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
#include "translator.h"
//#include "helpassistant.h"


Kuzya::Kuzya(QWidget *parent)
        : QMainWindow(parent)
{
        setupUi(this);
        setObjectName("Kuzya");
        setWindowTitle("Kuzya");

        languageComboBox = new QComboBox(this);

        toolBar->addAction(actionNew);
        toolBar->addAction(actionOpen);
        toolBar->addAction(actionSave);
        toolBar->addSeparator();
        toolBar->addAction(actionUndo);
        toolBar->addAction(actionRedo);
        actionCut->setShortcuts(QKeySequence::Cut);
        toolBar->addAction(actionCut);
        actionCopy->setShortcuts(QKeySequence::Copy);
        toolBar->addAction(actionCopy);
        actionPaste->setShortcuts(QKeySequence::Paste);
        toolBar->addAction(actionPaste);
        toolBar->addSeparator();
        toolBar->addAction(actionNotificationList);
        toolBar->addSeparator();
        toolBar->addAction(actionRun);
        toolBar->addAction(actionCompile);
        toolBar->addSeparator();
        toolBar->addWidget(languageComboBox);



        statusLabel = new QLabel(this);
        statusBar()->addPermanentWidget(statusLabel);

        QGridLayout *gridLayout;
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

        textEditor = new QsciScintilla(this);

        notificationList = new QListWidget(this);
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

        //QsciLexerCPP *
        cppLexer = new QsciLexerCPP(this);
       
        textEditor->setLexer(cppLexer);

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
        connect(actionCommon,	SIGNAL(triggered()), settings, 		SLOT(slotCommOptions()));
        connect(actionAbout, 	SIGNAL(triggered()),this,		SLOT(slotAbout()));
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

        connect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeTranslation(QString)));

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

        if(settings->isLineMarginVisible) textEditor->setMarginWidth(3,QVariant(textEditor->lines()).toString());

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
        notificationList->clear();
        languageComboBox->clear();

        fileName = QString::null;
        translatedFileName = QString::null;

        textEditor->clear();
        textEditor->setMarginWidth(3,"12");
        setWindowTitle("Kuzya");
        statusBar()->showMessage(tr("Created new file"), 2000);
        textEditor->setModified(false);
}

/**
*******************************************************************************************************
**/
void Kuzya::slotOpen(void)
{
        if(slotSaveChangesNotifier()==false) return;

        QString filter;
        QStringList supportedList = compiler->getSupportedLanguages();
        foreach (QString lang, supportedList)
        {
            filter = filter+lang+" ("+compiler->getSupportedExtensions(lang)+");;";
        }

        filter = filter+"All Files (*)";

        QString openedFileName = QFileDialog::getOpenFileName(this, tr("Open File"), DefaultDir, filter);

        if ("" != openedFileName)
        {
                textEditor->markerDeleteAll();
                notificationList->clear();
                openFile(openedFileName);
                refreshProfileSettings();
        }
}

/**
*******************************************************************************************************
**/

void Kuzya::refreshProfileSettings()
{
    QStringList supportedList = compiler->getSupportedLanguages();    
    QString ex(fileName);
    ex = ex.remove(0, ex.lastIndexOf("."));

    QString language;
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
        compiler->loadProfile(language, compiler->getSupportedCompilers(language).at(0));
   #ifdef WIN32
        QString path = QApplication::applicationDirPath();
        path.truncate(path.lastIndexOf("/", -1));
        path = path+"/profiles/";
   #else
        QString path = "/usr/share/kuzya/profiles/";
   #endif
        unloadTemplates();
        loadTemplates(path+language+"/"+language+".ini");
    }
    else compiler->loadProfile("","");


#ifdef WIN32
    QString path = QApplication::applicationDirPath();
    path.truncate(path.lastIndexOf("/", -1));
    path = path+"/resources/";
#else
    QString path = "/usr/share/kuzya/resources/";
#endif

    translator->openFile(fileName, language);

    QStringList supportedTranslations = translator->getSupportedTranslations(language);
    if (supportedTranslations.isEmpty())
    {
        languageComboBox->setVisible(true);
        return;
    }
    else
    {
        disconnect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeTranslation(QString)));
        languageComboBox->clear();
        languageComboBox->addItem(QIcon(path+"uk.png"), "en");
        foreach (QString trans, supportedTranslations)
        {
           languageComboBox->addItem(QIcon(path+trans+".png"), trans);
        }
        QString codeTranslation = translator->translation();
        languageComboBox->setCurrentIndex(supportedTranslations.indexOf(codeTranslation)+1);
        connect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotChangeTranslation(QString)));
    }
}

/**
*******************************************************************************************************
**/
bool Kuzya::slotSave(void)
{
        if (fileName.isEmpty())
        {
            QString filter;
            QStringList supportedList = compiler->getSupportedLanguages();
            foreach (QString lang, supportedList)
            {
                filter = filter+lang+" ("+compiler->getSupportedExtensions(lang)+");;";
            }

            filter = filter+"All Files (*)";
            fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), DefaultDir, filter);
            slotUpdateWindowName(false);
        }

        if (fileName.isEmpty())
        {
                return false;
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

        return true;
}

/**
*******************************************************************************************************
**/
void Kuzya::slotSave_as(void)
{
        //QFileDialog::setDirectory(DefaultDir);
        QString filter;
        QStringList supportedList = compiler->getSupportedLanguages();
        foreach (QString lang, supportedList)
        {
            filter = filter+lang+" ("+compiler->getSupportedExtensions(lang)+");;";
        }

        filter = filter+"All Files (*)";
        newFileName = QFileDialog::getSaveFileName(this, tr("Save as..."),
                                           DefaultDir , filter);
        if (!newFileName.isEmpty()) fileName = newFileName;
/*=======
        if (!fileName.isEmpty()) fileName = newFileName;
>>>>>>> .r78
*/
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
    compiler->run();
}

/**
*******************************************************************************************************
**/
void Kuzya::slotCompile(void)
{
        if (textEditor->isModified()) slotSave();

        removeAllNotifications();
        if (fileName.isEmpty())
        {
//                statusBar()->showMessage(tr("No source to compile"), 2000);
                addNotification(FAILING, tr("No source to compile"));
                return;
        }

        textEditor->setReadOnly(true);
        if (compiler->isReady() && compiler->isModeAvailable(Compiler::DEFAULT))
        {
                addNotification(WAIT, tr("Compilling..."));
                textEditor->setReadOnly(true);
                translator->retranslate();
                compiler->compile(translator->codeFile(), Compiler::DEFAULT);
                removeAllNotifications();
        }
        else addNotification(FAILING, tr("ERROR : Could not find compiler profile or compile mode is not available."));
            //statusBar()->showMessage(tr("ERROR : Could not find compiler profile or compile mode is not available."));
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
        if (Compiler::NOERROR == status)
        {
                addNotification(SUCCESS, tr("Compiled successfuly!"));
                paintWarningMarkers(compiler->getLastWarnings());
        }
        else
        {
                addNotification(FAILING, tr("Compilation failed!!!"));
                paintErrorMarkers(compiler->getLastErrors());
                paintWarningMarkers(compiler->getLastWarnings());
                //slotShowNotificationList(true);
        }

}

/**
*******************************************************************************************************
**/
void Kuzya::paintErrorMarkers(QList<Compiler::compilerError>* errorList)
{
        if (errorList->empty()) return;
        for (int i = 0; i < errorList->size(); ++i)
        {
                addNotification(ERROR, errorList->at(i).description, true, errorList->at(i).line);
        }

        QString str;
        //if (errorList->size()==1) str = QString(tr("1 error found in file %1")).arg(fileName);
          //  else str = QString(tr("%1 errors found in file %2")).arg(errorList->size()).arg(fileName);

        addNotification(INFO, tr("Found errors (%2) in file %1").arg(fileName).arg(errorList->size()));

        textEditor->setCursorPosition(notificationList->item(1)->data(Kuzya::lineRole).toInt(), 0);
        statusBar()->showMessage(notificationList->item(1)->data(Kuzya::descriptionRole).toString());
}
/**
*******************************************************************************************************
**/
void Kuzya::paintWarningMarkers(QList<Compiler::compilerWarning>* warningList)
{
        if (warningList->empty()) return;
        for (int i = 0; i < warningList->size(); ++i)
        {
                addNotification(WARNING, warningList->at(i).description, true, warningList->at(i).line);
        }

        QString str;
        addNotification(INFO, tr("Found warnings (%2) in file %1").arg(fileName).arg(warningList->size()));
        //if (errorList->size()==1) str = QString(tr("1 error found in file %1")).arg(fileName);
            //else str = QString(tr("%1 errors found in file %2")).arg(errorList->size()).arg(fileName);

        //addNotification(FAILING, str);

        //textEditor->setCursorPosition(notificationList->item(1)->data(Kuzya::lineRole).toInt(), 0);

        //statusBar()->showMessage(notificationList->item(1)->data(Kuzya::descriptionRole).toString());
}

/**
*******************************************************************************************************
**/
void Kuzya::slotAbout(void)
{
    QMessageBox *aboutBox= new QMessageBox( QMessageBox::Information,tr("About"),tr("\t  <big><b><centre> \t    The Kuzya 2.0.1 </centre> </b></big>  "
                                              "\n  <p> Free Development Environment</p>\n\n"
                                              "build on Jul 6 2009"
                                              "<p> Kuzya is simple crossplatform IDE for people who study  programming."
                                              "Main idea of it is to concentrate attention  of the users only on learning the programming \n"
                                              "\t language  but not on usage of IDE\n\n </p>"
                                              "<pre> <b>Idea:</b> \n \t <centre>Grygoriy Zlobin</centre>"
                                              "\n <u>zlobin@electronics.wups.lviv.ua</u> "
                                              "\n\n <b>Team leader:</b> \n \t <centre>Andriy Shevchyk</centre> "
                                              "\n <u>shevchyk@users.sourceforge.net</u> "
                                              "\n\n <b>Authors:</b>      \n \t <centre>Volodymyr Shevchyk</centre> "
                                              "\n <u>volder@users.sourceforge.net</u>"
                                              "\n              \n \t <centre>Victor Sklyar</centre> "
                                              "\n <u>bouyantgrambler@users.sourceforge.net</u>"
                                              "\n              \n \t <centre>Alex Chmykhalo</centre> "
                                              "\n <u>alexchmykhalo@users.sourceforge.net</u> </pre>"),QMessageBox::Ok,this,Qt::Dialog);
    #ifdef WIN32
        aboutBox->setIconPixmap(QPixmap(QApplication::applicationDirPath()+"/../resources/Kuzya.png"));
    #else
        aboutBox->setIconPixmap(QPixmap("/usr/share/kuzya/resources/Kuzya.png"));
    #endif
        aboutBox->exec();
    delete aboutBox;
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
void Kuzya::slotMarginClicked(int margin,int line,Qt::KeyboardModifiers)
{
        if (1 == textEditor->markersAtLine(line))
        {
            QListWidgetItem *item = notificationList->findItems(QString("Compilation error (line %1)").arg(line+1), Qt::MatchContains).at(0);
            notificationList->setCurrentItem(item);
            notificationList->setFocus();
            textEditor->markerDeleteAll(currentMarker);            
            textEditor->markerAdd(line,currentMarker);
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
                refreshProfileSettings();
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
#ifdef WIN32
        HelpBrowser* helpBrowser = new HelpBrowser(QApplication::applicationDirPath()+"/../doc/Kuzya_Help","main.htm");
#else
              HelpBrowser* helpBrowser = new HelpBrowser("/usr/doc/kuzya/doc/Kuzya_Help","main.htm");
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
        case ERROR:
            str = "Compilation error (line "+QVariant(line).toString()+") "+descr;
            icon.addFile(":/notifications/error");
            textEditor->markerAdd(line-1, errorMarker);
            break;
        case WARNING:
            str = "Compilation warning (line "+QVariant(line).toString()+") "+descr;
            icon.addFile(":/notifications/warning");
            textEditor->markerAdd(line-1, warningMarker);
            break;
        case SUCCESS:
            icon.addFile(":/notifications/success");
            statusBar()->showMessage(descr, 3000);
            break;
        case FAILING:
            icon.addFile(":/notifications/failing");
            statusBar()->showMessage(descr, 3000);
            slotShowNotificationList(true);
            break;
        case WAIT:
            icon.addFile(":/notifications/wait");
            statusBar()->showMessage(descr, 3000);
            break;
        case INFO:
            icon.addFile(":/notifications/info");
            break;
    }

    if (str.isEmpty()) str = descr;

    QListWidgetItem *newItem = new QListWidgetItem(str);
    newItem->setData(Kuzya::attachedRole, QVariant(attached));
    newItem->setData(Kuzya::lineRole, QVariant(line));
    newItem->setData(Kuzya::descriptionRole,QVariant(descr));
    newItem->setIcon(icon);
    //newItem->setSizeHint(QSize(0,fontMetrics().height()*2));
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
            }
}
/**
******************************************************************************************
*****************translate program code to English**************************************
*/
void Kuzya::slotChangeTranslation(QString translation)
{
    translator->setTranslation(translation);
    openFile(translator->translatedCodeFile());
}
///***********************************************************************************************************///
void Kuzya::slotPasteTemplate(QString keyStr)
{
    tlist->beginGroup("templates");
        tlist->beginReadArray(keyStr.left(keyStr.indexOf("/")));
            tlist->setArrayIndex((keyStr.right(keyStr.length()-keyStr.indexOf("/")-1)).toUInt());
           textEditor->insert(tlist->value("template").toString());
        tlist->endArray();
    tlist->endGroup();
}
///***********************************************************************************************************///
void Kuzya::loadTemplates(QString templatesPath)
{
    tlist=new QSettings(templatesPath,QSettings::IniFormat);
    tlist->beginGroup("templates");
    int size;
    templatesCroupsList=tlist->childGroups();
    templatesSignalMapper = new QSignalMapper(this);
    QMenu *menu;
    for(int j=0; j<templatesCroupsList.count();++j)
    {
        menu=menuTemplates->addMenu(QIcon(":/menu/templates_menu"), templatesCroupsList.at(j));
        size=tlist->beginReadArray(templatesCroupsList.at(j));
        for(int i=0;i<size;++i)
        {

            tlist->setArrayIndex(i);
            templlateAct.push_back(new QAction(QIcon(":/menu/template"), tlist->value("label").toString(),this));
            menu->addAction(templlateAct[i]);
            connect(templlateAct[i],SIGNAL(triggered()),templatesSignalMapper,SLOT(map()));
            templatesSignalMapper->setMapping(templlateAct[i],templatesCroupsList.at(j)+"/"+QVariant(i).toString());
        }
        tlist->endArray();
        templlateAct.clear();
    }
    tlist->endGroup();
    connect(templatesSignalMapper,SIGNAL(mapped(QString)),this,SLOT(slotPasteTemplate(QString)));

}
///***********************************************************************************************************///
void Kuzya::unloadTemplates()
{
    menuTemplates->clear();
    if(templatesSignalMapper==0) disconnect(templatesSignalMapper,SIGNAL(mapped(QString)),this,SLOT(slotPasteTemplate(QString)));


}
