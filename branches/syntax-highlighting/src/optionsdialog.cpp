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
#include <QMessageBox>
#include <QFontDialog>
#include <QColorDialog>
#include <QColor>
#include <QFileDialog>
#include <QDebug>
#include "optionsdialog.h"

 #include <QPalette>
OptionsDialog::OptionsDialog(QWidget *parent)
 : QDialog(parent)
{
	setupUi(this);
	setWindowTitle(tr("Settings"));
	trans_ua = new QTranslator();
	trans_en = new QTranslator();
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
        connect(fontBtn_5, SIGNAL(clicked()), this,SLOT(slotChangeFont(void)));
	connect(defaultBtn,SIGNAL(clicked()),this,SLOT(slotDefaultAll(void)));
        connect(directoryBox,SIGNAL(activated(int)),this,SLOT(slotChangeDefDir(int)));
        connect(styleCBox, SIGNAL(activated(int)), this ,SLOT(slotChangeStyle(int)));
        connect(skinCBox, SIGNAL(activated(QString)), this, SLOT(slotChangeSkin(QString)));
        connect(languageComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotUpdateCompilerCBox(QString)));
        connect(compilerComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slotLoadCompilerOptions(QString)));
        connect(browseButton, SIGNAL(clicked()), this, SLOT(slotChangeCompilerLocation()));
        connect(defaultUsePushButton, SIGNAL(clicked()), this, SLOT(slotDefaultCompiler()));
        connect(compilerResetPushButton, SIGNAL(clicked()) ,this, SLOT(slotResetCompilerOptions()));

        connect(fgColorPanelBtn,SIGNAL(clicked()),this,SLOT(slotChangeFgColor()));
        connect(bgColorPanelBtn,SIGNAL(clicked()),this,SLOT(slotChangeBgColor()));
        connect(cleanBgColorBtn,SIGNAL(clicked()),this,SLOT(slotCleanBgColor()));
        connect(tEColorShemeCBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeColorScheme(int)));
        connect(lexerLanguageCBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeLexerLenguage(int)));
        connect(tEStylesList,SIGNAL(itemSelectionChanged()),this,SLOT(slotLoadCurrentStyleItemProperties()));
        connect(cleanBgColorBtn,SIGNAL(clicked()),this,SLOT(slotSetDefaultBgColor()));
        /*connect(teFontStyle_BoldCheckBox,SIGNAL(clicked(bool)),this,SLOT(slotSetBoldStyle(bool)));
        connect(teFontStyle_ItalicCheckBox,SIGNAL(clicked(bool)),this,SLOT(slotSetItalicStyle(bool)));*/
        connect(teFontStyle_BoldCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotSetBoldStyle(bool)));
        connect(teFontStyle_ItalicCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotSetItalicStyle(bool)));
        //connect(tEFontSizeCBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotSetFontSize(QString)));
        connect(tEFontNameFCBox,SIGNAL(currentFontChanged(QFont)),this,SLOT(slotSetFontName(QFont)));
///-----------------------------Fonts and Colors-------------------------------------------------------
       styleCBox->addItems(QStyleFactory::keys());
       filters<<"*.qss";
       stylesDir.setNameFilters(filters);
#ifdef WIN32
        stylesDir=QDir(QApplication::applicationDirPath()+"/../resources/qss/");
		colorSchemesDir=(QDir(QApplication::applicationDirPath()+"/../resources/ColorSchemes/"));
#else
        stylesDir=QDir("/usr/share/kuzya/resources/qss/");
        //colorSchemesDir=QDir("/usr/share/kuzya/resources/ColorSchemes/");
        colorSchemesDir=QDir("./../resources/ColorSchemes/");
#endif
       slotUpdateSkinsCBox();
       languageComboBox->clear();
       QStringList supportedList = mw->getCurrentCompiler()->getSupportedLanguages();
       supportedList.sort();
       languageComboBox->addItems(supportedList);
       slotUpdateColorSchemesBox();
       //slotUpdateLexerLanguageBox();

}
void OptionsDialog::slotUpdateSkinsCBox(void)
{
     skinCBox->addItems(stylesDir.entryList(stylesDir.nameFilters(),QDir::Files,QDir::Name));
}


OptionsDialog::~OptionsDialog()
{
}
void OptionsDialog::retranslate(void)
{
	retranslateUi(this);
}

