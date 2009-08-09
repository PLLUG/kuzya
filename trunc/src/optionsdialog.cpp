/******************************************************************************
 *   Copyright (C) 2008 by                                                    *
 *                     Victor Sklyar (bouyantgrambler@users.sourceforge.net), *
 *                     Alex Chmykhalo (alexchmykhalo@users.sourceforge.net)   *
 *                                                                            *
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
#include "optionsdialog.h"

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
        settings = new QSettings("/usr/share/kuzya/settings.ini", QSettings::IniFormat);
#endif
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
///-----------------------------Fonts and Colors-------------------------------------------------------
       styleCBox->addItems(QStyleFactory::keys());
       filters<<"*.qss";
       stylesDir.setNameFilters(filters);
#ifdef WIN32
        stylesDir=QDir(QApplication::applicationDirPath()+"/../resources/qss/");
#else
        stylesDir=QDir("/usr/share/kuzya/resources/qss/");
#endif
       slotUpdateSkinsCBox();


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
       settings->endGroup();

 ///-------------------------------------------------------------------------------------
}

///****
///************************************
///*******showForm***********************************************************************************************
void OptionsDialog::slotCommOptions(void)
{
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
}
void OptionsDialog::slotOk(void)
{
	writeSettings();
	readODWSettings();
	close();
}
void OptionsDialog::slotDefaultAll(void)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle(tr("Set default settings?"));
	msgBox.setText(tr("Are you sure?"));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
        msgBox.exec();
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


void OptionsDialog::slotUpdateCompilerCBox(QString lang)
{
        QStringList compilers = mw->getCurrentCompiler()->getSupportedCompilers(lang);
        compilerComboBox->clear();
        compilerComboBox->addItems(compilers);
}

void OptionsDialog::slotLoadCompilerOptions(QString comp)
{
       QString lang = languageComboBox->currentText();
       QString info = mw->getCurrentCompiler()->getCompilerInfo(lang, comp);
       compilerInfo->setText(info);
}

void OptionsDialog::slotChangeCompilerLocation()
{
    QString comp = compilerComboBox->currentText();
    QString dir = QFileDialog::getExistingDirectory(this,
                                                   tr("Show compiler location: (")+comp+")",
                                                   "/home",
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);
    if((dir!="")&&(directoryBox->findText(dir)==-1))
    {
        compilerDirLineEdit->clear();
        compilerDirLineEdit->setText(dir);
    }
}
