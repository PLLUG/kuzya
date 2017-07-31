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

#include <Qsci/qsciscintilla.h>
#include <QListWidget>
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QColor>
#include <QFileDialog>
#include <QDebug>
#include <QLocale>
#include "optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    setWindowTitle(tr("Settings"));
    mw =(Kuzya*)parent;
    textEditor=mw->getTextEditorPointer();
#ifdef WIN32
    settings = new QSettings(QApplication::applicationDirPath()+"/settings.ini", QSettings::IniFormat);
#else
    settings = new QSettings(QDir::homePath()+"/.config/kuzya/settings.ini", QSettings::IniFormat);
#endif
    //readODWSettings();
    connect(closeBtn,SIGNAL(clicked()), this,SLOT(slotClose(void)));
    connect(applyBtn,SIGNAL(clicked()), this,SLOT(slotApply(void)));
    connect(okBtn,	 SIGNAL(clicked()), this,SLOT(slotOk(void)));
    connect(defaultBtn,SIGNAL(clicked()),this,SLOT(slotDefaultAll(void)));
    connect(directoryBox,SIGNAL(activated(int)),this,SLOT(slotChangeDefDir(int)));
    connect(directoryBox,SIGNAL(editTextChanged(QString)),this,SLOT(slotChangeDefDir(QString)));
    connect(styleCBox, SIGNAL(activated(int)), this ,SLOT(slotChangeStyle(int)));
    connect(skinCBox, SIGNAL(activated(QString)), this, SLOT(slotChangeSkin(QString)));
    connect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotUpdateCompilerCBox(QString)));
    connect(compilerComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotLoadCompilerOptions(QString)));
    connect(browseButton, SIGNAL(clicked()), this, SLOT(slotChangeCompilerLocation()));
    connect(defaultUsePushButton, SIGNAL(clicked()), this, SLOT(slotDefaultCompiler()));
    connect(compilerResetPushButton, SIGNAL(clicked()) ,this, SLOT(slotResetCompilerOptions()));
    connect(localizationLanguageCBox,SIGNAL(activated(QString)),this,SLOT(slotChangsLocalizationLanguage(QString)));
    connect(mTabIconView,SIGNAL(currentRowChanged(int)),this,SLOT(slotChangeOptionPage(int)));
    ///-----------------------------Fonts and Colors-------------------------------------------------------
    styleCBox->addItems(QStyleFactory::keys());


#ifdef WIN32
    stylesDir=QDir::cleanPath(QApplication::applicationDirPath() + "/../../resources/qss/");
    localizationLanguageDir=QDir::cleanPath(QApplication::applicationDirPath() + "/../../resources/translations/");
