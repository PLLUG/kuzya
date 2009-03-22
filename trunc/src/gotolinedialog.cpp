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

#include "gotolinedialog.h"

GoToLineDialog::GoToLineDialog(QWidget *parent)
 : QDialog(parent)
{
	setupUi(this);

	textEditor = (QsciScintilla*)parentWidget();

	connect(goButton, SIGNAL(clicked()), this, SLOT(goAndCloseSlot()));
}


GoToLineDialog::~GoToLineDialog()
{
}

void GoToLineDialog::retranslate(void)
{
	retranslateUi(this);
}
void GoToLineDialog::slotGoToLine(void)
{
	textEditor = (QsciScintilla*)parentWidget();
	horizontalSlider->setMaximum(textEditor->lines());
	spinBox->setMaximum(textEditor->lines());
	horizontalSlider->setMinimum(1);
	spinBox->setMinimum(1);
	show();
}

void GoToLineDialog::goAndCloseSlot(void)
{
	textEditor->setCursorPosition(horizontalSlider->value() - 1, 0);
	close();
}

