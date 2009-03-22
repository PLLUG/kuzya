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
#include <Qsci/qsciscintilla.h>
#include <QMessageBox>
#include "optionsdialog.h"
#include <QFontDialog>
#include <QColorDialog>
#include <QColor>
#include <QFileDialog>
OptionsDialog::OptionsDialog(QWidget *parent)
 : QDialog(parent)
{
	setupUi(this);
	setWindowTitle(tr("Settings"));
	trans_ua = new QTranslator();
	trans_en = new QTranslator();
        mw =(Kuzya*)parent;// (Cukr*)parentWidget();
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "LLUGDevTeam", "Kuzya");
	//readODWSettings();
	connect(closeBtn,SIGNAL(clicked()), this,SLOT(slotClose(void)));
	connect(applyBtn,SIGNAL(clicked()), this,SLOT(slotApply(void)));
	connect(okBtn,	 SIGNAL(clicked()), this,SLOT(slotOk(void)));
	connect(fontBtn, SIGNAL(clicked()), this,SLOT(slotChangeFont(void)));
	connect(defaultBtn,SIGNAL(clicked()),this,SLOT(slotDefaultAll(void)));
        connect(pascalBtn, SIGNAL(clicked()), this, SLOT(slotLoadCompilerSettings(void)));
        connect(cppBtn, SIGNAL(clicked()), this, SLOT(slotLoadCompilerSettings(void)));
