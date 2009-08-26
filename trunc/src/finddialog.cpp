/***************************************************************************
 *   Copyright (C) 2008 by Volodymyr Shevchyk                              *
 *   volder@users.sourceforge.net                                          *
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
#include "finddialog.h"

FindDialog::FindDialog(QWidget *parent)
 : QDialog(parent)//, Ui::findForm()
{
	setupUi(this);
	textEditor=(QsciScintilla*)parentWidget();	
	
	connect(findBtn,SIGNAL(clicked()),this,SLOT(slotFind()));
	connect(closeBtn,SIGNAL(clicked()),this,SLOT(close()));
	connect(fromcurCB,SIGNAL(stateChanged(int)),this,SLOT(slotChangeCursPos(int)));
	
	
	
}
FindDialog::~FindDialog()
{
}
void FindDialog::retranslate(void)
{
        retranslateUi(this);
}
void FindDialog::slotFindDialog(void)
{
	show();
	if(findComboBox->findText(textEditor->selectedText())==-1)
	{
		if(findComboBox->count()>5) findComboBox->clear();
		findComboBox->insertItem(0,textEditor->selectedText());
		findComboBox->setCurrentIndex(0);
	}
}

void FindDialog::slotFind(void)
{
	if(forwardRBtn->isChecked())
	{
		if(!textEditor->findFirst(findComboBox->currentText(),regExprCB->isChecked(),caseSensCB->isChecked(),		wWOnlyCB->isChecked(),wrapCB->isChecked(),forwardRBtn->isChecked(),-1,-1,true))
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle(tr("Sorry"));
			msgBox.setText(tr("No matches found for:  ")+findComboBox->currentText());
			msgBox.exec();
		}
	}
	else
	{
		textEditor->findFirst(findComboBox->currentText(),regExprCB->isChecked(),caseSensCB->isChecked(),wWOnlyCB->isChecked(),wrapCB->isChecked(),forwardRBtn->isChecked(),-1,-1,true);
		if(!textEditor->findNext())
		{
			QMessageBox msgBox;
			msgBox.setWindowTitle(tr("Sorry"));
			msgBox.setText(tr("No matches found for:  ")+findComboBox->currentText());
			msgBox.exec();
		}
	}
	
}

void FindDialog::slotChangeCursPos(int state)
{
	if(state!=2)
	{
		if(forwardRBtn->isChecked())
		{
			textEditor->setCursorPosition(0,0);
		}
		if(!forwardRBtn->isChecked())
		{
			textEditor->setCursorPosition(textEditor->lines(),textEditor->lineLength(textEditor->lines()));
		}
	}
}
