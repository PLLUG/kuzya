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
#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QObject>
#include <QDialog>
#include "ui_replacedialog.h"
class QsciScintilla;
/**
	@author Sklyar Victor
*/
class ReplaceDialog : public QDialog, private Ui::replaceForm
{
		Q_OBJECT
	public:
		ReplaceDialog ( QWidget *parent = 0 );
		~ReplaceDialog();

		void retranslate ( void );

		void promptMessage ( void );
	public slots:
		void slotReplaceDialog ( void );
	private slots:
		void slotReplace ( void );
		void slotReplaceOnce ( void );
		void slotReplaceAll ( void );
		void slotReplace_Close ( void );
		void slotFindNext ( void );
		void slotCancel ( void );
	private:
		QsciScintilla* textEditor;
		QWidget *promptMessageDialog;
};

#endif
