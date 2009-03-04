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


#include <QtGui>
#include <QTextStream>
#include <QPoint>


#include "readstdin.h"

#include "graphics.h"

graphics::graphics(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setObjectName("stdintest");	

	pix = QPixmap(490,490);
	pix.fill(Qt::transparent);

	ReadStdIn* rsi = new ReadStdIn(this);
	connect(rsi, SIGNAL(commandAppeared(QString)), this, SLOT(processCommand(QString)));
	rsi->start();
}

graphics::~graphics()
{
}

void graphics::processCommand(QString  command)
{
	QPainter p(&pix);
	int indexOfSimbol, numberOf;
	if(getMethodName(command) == "setColor")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
		methodText = "Qt::" + command.mid(index+1, numberOf-1);

		p.setPen(methodText);
	}
	if(getMethodName(command) == "point")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
		x = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index);
		numberOf = indexOfSimbol - index;
		y = command.mid(index+1, numberOf-1).toInt(0,10);
		
    	p.drawPoint(x, y);
    	update();	
	}
	if(getMethodName(command) == "ellipse")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
		x = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		y = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		x1 = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		y1 = command.mid(index+1, numberOf-1).toInt(0,10);

		p.drawEllipse(x, y, x1, y1);
		update();
	}
	if (getMethodName(command) == "outTextXY")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
		x = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		y = command.mid(index+1, numberOf-1).toInt(0,10);

		index = command.indexOf("\"", 0);
		indexOfSimbol = command.indexOf("\"", index+ 1);
		numberOf = indexOfSimbol - index;
		methodText = command.mid(index+1, numberOf-1);

		p.drawText(x, y, methodText);
		update();
	}
	if(getMethodName(command) == "line")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
		x = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		y = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		x1 = command.mid(index+1, numberOf-1).toInt(0,10);
	
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		y1 = command.mid(index+1, numberOf-1).toInt(0,10);

		p.drawLine(x, y, x1,y1);
		update();
	}
	if (getMethodName(command) == "rectangle")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
		x = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		y = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		x1 = command.mid(index+1, numberOf-1).toInt(0,10);
	
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		y1 = command.mid(index+1, numberOf-1).toInt(0,10);
	
		p.drawRect(x, y, x1-x, y1-y);
		update();
	}
	if (getMethodName(command) == "setColor")
	{
		index = command.indexOf("(", 0);
		indexOfSimbol = command.indexOf(")", index+ 1);
		numberOf = indexOfSimbol - index;
		methodText = command.mid(index+1, numberOf-1);
	
		QColor color(methodText);
		p.setPen(Qt::red);
		update();
	}
}

void graphics::paintEvent(QPaintEvent * /* event */)
{
 	QPainter painter(this );
//	painter.setFont(QFont("Arial", 30));
	painter.drawPixmap( 0 , 0 , pix ) ;
}

QString graphics::getMethodName(QString command)
{
	QString methodName;
	index = command.indexOf("(",0);
	methodName = command.left(index);
	return methodName;
}
