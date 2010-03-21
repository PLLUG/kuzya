/***************************************************************************
 *   Copyright (C) 2009 by Volodymyr Shevchyk                              *
 *   volder@users.sourceforge.nets                                         *
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


#include "readstdin.h"
#include "ui_graphics.h"

class graphics: public QMainWindow, private Ui::graphicsForm
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
		void createPixmap(int width, int height);
                void creatBGPixmap(int width, int height);
		QString getMethodName(QString command);
                void setCurentBGColor(int curentBGColor);
                void setCurentColor(int curentColor);
                void setCurentFillColor(int curentFillColor);
                void setFillPatern(int patern);
                void drawFunc(int numOfPoints, double **arrayXY);
                void setLineStyle(int lineStyle);


	private:
		int step;
		int index;
		int curentColor;
                int curentBGColor;
                int curentFillColor;
		int textSize;
                int textDirection;
		int lineThickness;
		int lineStyle;
		int x;
		int y;
		int z;
		int x1;
		int y1;
		int r;
		int stAngle;
		int endAngle;
		int width;
		int height;
		int numOfPoints;
		int indexOfSimbol;
		int numberOf;
                bool readKomands;
                int fillPatern;
                bool BGColorWasChanged;
                double grid;
	
	private:
 		QPixmap pix;
                QPixmap pixBG;
                QPixmap FGPix;
		QPainter p;
		QString methodText;
		QString textFont;
		QPen pen;
                ReadStdIn* rsi;
                QBrush *fillBrush;
};

#endif

