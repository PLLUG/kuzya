/***************************************************************************
 *   Copyright (C) 2009 by Andriy Shevchyk   *
 *   sheva@sheva.immeditek   *
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


#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QDialog>
#include <QPainter>

#include "ui_graphics.h"

class graphics: public QDialog, private Ui::graphicsForm
{
	Q_OBJECT

	public:
		graphics(QWidget *parent = 0);
		~graphics();

	protected:

	private slots:
		void processCommand(QString  command);

	private:
		Ui::graphicsForm ui;
		void paintEvent(QPaintEvent *event);
		QString getMethodName(QString command);

	private:
		int index, x, y, x1, y1, r;		//position
 		QPixmap pix;
 		QPainter *qpainter;
		QString methodText;
};

#endif