#else
    stylesDir=QDir(QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/resources/qss/"));
    if (stylesDir.exists() == false)
    {
        stylesDir=QDir(QDir::cleanPath(QApplication::applicationDirPath() + "/../"+STYLESHEETS_RELATIVE_PATH));
        if (stylesDir.exists() == false)
        {
            QMessageBox msgBox;
            msgBox.setText("There is some problem with loading of styles");
            msgBox.exec();
        }
    }

    localizationLanguageDir=QDir(QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/resources/translations/"));
    if (localizationLanguageDir.exists() == false)
    {
        localizationLanguageDir=QDir(QDir::cleanPath(QApplication::applicationDirPath() + "/../"+TRANSLATIONS_REALATIVE_PATH));
        if (localizationLanguageDir.exists() == false)
        {
            QMessageBox msgBox;
            msgBox.setText("There is some problem with loading of translations");
            msgBox.exec();
        }
    }
#endif

    languageComboBox->clear();
    QStringList supportedList = mw->getCurrentCompiler()->getSupportedLanguages();
    supportedList.sort();
    languageComboBox->addItems(supportedList);

    styleFilters<<"*.qss";
    localizationLanguageFilters<<"*.qm";
    stylesDir.setNameFilters(styleFilters);
    localizationLanguageDir.setNameFilters(localizationLanguageFilters);

    slotUpdateSkinsCBox();
    slotUpdatelocalizationLanguageCBox();


}
void OptionsDialog::slotUpdateSkinsCBox(void)
{
    QStringList skinsNameList = stylesDir.entryList(stylesDir.nameFilters(),QDir::Files,QDir::Name);
    for(int i = 0;i<skinsNameList.count();i++)
    {
        skinsNameList[i]=skinsNameList.at(i).left(skinsNameList.at(i).lastIndexOf("."));
    }
    skinCBox->addItems(skinsNameList);
    //skinCBox->addItems(stylesDir.entryList(stylesDir.nameFilters(),QDir::Files,QDir::Name));
}
void OptionsDialog::slotUpdatelocalizationLanguageCBox()
{
    QStringList localizationLanguageList = localizationLanguageDir.entryList(localizationLanguageDir.nameFilters(),QDir::Files,QDir::Name);
    for(int i=0;i<localizationLanguageList.count();i++)
    {
        localizationLanguageList[i]=localizationLanguageList.at(i).left(localizationLanguageList.at(i).lastIndexOf("."));
        localizationLanguageCBox->addItem(QIcon(":flags/images/flags/"+localizationLanguageList[i]+".png"),localizationLanguageList[i]);
    }
}

OptionsDialog::~OptionsDialog()
{
}
void OptionsDialog::retranslate(void)
{
    retranslateUi(this);
}

void OptionsDialog::slotChangeStyle(int)
{
    qApp->setStyle(styleCBox->currentText());
}
void OptionsDialog::slotChangeSkin(QString sheetName)
{
#ifdef WIN32
    QFile file(QApplication::applicationDirPath()+"/../../resources/qss/"+sheetName.toLower()+".qss");
#else
    //    QFile file(QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/resources/qss/")+sheetName.toLower()+".qss");
    QFile file(stylesDir.absolutePath()+"/"+sheetName.toLower()+".qss");
#endif
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);

}
///**************
///*************************************************
///*******writeSettings****************************************************************************************
void OptionsDialog::writeSettings(void)
{

    ///--------------------MainWindow--------------------------------------------
    settings->beginGroup("Settings/MainWindow");
    settings->setValue("StartupPro", checkBox->isChecked());

    ///------------------------------------------------------------------------------------------
    mw->setMaxCount_RFileList(sB_LOFCount->value());
    mw->resizeRFileList(mw->getMaxCount_RFileList());
    settings->setValue("LOFCount",sB_LOFCount->value());
    ///-----Style&Skins----------------------------------------------------------------------
    settings->beginGroup("Interface");
    settings->setValue("Style",styleCBox->currentText());
    settings->setValue("Skin",skinCBox->currentText());
    settings->endGroup();
    ///-----DefaultDirectory-------------------------------------------------------------------------
    settings->setValue("DefaultDir",directoryBox->currentText());

    ///-----LANGUAGE-------------------------------------------------------------------------------------
    settings->setValue("Language",localizationLanguageCBox->currentText());
    ///-------------------------------------------------------------------------------------------
    settings->endGroup();
    ///-----PROGRAMING--LANGUAGE---------------------------------------------------
    settings->beginGroup("compilation_settings");
    settings->setValue("defaultLanguage/text",defaultLanguageComboBox->currentText());
    settings->setValue("defaultLanguage/index", defaultLanguageComboBox->currentIndex());
    QString val = languageComboBox->currentText()+"/"+compilerComboBox->currentText();
    QString location = compilerDirLineEdit->text().replace("/", QDir::separator());
    if (!location.isEmpty())
    {
        compilerDirLineEdit->setText(location);
    }
    settings->setValue(val+"/location", location);
    settings->setValue(val+"/options", compilerOptionsEdit->toPlainText().remove("\n"));/**/

    settings->endGroup();
    ///-----RECENT FILES------------------------------------------------------------------

    settings->beginWriteArray("RecentFiles");
    remove("");
    for(int i=0;i<mw->getFileListCount();i++)
    {
        settings->setArrayIndex(i);
        settings->setValue("FileName",mw->getRFileListAt(i));
    }
    settings->endArray();
    ///***************************TEXT_EDITOR************************************************************************
    settings->beginGroup("/Settings/TextEditor");
    settings->setValue("ShowLineNumber",lineNumbCHB->isChecked());
    settings->setValue("WordCompletion",autoComplCHB->isChecked());
    settings->setValue("WordWrap",wordWrapCHB->isChecked());
    settings->setValue("CodeFolding",foldingMarkersCHB->isChecked());
    settings->beginGroup("/Indentation");
    settings->setValue("AutoIndentation",autoIndentCHB->isChecked());
    settings->setValue("ShowIndentLine",indentLineCHB->isChecked());
    settings->setValue("UseTabIndent",tabKeyindentCHB->isChecked());
    settings->setValue("UseBackspaceIndent",BkspaceIndentCHB->isChecked());
    settings->endGroup();
    settings->endGroup();
    settings->sync();
}