//	connect(formClrBtn,SIGNAL(clicked()),this,SLOT(slotChangeFormColor(void)));
	connect(directoryBox,SIGNAL(activated(int)),this,SLOT(slotChangeDefDir(int)));
}
void OptionsDialog::slotLoadCompilerSettings(void)
{
       settings->beginGroup("Settings");
                if (cppBtn->isChecked())
                {
                        settings->beginGroup("Prog_Lang_Cpp");
                        profileDirLineEdit->setText(settings->value("profileDir").toString());
                        compilerDirLineEdit->setText(settings->value("compilerDir").toString());
                        libDirLineEdit->setText(settings->value("libDir").toString());
                        includeDirLineEdit->setText(settings->value("includeDir").toString());
                        compilerOptionsLineEdit->setText(settings->value("compilerOptions").toString());
                        settings->endGroup();
                }
                else
                {
                        settings->beginGroup("Prog_Lang_Pascal");
                        profileDirLineEdit->setText(settings->value("profileDir").toString());
                        compilerDirLineEdit->setText(settings->value("compilerDir").toString());
                        libDirLineEdit->setText(settings->value("libDir").toString());
                        includeDirLineEdit->setText(settings->value("includeDir").toString());
                        compilerOptionsLineEdit->setText(settings->value("compilerOptions").toString());
                        settings->endGroup();
                }
        settings->endGroup();
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
	fontLable->setText(s.remove(s.indexOf(","),s.count()-s.indexOf(",")));
	//mw->setEditorFont(font);
	qApp->setFont(font);
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
        settings->beginGroup("Settings");
		if (cppBtn->isChecked())
		{
                        settings->setValue("Prog_Lang","cpp");
                        settings->beginGroup("Prog_Lang_Cpp");
                        settings->setValue("profileDir",profileDirLineEdit->text());
                        settings->setValue("compilerDir",compilerDirLineEdit->text());
                        settings->setValue("libDir",libDirLineEdit->text());
                        settings->setValue("includeDir",includeDirLineEdit->text());
                        settings->setValue("compilerOptions",compilerOptionsLineEdit->text());
                        settings->endGroup();

		}
		else
		{
                        settings->setValue("Prog_Lang","pascal");
                        settings->beginGroup("Prog_Lang_Pascal");
                        settings->setValue("profileDir",profileDirLineEdit->text());
                        settings->setValue("compilerDir",compilerDirLineEdit->text());
                        settings->setValue("libDir",libDirLineEdit->text());
                        settings->setValue("includeDir",includeDirLineEdit->text());
                        settings->setValue("compilerOptions",compilerOptionsLineEdit->text());
                        settings->endGroup();
                }
        settings->endGroup();
///-----RECENT FILES------------------------------------------------------------------
	
        settings->beginWriteArray("RecentFiles");
		remove("");///?????????????????????????????????????????????????????????????????
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
}
///****
///************************************
///*******readOtionDialogWindowSettings***************************************************************************
void OptionsDialog::readODWSettings()
{	
        settings->beginGroup("Settings/MainWindow");
                checkBox->setChecked(settings->value("StartupPro",false).toBool());
		///-----------------------------------------------------------------------------
                sB_LOFCount->setValue(settings->value("LOFCount",5).toInt());
		mw->setMaxCount_RFileList(sB_LOFCount->value());

///-----LANGUAGE-------------------------------------------------------------------------
                if(settings->value("Language","eng").toString()=="ukr")
		{
			ukrRBtn->setChecked(true);
			translator.load(QApplication::applicationDirPath()+"/../trunc/src/translations/kuzya_ua");
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
                if(directoryBox->findText(settings->value("DefaultDir","/home").toString())==-1)
		{
			directoryBox->removeItem(0);
                        directoryBox->insertItem(0,settings->value("DefaultDir","/home").toString());
			directoryBox->setCurrentIndex(0);	
		}
		mw->setDefaultDir(directoryBox->currentText());
		///------------------------------------------------------------------------------
        settings->endGroup();
///-----PROGRAMING--LANGUAGE---------------------------------------------------
        settings->beginGroup("Settings");
                if(settings->value("Prog_Lang","cpp").toString()=="cpp")
		{
			cppBtn->setChecked(true);
                        mw->loadCPPLexer();
                        settings->beginGroup("Prog_Lang_Cpp");
                        profileDirLineEdit->setText(settings->value("profileDir").toString());
                        compilerDirLineEdit->setText(settings->value("compilerDir").toString());
                        libDirLineEdit->setText(settings->value("libDir").toString());
                        includeDirLineEdit->setText(settings->value("includeDir").toString());
                        compilerOptionsLineEdit->setText(settings->value("compilerOptions").toString());
                        settings->endGroup();
                        mw->getCurrentCompiler()->loadProfile(profileDirLineEdit->text()+QString("c_profile.ini"));
                        mw->getCurrentCompiler()->setOptions(compilerOptionsLineEdit->text());
                        mw->getCurrentCompiler()->setIncludeDir(includeDirLineEdit->text());
                        mw->getCurrentCompiler()->setCompilerDir(compilerDirLineEdit->text());
                        mw->getCurrentCompiler()->setLibDir(libDirLineEdit->text());
}
		else
		{
			pascalBtn->setChecked(true);
                        settings->beginGroup("Prog_Lang_Pascal");
                        profileDirLineEdit->setText(settings->value("profileDir").toString());
                        compilerDirLineEdit->setText(settings->value("compilerDir").toString());
                        libDirLineEdit->setText(settings->value("libDir").toString());
                        includeDirLineEdit->setText(settings->value("includeDir").toString());
                        compilerOptionsLineEdit->setText(settings->value("compilerOptions").toString());
                        settings->endGroup();
                        mw->loadCPPLexer();
                        mw->getCurrentCompiler()->loadProfile(profileDirLineEdit->text()+QString("pascal_profile.ini"));
                        mw->getCurrentCompiler()->setOptions(compilerOptionsLineEdit->text());
                        mw->getCurrentCompiler()->setIncludeDir(includeDirLineEdit->text());
                        mw->getCurrentCompiler()->setCompilerDir(compilerDirLineEdit->text());
                        mw->getCurrentCompiler()->setLibDir(libDirLineEdit->text());
                        mw->loadPascalLexer();
		}	
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
                        font=QFont(settings->value("FontName","").toString());
                        QString s=(settings->value("FontName",qApp->font()).toString());
			fontLable->setText(s.remove(s.indexOf(","),s.count()-s.indexOf(",")));
                        font.setPointSize(settings->value("FontSize",10).toInt());
                        font.setItalic(settings->value("FontItalic",false).toBool());
                        font.setBold(settings->value("FontBold",false).toBool());
			//mw->setEditorFont(font);
			qApp->setFont(font);
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
        QString fileName =settings->value("LastProjectName","/home/volder/Projects/Svitlofor/src/main.cpp").toString();
        settings->endGroup();
	mw->openFile(fileName);
	}
}
///***********************************
///*******slotClose***********************************************************************************************
void OptionsDialog::slotClose(void)
{
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
	//msgBox.setInformativeText();
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	int ret = msgBox.exec();
	if (msgBox.clickedButton() != msgBox.defaultButton()) 
	{
                settings->remove("Settings/MainWindow/RecentFiles");
                settings->clear();
		readODWSettings();
	}
		
}
void OptionsDialog::slotChangeFormColor(void)
{
	bool ok;
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
