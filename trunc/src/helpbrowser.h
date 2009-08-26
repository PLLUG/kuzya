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
#ifndef _HelpBrowser_h_
#define _HelpBrowser_h_
#include <QtGui>
#include <QObject>
#include <QPushButton>
class HelpBrowser:public QWidget
{
		Q_OBJECT
	public:HelpBrowser (	const QString& strPath,
		                     const QString& strFileName,
		                     QWidget* pwgt=0 ) :QWidget ( pwgt )
		{
			setAttribute(Qt::WA_DeleteOnClose);
			setAttribute(Qt::WA_GroupLeader);
			QPushButton* pcmdBack = new QPushButton ( "<<" );
			QPushButton* pcmdHome=new QPushButton ( "Home" );
			QPushButton* pcmdForward = new QPushButton ( ">>" );
			QTextBrowser* ptxtBrowser = new QTextBrowser;
			connect ( pcmdBack,SIGNAL ( clicked() ),ptxtBrowser,SLOT ( backward() ) );
			connect ( pcmdHome,SIGNAL ( clicked() ),ptxtBrowser,SLOT ( home() ) );
			connect ( pcmdForward,SIGNAL ( clicked() ),ptxtBrowser,SLOT ( forward() ) );
			connect ( ptxtBrowser,SIGNAL ( backwardAvailable ( bool ) ),pcmdBack,SLOT (setEnabled(bool)));
			connect ( ptxtBrowser,SIGNAL ( forwardAvailable ( bool ) ),pcmdForward,SLOT (setEnabled(bool)));
			ptxtBrowser->setSearchPaths(QStringList() << strPath);
			//ptxtBrowser->setPlainText(QString(strFileName));
			ptxtBrowser->setSource(QString(strFileName));
//Layoutsetup
			QVBoxLayout*pvbxLayout = new QVBoxLayout;
			QHBoxLayout*phbxLayout = new QHBoxLayout;
			phbxLayout->addWidget ( pcmdBack );
			phbxLayout->addWidget ( pcmdHome );
			phbxLayout->addWidget ( pcmdForward );
			pvbxLayout->addLayout ( phbxLayout );
			pvbxLayout->addWidget ( ptxtBrowser );
			setLayout ( pvbxLayout );
		}
};
#endif