///****
///************************************
///*******readOptionDialogWindowSettings***************************************************************************


void OptionsDialog::readODWSettings()
{	
    settings->beginGroup("Settings/MainWindow");
    checkBox->setChecked(settings->value("StartupPro",false).toBool());
    ///-----------------------------------------------------------------------------
    sB_LOFCount->setValue(settings->value("LOFCount",5).toInt());
    mw->setMaxCount_RFileList(sB_LOFCount->value());
    ///-----Style&Skins----------------------------------------------------------------------
    settings->beginGroup("Interface");
    styleCBox->setCurrentIndex(styleCBox->findText(settings->value("Style","Cleanlooks").toString()));
    qApp->setStyle(settings->value("Style","Cleanlooks").toString());
    if ("empty" == settings->value("Skin","empty").toString())
    {
        settings->setValue("Skin","creation");
    }
    skinCBox->setCurrentIndex(skinCBox->findText(settings->value("Skin","creation").toString()));
    slotChangeSkin(settings->value("Skin","creation").toString());
    settings->endGroup();
    ///-----LANGUAGE-------------------------------------------------------------------------

    //QVariant(QLocale::system().name()).toString()
    if ("empty" == settings->value("Language","empty").toString())
    {
        if (-1==localizationLanguageCBox->findText(QLocale::languageToString(QLocale::system().language())))
        {
            settings->setValue("Language","English");
        }
        else
        {
            settings->setValue("Language",QLocale::languageToString(QLocale::system().language()));
        }

    }
#ifdef WIN32                
    translator.load(QApplication::applicationDirPath()+"/../../resources/translations/"+settings->value("Language",QLocale::languageToString(QLocale::system().language())).toString()+".qm");

#else
    //    translator.load(QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/resources/translations/") + settings->value("Language",QLocale::languageToString(QLocale::system().language())).toString()+".qm");
    translator.load(localizationLanguageDir.absolutePath()+"/" + settings->value("Language",QLocale::languageToString(QLocale::system().language())).toString()+".qm");

#endif

    qApp->installTranslator(&translator);
    mw->retranslateAll();

    localizationLanguageCBox->setCurrentIndex(localizationLanguageCBox->findText(settings->value("Language","English.qm").toString()));
    ///-----DefaultDirectory-------------------------------------------------------------------------
    directoryBox->setCurrentIndex(0);
    if(directoryBox->findText(settings->value("DefaultDir",QDir::homePath()).toString())==-1)
    {
        directoryBox->insertItem(0,settings->value("DefaultDir",QDir::homePath()).toString());
        directoryBox->removeItem(1);
        directoryBox->setCurrentIndex(0);
    }
    mw->setDefaultDir(directoryBox->currentText());

    ///------------------------------------------------------------------------------
    settings->endGroup();
    ///-----PROGRAMING--LANGUAGE---------------------------------------------------
    slotLoadCompilerOptions(compilerComboBox->currentText());
    //        settings->endGroup();
    ///-----RECENT FILES------------------------------------------------------------------

    int size=settings->beginReadArray("RecentFiles");
    for (int i = 0; i < size; ++i)
    {
        settings->setArrayIndex(i);
        mw->addFileNameToList(settings->value("FileName","").toString());
    }
    if (size==0)
    {
        mw->resizeRFileList(0);
    }
    settings->endArray();
    ///***************************TEXT_EDITOR************************************************************************
    settings->beginGroup("/Settings/TextEditor");

    lineNumbCHB->setChecked(settings->value("ShowLineNumber",false).toBool());
    if(lineNumbCHB->isChecked())
    {
        if(textEditor->lines()>2)
        {
            textEditor->setMarginWidth(3,QVariant(textEditor->lines()).toString());
        }else
        {
            textEditor->setMarginWidth(3,"12");
        }
        isLineMarginVisible=true;
    }
    else
    {
        textEditor->setMarginWidth(3,"");
        isLineMarginVisible=false;
    }
    textEditor->setMarginLineNumbers(3,lineNumbCHB->isChecked());

    autoComplCHB->setChecked(settings->value("WordCompletion",false).toBool());
    mw->setAutoCompletionEnabled(autoComplCHB->isChecked());         /// nado shoto sdelat'

    wordWrapCHB->setChecked(settings->value("WordWrap",false).toBool());
    if(wordWrapCHB->isChecked())
    {
        textEditor->setWrapMode(QsciScintilla::WrapWord);
    }
    else
    {
        textEditor->setWrapMode(QsciScintilla::WrapNone);
    }

    foldingMarkersCHB->setChecked(settings->value("CodeFolding",true).toBool());
    if(foldingMarkersCHB->isChecked())
    {
        textEditor->setFolding(QsciScintilla::PlainFoldStyle,4);
        mw->toggleFoldsActionEnabled(true);
    }
    else
    {
        textEditor->setFolding(QsciScintilla::NoFoldStyle,4);
        mw->toggleFoldsActionEnabled(false);
    }
    settings->beginGroup("/Indentation");

    autoIndentCHB->setChecked(settings->value("AutoIndentation",false).toBool());
    textEditor->setAutoIndent(settings->value("AutoIndentation",false).toBool());

    indentLineCHB->setChecked(settings->value("ShowIndentLine",false).toBool());
    textEditor->setIndentationGuides(indentLineCHB->isChecked());

    tabKeyindentCHB->setChecked(settings->value("UseTabIndent",true).toBool());
    textEditor->setIndentationsUseTabs(tabKeyindentCHB->isChecked());
    if(tabKeyindentCHB->isChecked())
    {	textEditor->setTabWidth(tabWidthSpinBox->value());	}
    BkspaceIndentCHB->setChecked(settings->value("UseBackspaceIndent",false).toBool());
    textEditor->setBackspaceUnindents(BkspaceIndentCHB->isChecked());
    settings->endGroup();
    settings->endGroup();

    ///-------------------------------------------------------------------------------------
}