void OptionsDialog::slotChangeFont()
{
	bool ok;
	font=QFontDialog::getFont( &ok, font, this);
	QString s=font.toString();
        fontLable_5->setText(s.remove(s.indexOf(","),s.count()-s.indexOf(",")));
	//mw->setEditorFont(font);
	qApp->setFont(font);
}
void OptionsDialog::slotChangeStyle(int)
{
    qApp->setStyle(styleCBox->currentText());
}
void OptionsDialog::slotChangeSkin(QString sheetName)
{
#ifdef WIN32
    QFile file(QApplication::applicationDirPath()+"/../resources/qss/"+sheetName.toLower());
#else
    QFile file("/usr/share/kuzya/resources/qss/"+sheetName.toLower());
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
		if (ukrRBtn->isChecked())
		{
                        settings->setValue("Language","ukr");
			
		}
		else
		{
                        settings->setValue("Language","eng");
		}
		///-------------------------------------------------------------------------------------------
        settings->endGroup();
///-----PROGRAMING--LANGUAGE---------------------------------------------------
        settings->beginGroup("compilation_settings");
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
///-----EDITOR--------------------------------------------------------------------------------	
        settings->beginGroup("Settings/TextEditor");
                settings->beginGroup("Font");
                settings->setValue("FontName",font.toString());
                settings->setValue("FontSize",font.pointSize());
                settings->setValue("FontItalic",font.italic());
                settings->setValue("FontBold",font.bold());
                settings->endGroup();
        settings->endGroup();

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
                settings->beginGroup("/LexerStyle");
                        settings->setValue("ColorSchemeFile",tEColorShemeCBox->currentText());
                settings->endGroup();
        settings->endGroup();
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
                    skinCBox->setCurrentIndex(skinCBox->findText(settings->value("Skin","default.qss").toString()));
                    slotChangeSkin(settings->value("Skin","default.qss").toString());
                settings->endGroup();
///-----LANGUAGE-------------------------------------------------------------------------
                if(settings->value("Language","eng").toString()=="ukr")
		{
			ukrRBtn->setChecked(true);
#ifdef WIN32
                        translator.load(QApplication::applicationDirPath()+"/../resources/translations/kuzya_ua");

#else
                        translator.load("/usr/share/kuzya/resources/translations/kuzya_ua");    //QApplication::applicationDirPath()+"/../trunc/src/translations/kuzya_ua"

#endif
			///translator.load("./src/translations/kuzya_ua");
			qApp->installTranslator(&translator);
			mw->retranslateAll();			
		}
		else
		{
			engRBtn->setChecked(true);
			qApp->removeTranslator(&translator);
			mw->retranslateAll();			
		}
///-----DefaultDirectory-------------------------------------------------------------------------
                if(directoryBox->findText(settings->value("DefaultDir",QDir::homePath()).toString())==-1)
		{
			directoryBox->removeItem(0);
                        directoryBox->insertItem(0,settings->value("DefaultDir",QDir::homePath()).toString());
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
///-----EDITOR--------------------------------------------------------------------------------	
        settings->beginGroup("Settings/TextEditor");
                settings->beginGroup("Font");
                        font=QFont(settings->value("FontName","Arial Cyr").toString());
                        QString s=(settings->value("FontName",qApp->font()).toString());
                        fontLable_5->setText(s.remove(s.indexOf(","),s.count()-s.indexOf(",")));
                        font.setPointSize(settings->value("FontSize",10).toInt());
                        font.setItalic(settings->value("FontItalic",false).toBool());
                        font.setBold(settings->value("FontBold",false).toBool());
			//mw->setEditorFont(font);
			qApp->setFont(font);
                settings->endGroup();
        settings->endGroup();

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
                settings->beginGroup("/LexerStyle");
                        if(settings->value("ColorSchemeFile","none").toString()!="none")
                        {
                            tEColorShemeCBox->setCurrentIndex(tEColorShemeCBox->findText(settings->value("ColorSchemeFile","none").toString()));
                        }
                settings->endGroup();
       settings->endGroup();

 ///-------------------------------------------------------------------------------------
}

///****
///************************************
///*******showForm***********************************************************************************************
void OptionsDialog::slotCommOptions(void)
{
        lexerLanguageCBox->setCurrentIndex(lexerLanguageCBox->findText(mw->currentProgramingLanguage()));
        tEStylesList->setCurrentRow(0);
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
            mw->openFile(fileName);
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
        saveColorSchemeProperties();
        mw->setHighlighting(mw->currentProgramingLanguage(),colorSchemesDir.absolutePath()+"/"+tEColorShemeCBox->currentText());
}
void OptionsDialog::slotOk(void)
{
	writeSettings();
	readODWSettings();
        mw->refreshProfileSettings();
        mw->setHighlighting(mw->currentProgramingLanguage(),colorSchemesDir.absolutePath()+"/"+tEColorShemeCBox->currentText());
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
/*
bool OptionsDialog::ukrIsCheked()
{
    return ukrRBtn_2->isChecked();
}
*/
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
void OptionsDialog::slotChangeFgColor()
{
    QColor color = QColorDialog::getColor();
    /*QPalette palette = fgColorPanelBtn->palette();
    palette .setColor( QPalette::Active, QPalette::Button,color);
    fgColorPanelBtn->setPalette(palette);
    fgColorPanelBtn->setAutoFillBackground( true );*/
    //fgColorPanelBtn->setStyleSheet("* { background-color: rgb(255,125,100) }");
    if (color.isValid()&&((tEStylesList->currentRow()!=(-1))))
    {
        fgColorPanelBtn->setStyleSheet("* { background-color: "+color.name() +"}");
        tEStylesList->currentItem()->setForeground(color);
    }

}
void OptionsDialog::slotChangeBgColor()
{
    QColor color = QColorDialog::getColor();
    if (color.isValid()&&(tEStylesList->currentRow()!=(-1)))
    {
        bgColorPanelBtn->setStyleSheet("* { background-color: "+color.name() +"}");
        tEStylesList->currentItem()->setBackgroundColor(color);
    }

}
void OptionsDialog::slotCleanBgColor()
{
    
    bgColorPanelBtn->setStyleSheet("* { background-color: #FFFFFF }");
}
void OptionsDialog::slotUpdate_tEStylesList(QString fileName, bool reloadLexerLanguagesList)
{
    tEStylesList->clear();
    QDomDocument domDoc;
    QFile file(colorSchemesDir.absolutePath()+"/"+fileName);      //file with lexer's preferences
    if (!file.open(QIODevice::ReadOnly))
    {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File"+ fileName + "is mising.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         return;
    }
    if (!domDoc.setContent(&file))  //sets file as the content of the document
    {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File "+fileName+" is corrupted.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         file.close();
         return;
     }
     file.close();

     QDomElement domElement = domDoc.documentElement();
     QDomNode domNode = domElement.firstChild();
     domNode=domNode.nextSibling();
     domNode=domNode.firstChild();
     //qDebug()<<domNode.toElement().tagName();
     //qDebug()<<domNode.toElement().attribute("name","");
     //qDebug()<<"**********"<<languageNameStr;
     if (true==reloadLexerLanguagesList)
     {
        disconnect(lexerLanguageCBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeLexerLenguage(int)));
        lexerLanguageCBox->clear();
        while(!domNode.isNull())
        {
           lexerLanguageCBox->addItem(domNode.toElement().attribute("name",""));
           domNode=domNode.nextSibling();
        }

        connect(lexerLanguageCBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slotChangeLexerLenguage(int)));
     }
     domNode = domElement.firstChild();
     domNode=domNode.nextSibling();
     domNode=domNode.firstChild();
     while((lexerLanguageCBox->currentText()!=domNode.toElement().attribute("name",""))&&(!domNode.isNull()))
     {
         //qDebug()<<domNode.toElement().attribute("name","");
         domNode=domNode.nextSibling();
     }
     if(lexerLanguageCBox->currentText()!=domNode.toElement().attribute("name",""))
     {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File "+fileName+"is corrupted.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         file.close();
         return;
     }
     QDomNode styleNode = domNode.namedItem("LexerStyle");
     QDomElement wordStyleDomElement = styleNode.firstChildElement();

 //    tEFontSizeCBox->setCurrentIndex(tEFontSizeCBox->findText(styleNode.toElement().attribute("defaultFontSize","9")));

     tEFontNameFCBox->setCurrentFont(QFont(styleNode.toElement().attribute("defaultFontName","")));
     while(!wordStyleDomElement.isNull())
     {
         QListWidgetItem *item = new QListWidgetItem;
         QFont font;
         QFontDatabase fontDataBase;
         item->setText(wordStyleDomElement.attribute("name",""));

         if(fontDataBase.families().contains(wordStyleDomElement.attribute("fontName","no")))
         {
             font=QFont(wordStyleDomElement.attribute("fontName","Monospace"));
         }
         else
         {
             font=QFont(styleNode.toElement().attribute("defaultFontName","Monospace"));
             font.setPointSize(styleNode.toElement().attribute("defaultFontSize","9").toInt());
         }
         if (""!=wordStyleDomElement.attribute("fontSize",styleNode.toElement().attribute("defaultFontSize","9")))
         {
             font.setPointSize(wordStyleDomElement.attribute("fontSize",styleNode.toElement().attribute("defaultFontSize","9")).toInt());
         }
         else
         {
              font.setPointSize(styleNode.toElement().attribute("defaultFontSize","9").toInt());
         }
         if (1==wordStyleDomElement.attribute("fontStyle","-1").toInt())
         {
             //= item->font();
             font.setBold(true);
             item->setFont(font);
         }
         if (2==wordStyleDomElement.attribute("fontStyle","-1").toInt())
         {
             //item->font();
             font.setItalic(true);
             item->setFont(font);
         }
         item->setForeground(QColor(wordStyleDomElement.attribute("fgColor","#808080")));
         item->setBackgroundColor(QColor(wordStyleDomElement.attribute("bgColor","#FFFFFF")));
         tEStylesList->addItem(item);
         wordStyleDomElement=wordStyleDomElement.nextSiblingElement();
         //qDebug()<<wordStyleDomElement.attribute("name","");
     }


}
void OptionsDialog::slotUpdateColorSchemesBox()
{
    tEColorShemeCBox->clear();
    tEColorShemeCBox->addItems(colorSchemesDir.entryList(colorSchemesDir.nameFilters(),QDir::Files,QDir::Name));
    slotUpdate_tEStylesList(tEColorShemeCBox->currentText(),true);

}
void OptionsDialog::slotChangeColorScheme(int index)
{

    slotUpdate_tEStylesList(tEColorShemeCBox->itemText(index),true);
    lexerLanguageCBox->setCurrentIndex(lexerLanguageCBox->findText(mw->currentProgramingLanguage()));

    if(tEStylesList->count()!=0)
    {
        teFontStyle_BoldCheckBox->setEnabled(true);
        teFontStyle_ItalicCheckBox->setEnabled(true);
        fgColorPanelBtn->setEnabled(true);
        bgColorPanelBtn->setEnabled(true);
        tEStylesList->setCurrentRow(0);
    }
    else
    {
        teFontStyle_BoldCheckBox->setEnabled(false);
        teFontStyle_ItalicCheckBox->setEnabled(false);
        fgColorPanelBtn->setEnabled(false);
        bgColorPanelBtn->setEnabled(false);
    }
}
void OptionsDialog::slotUpdateLexerLanguageBox()
{
    slotUpdate_tEStylesList(tEColorShemeCBox->currentText(),true);
    if(tEStylesList->count()!=0)
    {        
        tEStylesList->setCurrentRow(0);
    }

}
void OptionsDialog::slotChangeLexerLenguage(int index)
{
    slotUpdate_tEStylesList(tEColorShemeCBox->currentText(),false);
    if(tEStylesList->count()!=0)
    {
        teFontStyle_BoldCheckBox->setEnabled(true);
        teFontStyle_ItalicCheckBox->setEnabled(true);
        fgColorPanelBtn->setEnabled(true);
        bgColorPanelBtn->setEnabled(true);
        tEStylesList->setCurrentRow(0);
    }
    else
    {
        teFontStyle_BoldCheckBox->setEnabled(false);
        teFontStyle_ItalicCheckBox->setEnabled(false);
        fgColorPanelBtn->setEnabled(false);
        bgColorPanelBtn->setEnabled(false);
    }
}
void OptionsDialog::slotLoadCurrentStyleItemProperties()
{
    bgColorPanelBtn->setStyleSheet("* { background-color: "+tEStylesList->currentItem()->backgroundColor().name()+"}");
    fgColorPanelBtn->setStyleSheet("* { background-color: "+tEStylesList->currentItem()->foreground().color().name()+"}");
    disconnect(teFontStyle_BoldCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotSetBoldStyle(bool)));
    disconnect(teFontStyle_ItalicCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotSetItalicStyle(bool)));
  //  disconnect(tEFontSizeCBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotSetFontSize(QString)));
    teFontStyle_BoldCheckBox->setChecked(tEStylesList->currentItem()->font().bold());
    teFontStyle_ItalicCheckBox->setChecked(tEStylesList->currentItem()->font().italic());
   // tEFontSizeCBox->setCurrentIndex(tEFontSizeCBox->findText("20"));
    connect(teFontStyle_BoldCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotSetBoldStyle(bool)));
    connect(teFontStyle_ItalicCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotSetItalicStyle(bool)));
  //  connect(tEFontSizeCBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(slotSetFontSize(QString)));


}
QString OptionsDialog::currentLexerColorSchemeFileName()
{
    return (colorSchemesDir.absolutePath()+"/"+ tEColorShemeCBox->currentText());
}
void OptionsDialog::saveColorSchemeProperties()
{
    QDomDocument domDoc;
    QFile file(colorSchemesDir.absolutePath()+"/"+tEColorShemeCBox->currentText());      //file with lexer's preferences
    if (!file.open(QIODevice::ReadOnly))
    {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File"+ tEColorShemeCBox->currentText() + "is mising.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         return;
    }
    if (!domDoc.setContent(&file))  //sets file as the content of the document
    {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File "+tEColorShemeCBox->currentText()+" is corrupted.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         file.close();
         return;
     }
     file.close();

     QDomElement domElement = domDoc.documentElement();
     QDomNode domNode = domElement.firstChild();
     domNode=domNode.nextSibling();
     domNode=domNode.firstChild();
     while((lexerLanguageCBox->currentText()!=domNode.toElement().attribute("name",""))&&(!domNode.isNull()))
     {
         //qDebug()<<domNode.toElement().attribute("name","");
         domNode=domNode.nextSibling();
     }
     if(lexerLanguageCBox->currentText()!=domNode.toElement().attribute("name",""))
     {
         QMessageBox msgBox;
         msgBox.setIcon(QMessageBox::Warning);
         msgBox.setText("<b>Error</b>");
         msgBox.setInformativeText("File "+tEColorShemeCBox->currentText()+"is corrupted.\nPlease reinstall application!");
         msgBox.setStandardButtons(QMessageBox::Ok);
         msgBox.setDefaultButton(QMessageBox::Ok);
         msgBox.exec();
         file.close();
         return;
     }
     QDomNode styleNode = domNode.namedItem("LexerStyle");
     styleNode.toElement().setAttribute("defaultFontName",tEFontNameFCBox->currentFont().family());
     //styleNode.toElement().setAttribute("defaultFontSize",tEFontSizeCBox->currentText());
     QDomElement wordStyleDomElement = styleNode.firstChildElement();
     int i = 0;
     while(!wordStyleDomElement.isNull())
     {
        wordStyleDomElement.setAttribute("fgColor",tEStylesList->item(i)->foreground().color().name());
        wordStyleDomElement.setAttribute("bgColor",tEStylesList->item(i)->backgroundColor().name());
        wordStyleDomElement.setAttribute("fontStyle",0) ;
        if ((tEStylesList->item(i)->font().bold()) && (tEStylesList->item(i)->font().italic())) wordStyleDomElement.setAttribute("fontStyle",3);
        else
        {
            if (tEStylesList->item(i)->font().bold())  wordStyleDomElement.setAttribute("fontStyle",1);
            if (tEStylesList->item(i)->font().italic()) wordStyleDomElement.setAttribute("fontStyle",2);
        }
        wordStyleDomElement=wordStyleDomElement.nextSiblingElement();
         i++;
     }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("<b>Error</b>");
        msgBox.setInformativeText("Cannot save changes into"+ tEColorShemeCBox->currentText() );
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    QTextStream out(&file);
    out << domDoc.toString();
    return;
}
void OptionsDialog::slotSetDefaultBgColor()
{
    tEStylesList->currentItem()->setBackgroundColor(tEStylesList->item(0)->backgroundColor());
    bgColorPanelBtn->setStyleSheet("* { background-color: "+tEStylesList->item(0)->backgroundColor().name() +"}");
}
void OptionsDialog::slotSetBoldStyle(bool b)
{
    if(tEStylesList->currentRow()!=-1)
    {
        QFont font = tEStylesList->currentItem()->font();
        font.setBold(b);
        tEStylesList->currentItem()->setFont(font);
    }
}
void OptionsDialog::slotSetItalicStyle(bool b)
{
    if(tEStylesList->currentRow()!=-1)
    {
        QFont font = tEStylesList->currentItem()->font();
        font.setItalic(b);
        tEStylesList->currentItem()->setFont(font);
    }
}
void OptionsDialog::slotSetFontSize(QString size)
{
    QFont font ;
    for(int i=0;i<tEStylesList->count();i++)
    {
     /*  font = tEStylesList->item(i)->font();
        font.setPointSize(size.toInt());
        tEStylesList->item(i)->setFont(font);*/

    }qDebug()<<size;


}
void OptionsDialog::slotSetFontName(QFont font)
{
    for(int i=0;i<tEStylesList->count();i++)
    {
        tEStylesList->item(i)->setFont(font);
    }
}