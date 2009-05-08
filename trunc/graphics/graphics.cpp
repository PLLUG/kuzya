/***************************************************************************
 *   Copyright (C) 2009 by Volodymyr Shevchyk                              *
 *   volderne@gmail.com                                                    *
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
#include <QPainterPath>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_painter.h>
#include <QPoint>
#include <math.h>


#include "readstdin.h"
#include "graphics.h"

graphics::graphics(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setObjectName("graphics");

	width = 600;
	height = 400;

	curentColor = 0;
	textSize = 12;
	textFont = "Arial";
	textDirection = 0;
	lineThickness = 1;
	lineStyle = 0;

	pix = QPixmap(width, height);
	pix.fill(Qt::white);

	resize(width, height);

	ReadStdIn* rsi = new ReadStdIn(this);
	connect(rsi, SIGNAL(commandAppeared(QString)), this, SLOT(processCommand(QString)));
	rsi->start();
}

graphics::~graphics()
{
}

void graphics::processCommand(QString  command)
{
	if(getMethodName(command) == "initgraph")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
		width = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		height = command.mid(index+1, numberOf-1).toInt(0,10);

		createPixmap(width, height);
	}
	QPainterPath myPath;
	
	p.begin(&pix);
	pen.setWidth(lineThickness);
	switch(curentColor)
		{
			case 0: pen.setColor(Qt::black); 
					p.setPen(pen); 				update();break;
			case 1: pen.setColor(Qt::darkBlue); 
					p.setPen(pen);				update();break;
			case 2: pen.setColor(Qt::darkGreen); 
					p.setPen(pen);				update();break;
			case 3: pen.setColor(Qt::darkCyan); 
					p.setPen(pen);				update();break;
			case 4: pen.setColor(Qt::darkRed);
					p.setPen(pen);				update();break;
			case 5: pen.setColor(QColor(139, 0, 139)); 
					p.setPen(pen);				update();break;
			case 6: pen.setColor(QColor(165, 42, 42));
					p.setPen(pen);			 	update();break;
			case 7: pen.setColor(Qt::gray);
					p.setPen(pen);	 			update();break;
			case 8: pen.setColor(Qt::darkGray); 
					p.setPen(pen);				update();break;
			case 9: pen.setColor(Qt::blue); 
					p.setPen(pen);				update();break;
			case 10:pen.setColor(Qt::green); 
					p.setPen(pen);				update();break;
			case 11:pen.setColor(Qt::cyan);
					p.setPen(pen);	 			update();break;
			case 12:pen.setColor(Qt::red);
					p.setPen(pen);	 			update();break;
			case 13:pen.setColor(QColor(255, 0, 255));
					p.setPen(pen);				update();break;
			case 14:pen.setColor(Qt::yellow);
					p.setPen(pen);	 			update();break;
			case 15:pen.setColor(Qt::white);
					p.setPen(pen);	 			update();break;
		}
	switch(lineStyle)
	{
			case 0: pen.setStyle(Qt::SolidLine); 
					p.setPen(pen); 				update();break;
			case 1: pen.setStyle(Qt::DotLine); 
					p.setPen(pen);				update();break;
			case 2: pen.setStyle(Qt::DashDotLine); 
					p.setPen(pen);				update();break;
			case 3: pen.setStyle(Qt::DashLine); 
					p.setPen(pen);				update();break;
			case 4: pen.setStyle(Qt::CustomDashLine);
					p.setPen(pen);				update();break;
	}
	if(getMethodName(command) == "arc")
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
		stAngle = command.mid(index+1, numberOf-1).toInt(0,10) / 2;

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		endAngle = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		r = command.mid(index+1, numberOf-1).toInt(0,10);

		p.drawArc(x-r, y-r, 2*r, 2*r, stAngle, endAngle);
    	update();
	}
	if (getMethodName(command) == "bar")
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
	if (getMethodName(command) == "bar3D")
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
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		y1 = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		z = command.mid(index+1, numberOf-1).toInt(0,10);	

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		methodText = command.mid(index+1, numberOf-1);
	
		p.drawRect(x, y, x1-x, y1-y);
		if(methodText == "true")
		{
			int a, c;
			c = z;
			a = abs(c / sqrt(2));
 			p.drawLine(x, y, x+a, y-a); 
			p.drawLine(x+a, y-a, x1+a, y-a);
			p.drawLine(x1+a, y-a, x1+a, y1-a);
			p.drawLine(x1, y, x1+a, y-a);
			p.drawLine(x1, y1, x1+a, y1-a);
		}
		update();
	}
	if(getMethodName(command) == "circle")
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
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		r = command.mid(index+1, numberOf-1).toInt(0,10);

		p.drawEllipse(x-r, y-r, 2*r, 2*r);
		update();
	}
	if(getMethodName(command) == "clearDevice")
	{
		pix.fill(Qt::transparent);
	}
	if(getMethodName(command) == "drawpoly")
	{
/*		int pX, pY;
		if(step == 1)
		{

			index = command.indexOf("(",0);
			indexOfSimbol = command.indexOf(",", index+1);
			numberOf = indexOfSimbol - index;
			x1 = command.mid(index+1, numberOf-1).toInt(0,10);

			index = indexOfSimbol;
			indexOfSimbol = command.indexOf(")", index+1);
			numberOf = indexOfSimbol - index;
			y1 = command.mid(index+1, numberOf-1).toInt(0,10);
			
			pX = x1;
			pY = y1;
			myPath.moveTo(x1, y1);
			ui.label->setText(command);
		}
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		x = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		y = command.mid(index+1, numberOf-1).toInt(0,10);

		
        polygon[step - 1] = QPointF(x, y);
		myPath.moveTo(pX, pY);
		myPath.lineTo(x, y);
	
		pX = x;
		pY = y;
		step += 1;	

		if(step > numOfPoints)
		{
			myPath.addPolygon(polygon);
			ui.label->setText(command);
			p.drawPath(myPath);
			update();
		}
*/	}
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

		p.drawEllipse(x-x1, y-y1, 2*x1, 2*y1);
		update();
	}
	if(getMethodName(command) == "fillellipse")
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

		myPath.addEllipse(x-x1, y-y1, 2*x1, 2*y1);
		p.drawPath(myPath);
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
	if (getMethodName(command) == "outText")
	{
		index = command.indexOf("\"", 0);
		indexOfSimbol = command.indexOf("\"", index+ 1);
		numberOf = indexOfSimbol - index;
		methodText = command.mid(index+1, numberOf-1);

		p.drawText(x, y, methodText);
		update();
	}
	if (getMethodName(command) == "outTextXY")
	{
		QString oneSimbol;
 		p.setFont(QFont(textFont, textSize));

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

		switch(textDirection)
		{
			case 0: p.drawText(x, y, methodText); break;
			case 1:
				for(int i = 0; i <= methodText.length(); i++)
				{
					oneSimbol = methodText.mid(i, 1);
					p.drawText(x, y, oneSimbol);
					y += textSize;
				}
		}

// 		p.drawText(x, y, methodText);
		update();
	}
	if(getMethodName(command) == "pieslice")
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
		stAngle = command.mid(index+1, numberOf-1).toInt(0,10) / 2;

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		endAngle = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		r = command.mid(index+1, numberOf-1).toInt(0,10);

		p.drawPie(x-r, y-r, 2*r, 2*r, stAngle, endAngle);
    	update();
	}
	if(getMethodName(command) == "putPixel")
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
	if (getMethodName(command) == "setColor")
	{
		index = command.indexOf("(", 0);
		indexOfSimbol = command.indexOf(")", index+ 1);
		numberOf = indexOfSimbol - index;
		curentColor = command.mid(index+1, numberOf-1).toInt(0,10);
	}
	if (getMethodName(command) == "setLineStyle")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
		lineStyle = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		lineThickness= command.mid(index+1, numberOf-1).toInt(0,10);
	}
	if (getMethodName(command) == "setTextStyle")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
		textFont = command.mid(index+1, numberOf-1);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
		textDirection = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index);
		numberOf = indexOfSimbol - index;
		textSize = command.mid(index+1, numberOf-1).toInt(0,10);
	}
	p.end();
}

void graphics::paintEvent(QPaintEvent * /*event*/ )
{
 	QPainter painter(this );
	painter.drawPixmap( 0 , 0 , pix ) ;
	update();
}

void graphics::createPixmap(int width, int height)
{
	resize(width, height);
	pix = QPixmap(width, height);
	pix.fill(Qt::white);
}

QString graphics::getMethodName(QString command)
{
	QString methodName;
	index = command.indexOf("(",0);
	methodName = command.left(index);
	return methodName;
}