///****
///************************************
///*******showForm***********************************************************************************************
void OptionsDialog::slotCommOptions(void)
{
    readODWSettings();
    show();

}
///****
///****************************************
///*******SaveLastProjectName************************************************************************************
///****
void OptionsDialog::saveLastProjectName(QString fileName)
{
    settings->beginGroup("Settings/MainWindow/");
    settings->setValue("LastProjectName",fileName);
    settings->endGroup();
    settings->sync();

}
///
///***************************************
///***************openLastProject*********************************************************************************
void OptionsDialog::openLastProject()
{
    mw->markerDeleteAll();
    //if(!settings->value("/StartupPro").toBool()) return;

    if(checkBox->isChecked())
    {
        settings->beginGroup("Settings/MainWindow/");
        QString fileName =settings->value("LastProjectName").toString();
        settings->endGroup();
        textEditor->markerDeleteAll();
        mw->openFile(fileName);
        mw->refreshProfileSettings();


    }
}
///***********************************
///*******slotClose***********************************************************************************************
void OptionsDialog::slotClose(void)
{
    readODWSettings();
    close();
}
///****
///************************************
///*******slotApply***********************************************************************************************
void OptionsDialog::slotApply(void)
{
    writeSettings();
    readODWSettings();
    mw->refreshProfileSettings();
}
void OptionsDialog::slotOk(void)
{
    writeSettings();
    readODWSettings();
    mw->refreshProfileSettings();
    close();
}
void OptionsDialog::slotDefaultAll(void)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Set default settings?"));
    msgBox.setText(tr("Are you sure?"));
    //msgBox.setInformativeText();
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    /*int ret = */msgBox.exec();
    if (msgBox.clickedButton() != msgBox.defaultButton())
    {
        settings->remove("Settings/MainWindow/RecentFiles");
        settings->clear();
        readODWSettings();
    }

}
void OptionsDialog::slotChangeFormColor(void)
{
    QColor FColor=QColorDialog::getColor(QColor("blue"),this);
    if(!FColor.isValid()) return;
}
void OptionsDialog::slotChangeDefDir(int index)
{
    if(directoryBox->itemText(index)==tr("<Other...>"))
    {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                        "/home",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
        if((dir!="")&&(directoryBox->findText(dir)==-1))
        {
            directoryBox->removeItem(0);
            directoryBox->insertItem(0,dir);
        }
        directoryBox->setCurrentIndex(0);
	
    }
}
void OptionsDialog::slotChangeDefDir(QString dirName)
{

    if (QDir(dirName).exists())
    {
        errorInformLabel->setStyleSheet("");
        errorInformLabel->setText("");
        applyBtn->setEnabled(true);
        okBtn->setEnabled(true);
    }
    else
    {
        errorInformLabel->setText(tr("The path:")+dirName+tr(" not exists"));
        errorInformLabel->setStyleSheet("background-color:red;");
        applyBtn->setEnabled(false);
        okBtn->setEnabled(false);

    }
}

void OptionsDialog::slotUpdateCompilerCBox(QString lang)
{
    QStringList compilers = mw->getCurrentCompiler()->getSupportedCompilers(lang);
    compilers.sort();
    compilerComboBox->clear();
    compilerComboBox->addItems(compilers);
    QString defaultComp = readDefaultCompiler(lang);
    int index = compilerComboBox->findText(defaultComp);
    if (-1 != index) compilerComboBox->setCurrentIndex(index);
}

void OptionsDialog::slotLoadCompilerOptions(QString comp)
{
    QString lang = languageComboBox->currentText();
    QString info = mw->getCurrentCompiler()->getCompilerInfo(lang, comp);
    compilerInfo->setText(info);
    settings->beginGroup("compilation_settings");
    defaultLanguageComboBox->setCurrentIndex(settings->value("defaultLanguage/index").toString().toInt());
    QString val = languageComboBox->currentText()+"/"+compilerComboBox->currentText();
    compilerDirLineEdit->setText(settings->value(val+"/location", "").toString());
    compilerOptionsEdit->setPlainText(settings->value(val+"/options", "").toString());
    settings->endGroup();
}

void OptionsDialog::slotChangeCompilerLocation()
{
    QString comp = compilerComboBox->currentText();
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Show compiler location: (")+comp+")",
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if((""!=dir)&&(directoryBox->findText(dir)==-1))
    {
        compilerDirLineEdit->clear();
        compilerDirLineEdit->setText(dir.replace("/", QDir::separator()));
    }
}

QString OptionsDialog::readDefaultCompiler(QString lang)
{
    settings->beginGroup("compilation_settings");
    QString def = settings->value(lang+"/default", "").toString();
    settings->endGroup();
    return def;
}

QString OptionsDialog::readCompilerLocation(QString lang, QString comp)
{
    settings->beginGroup("compilation_settings");
    QString location = settings->value(lang+"/"+comp+"/location", "").toString();
    settings->endGroup();
    return location;
}

QString OptionsDialog::readCompilerOptions(QString lang, QString comp)
{
    settings->beginGroup("compilation_settings");
    QString options = settings->value(lang+"/"+comp+"/options", "").toString();
    settings->endGroup();
    return options;
}

void OptionsDialog::slotDefaultCompiler()
{
    QString lang = languageComboBox->currentText();
    settings->beginGroup("compilation_settings");
    settings->setValue(lang+"/default", compilerComboBox->currentText());
    settings->endGroup();
}

void OptionsDialog::slotResetCompilerOptions()
{
    QString lang = languageComboBox->currentText();
    QString comp = compilerComboBox->currentText();
    settings->beginGroup("default_compilation_settings");
    QString location = settings->value(lang+"/"+comp+"/location", "").toString();
    QString options = settings->value(lang+"/"+comp+"/options", "").toString();
    settings->endGroup();
    compilerDirLineEdit->setText(location);
    compilerOptionsEdit->setPlainText(options);
}
void OptionsDialog::writeMainWindowState()
{
    settings->beginGroup("Settings/MainWindow");
    settings->setValue("MainWindowState",mw->saveState());
    settings->endGroup();
    settings->sync();
}
void OptionsDialog::readMainWindowState()
{
    settings->beginGroup("Settings/MainWindow");
    //mw->restoreMainWindowState(settings->value("MainWindowState","0").toByteArray());
    mw->restoreState(settings->value("MainWindowState","0").toByteArray());
    settings->endGroup();
}
void OptionsDialog::slotChangsLocalizationLanguage(QString langName)
{
#ifdef WIN32
    translator.load(QApplication::applicationDirPath()+"/../../resources/translations/"+langName);

#else
    //translator.load(QDir::cleanPath(QApplication::applicationDirPath() + "/../../usr/share/kuzya/resources/translations/")+langName);
    translator.load(localizationLanguageDir.absolutePath()+"/"+langName);

#endif
    qApp->installTranslator(&translator);
    mw->retranslateAll();
}

void OptionsDialog::slotChangeOptionPage(int pIndex)
{
    mStackedWidget->setCurrentIndex(pIndex);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void OptionsDialog::setCompilerLocation(QString &pathToCompiler)
{
    if(languageComboBox->currentText() == "c++" && compilerComboBox->currentText() == "g++" && compilerDirLineEdit->text().isEmpty())
    {
        compilerDirLineEdit->setText(pathToCompiler);
        mw->getCurrentCompiler()->setCompilerDir(compilerDirLineEdit->text());
    }
    slotApply();
}
