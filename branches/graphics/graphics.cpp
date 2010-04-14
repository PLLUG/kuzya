/***************************************************************************
 *   Copyright (C) 2009 by Volodymyr Shevchyk                              *
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


#include <QtGui>
#include <QTextStream>
#include <QPainterPath>
#include <QPoint>
#include <math.h>
#include <QDebug>
#include <QShortcut>

#include "graphics.h"

#define PI (3.141592653589793)

graphics::graphics(QWidget *parent)
        : QMainWindow(parent)
{
	ui.setupUi(this);
        this->setWindowTitle("GraphicCore");
	setObjectName("graphics");

        width = 640;
        height = 480;

        ui.centralwidget->setMinimumHeight(height);
        ui.centralwidget->setMinimumWidth(width);

	curentColor = 0;
        curentFillColor = 0;
	textSize = 12;
	textFont = "Arial";
	textDirection = 0;
        fillPatern = 1;
	lineThickness = 1;
	lineStyle = 0;
        curentBGColor = 15;

        pix = QPixmap(width, height);
        pixBG = QPixmap(width, height);
        pixBG.fill(Qt::white);
        pix.fill(Qt::transparent);
        pix.alphaChannel();

        p.begin(&pix);
        fillBrush = new QBrush();

        setLineStyle(lineStyle);
        setFillPatern(1);
        setCurentFillColor(0);
        setCurentColor(curentColor);
        setCurentBGColor(curentBGColor);

	resize(width, height);

        rsi = new ReadStdIn(this);
        new QShortcut(Qt::Key_Return, this, SLOT(close()));
	connect(rsi, SIGNAL(commandAppeared(QString)), this, SLOT(processCommand(QString)));
        rsi->start();

        //connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(processCommand()));
}

graphics::~graphics()
{
}

void graphics::processCommand(QString  command)
{
        ///***************all kuzygraphfunctions are heree************************
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

                this->resize(width, height);
                this->setMinimumHeight(height);
                this->setMinimumWidth(width);
                this->setMaximumHeight(height);
                this->setMaximumWidth(width);
		createPixmap(width, height);
                creatBGPixmap(width, height);
                return;
	}
        //setCurentFillColor(curentFillColor);
        //setFillPatern(fillPatern);
        //pen.setBrush(fillBrush);
        //setCurentColor(curentColor);
        else if(getMethodName(command) == "arc")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
                x1 = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
                y1 = command.mid(index+1, numberOf-1).toInt(0,10);

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

                p.drawArc(x1-r, y1-r, 2*r, 2*r, stAngle * 16, endAngle * 16);

	}
        else if (getMethodName(command) == "bar")
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

                QBrush myBrush;
                myBrush.setColor(fillBrush->color());
                myBrush.setStyle(fillBrush->style());

                p.fillRect(x, y, x1-x, y1-y, myBrush);

	}
        else if (getMethodName(command) == "bar3d")
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
                indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
                z = command.mid(index+1, numberOf-1).toInt(0,10);


                p.drawRect(x, y, x1-x, y1-y);
                int a, c;
                c = z;
                a = (int) fabs(c / sqrt(2.0)); //Modified to avoid warning: passing `double' for converting 1 of `int abs(int)'
                p.drawLine(x, y, x+a, y-a);
                p.drawLine(x+a, y-a, x1+a, y-a);
                p.drawLine(x1+a, y-a, x1+a, y1-a);
                p.drawLine(x1, y, x1+a, y-a);
                p.drawLine(x1, y1, x1+a, y1-a);

	}
        else if(getMethodName(command) == "circle")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
                x1 = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
                y1 = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		r = command.mid(index+1, numberOf-1).toInt(0,10);

                p.drawEllipse(x1-r, y1-r, 2*r, 2*r);

	}
        else if(getMethodName(command) == "cleardevice")
	{
                pix.fill(Qt::transparent);
	}
        else if (getMethodName(command) == "close")
        {
            p.end();
            close();
        }
        else if(getMethodName(command) == "closegraph")
        {
                p.end();
        }
        else if(getMethodName(command) == "drawfunc")
        {
            index = command.indexOf("(",0);
            indexOfSimbol = command.indexOf(",", index);
            numberOf = indexOfSimbol - index;
            numOfPoints = command.mid(index+1, numberOf-1).toInt(0,10);

            //qDebug() << command;//QVariant(numOfPoints).toString();

            double **arrayXY = new double *[2];
            for (int i = 0; i < 2; ++i )
                arrayXY[i] = new double [numOfPoints];

            for (int i = 0; i <= numOfPoints - 2; ++i )
            {
                index = indexOfSimbol;
                indexOfSimbol = command.indexOf(",", index+1);
                numberOf = indexOfSimbol - index;
                arrayXY[0][i] = command.mid(index+1, numberOf-1).toDouble();

                index = indexOfSimbol;
                indexOfSimbol = command.indexOf(",", index+1);
                numberOf = indexOfSimbol - index;
                arrayXY[1][i] = command.mid(index+1, numberOf-1).toDouble();

            }

            index = indexOfSimbol;
            indexOfSimbol = command.indexOf(",", index+1);
            numberOf = indexOfSimbol - index;
            arrayXY[0][numOfPoints - 1] = command.mid(index+1, numberOf-1).toDouble();

            index = indexOfSimbol;
            indexOfSimbol = command.indexOf(")", index+1);
            numberOf = indexOfSimbol - index;
            arrayXY[1][numOfPoints - 1] = command.mid(index+1, numberOf-1).toDouble();

                   drawFunc(numOfPoints, arrayXY);
            update();
        }
        else if(getMethodName(command) == "drawpoly")
	{
                index = command.indexOf("(",0);
                indexOfSimbol = command.indexOf(",", index);
                numberOf = indexOfSimbol - index;
                numOfPoints = command.mid(index+1, numberOf-1).toInt(0,10);

                QPolygon polygon;

                double **arrayXY = new double *[2];
                for (int i = 0; i < 2; ++i )
                    arrayXY[i] = new double [numOfPoints];

                for (int i = 0; i <= numOfPoints / 2 - 2; ++i )
                {
                    index = indexOfSimbol;
                    indexOfSimbol = command.indexOf(",", index+1);
                    numberOf = indexOfSimbol - index;
                    arrayXY[0][i] = command.mid(index+1, numberOf-1).toDouble();


                    index = indexOfSimbol;
                    indexOfSimbol = command.indexOf(",", index+1);
                    numberOf = indexOfSimbol - index;
                    arrayXY[1][i] = command.mid(index+1, numberOf-1).toDouble();

                    polygon << QPoint(arrayXY[0][i], arrayXY[1][i]);
                 }

                 index = indexOfSimbol;
                 indexOfSimbol = command.indexOf(",", index+1);
                 numberOf = indexOfSimbol - index;
                 arrayXY[0][numOfPoints - 1] = command.mid(index+1, numberOf-1).toDouble();

                 index = indexOfSimbol;
                 indexOfSimbol = command.indexOf(")", index+1);
                 numberOf = indexOfSimbol - index;
                 arrayXY[1][numOfPoints - 1] = command.mid(index+1, numberOf-1).toDouble();
                
                 polygon << QPoint(arrayXY[0][numOfPoints - 1], arrayXY[1][numOfPoints - 1]);

                 p.drawPolyline(polygon);
	}
        else if(getMethodName(command) == "ellipse")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
                x1 = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
                y1 = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
                int eWidth = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
                int eHeight = command.mid(index+1, numberOf-1).toInt(0,10);

                p.drawEllipse(x1 - eWidth, y1 - eHeight, 2*eWidth, 2*eHeight);
	}
        else if(getMethodName(command) == "fillellipse")
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

                QPainterPath myPath;
		myPath.addEllipse(x-x1, y-y1, 2*x1, 2*y1);
                QBrush brush;
                brush.setColor(fillBrush->color());
                brush.setStyle(fillBrush->style());
                p.fillPath(myPath, brush);
	} 
        else if (getMethodName(command) == "image")
        {
                index = command.indexOf("(",0);
                indexOfSimbol = command.indexOf(",", index);
                numberOf = indexOfSimbol - index;
                x1 = command.mid(index+1, numberOf-1).toInt(0,10);

                index = indexOfSimbol;
                indexOfSimbol = command.indexOf(",", index+1);
                numberOf = indexOfSimbol - index;
                y1 = command.mid(index+1, numberOf-1).toInt(0,10);

                index = command.indexOf("\"", 0);
                indexOfSimbol = command.indexOf("\"", index+ 1);
                numberOf = indexOfSimbol - index;
                methodText = command.mid(index+1, numberOf-1);

                QImage image(methodText);
                p.drawImage(x1, y1, image);
        }
        else if(getMethodName(command) == "line")
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
	}
        else if(getMethodName(command) == "lineto")
        {
                index = command.indexOf("(",0);
                indexOfSimbol = command.indexOf(",", index);
                numberOf = indexOfSimbol - index;
                x = command.mid(index+1, numberOf-1).toInt(0,10);

                index = indexOfSimbol;
                indexOfSimbol = command.indexOf(")", index+1);
                numberOf = indexOfSimbol - index;
                y = command.mid(index+1, numberOf-1).toInt(0,10);

                p.drawLine(x1, y1, x, y);

                x1 = x;
                y1 = y;
        }
        else  if(getMethodName(command) == "moveto")
        {
                index = command.indexOf("(",0);
                indexOfSimbol = command.indexOf(",", index);
                numberOf = indexOfSimbol - index;
                x1 = command.mid(index+1, numberOf-1).toInt(0,10);

                index = indexOfSimbol;
                indexOfSimbol = command.indexOf(")", index+1);
                numberOf = indexOfSimbol - index;
                y1 = command.mid(index+1, numberOf-1).toInt(0,10);
        }
        else if (getMethodName(command) == "outtext")
	{
		index = command.indexOf("\"", 0);
		indexOfSimbol = command.indexOf("\"", index+ 1);
		numberOf = indexOfSimbol - index;
		methodText = command.mid(index+1, numberOf-1);

                p.drawText(x1, y1, methodText);
	}
        else if (getMethodName(command) == "outtextxy")
	{
		QString oneSimbol;
 		p.setFont(QFont(textFont, textSize));

		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
                x1 = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
                y1 = command.mid(index+1, numberOf-1).toInt(0,10);

		index = command.indexOf("\"", 0);
		indexOfSimbol = command.indexOf("\"", index+ 1);
		numberOf = indexOfSimbol - index;
		methodText = command.mid(index+1, numberOf-1);

		switch(textDirection)
		{
                        case 0: p.drawText(x1, y1, methodText); break;
			case 1:
				for(int i = 0; i <= methodText.length(); i++)
				{
					oneSimbol = methodText.mid(i, 1);
                                        p.drawText(x1, y1, oneSimbol);
                                        y1 += textSize;
				}
                }
	}
        else if(getMethodName(command) == "pieslice")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
                x1 = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(",", index+1);
		numberOf = indexOfSimbol - index;
                y1 = command.mid(index+1, numberOf-1).toInt(0,10);

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


                QPainterPath myPath;
                //myPath.arcTo(x1-r, y1-r, 2*r, 2*r, stAngle, endAngle);
                //int sweepLenght = (PI * r * (endAngle - stAngle)) / 180;
                QPointF center(x1, y1);
                myPath.moveTo(center);
                //myPath.arcTo(150, 150, 100, 100, 0, 300);
                myPath.arcTo(x1 - r, y1 - r, 2 * r, 2 * r, stAngle, endAngle);
                QBrush brush;
                brush.setColor(fillBrush->color());
                brush.setStyle(fillBrush->style());
                p.fillPath(myPath, brush);
	}
        else if(getMethodName(command) == "putpixel")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
                x1 = command.mid(index+1, numberOf-1).toInt(0,10);
		
		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index);
		numberOf = indexOfSimbol - index;
                y1 = command.mid(index+1, numberOf-1).toInt(0,10);

                p.drawPoint(x1, y1);
	}
        else if (getMethodName(command) == "rectangle")
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
        }
        else if (getMethodName(command) == "save")
        {/*
            QByteArray bytes;
            QBuffer buffer(&bytes);
            buffer.open(QIODevice::ReadWrite);
            pix.save(&buffer, "PNG");


            bufferPix.loadFromData(bytes);
           */

            QPixmap bufferPix = QPixmap(width, height);
            QPainter myPainter;
            myPainter.begin(&bufferPix);
            myPainter.drawPixmap(0, 0, pixBG);
            myPainter.drawPixmap(0, 0, pix);

            index = command.indexOf("\"", 0);
            indexOfSimbol = command.indexOf("\"", index+ 1);
            numberOf = indexOfSimbol - index;
            methodText = command.mid(index+1, numberOf-1);

            bufferPix.save(methodText, "PNG", 100);
            myPainter.end();
        }
        else if (getMethodName(command) == "setbkcolor")
        {
                index = command.indexOf("(", 0);
                indexOfSimbol = command.indexOf(")", index+ 1);
                numberOf = indexOfSimbol - index;
                curentBGColor = command.mid(index+1, numberOf-1).toInt(0,10);
/*
                while (15 <= curentBGColor)
                {
                    curentBGColor -= 16;
                }
                while (0 >= curentBGColor)
                {
                    curentColor -= 16;
                }
*/
                setCurentBGColor(curentBGColor);
                BGColorWasChanged = true;
        }
        else if (getMethodName(command) == "setcolor")
	{
		index = command.indexOf("(", 0);
		indexOfSimbol = command.indexOf(")", index+ 1);
		numberOf = indexOfSimbol - index;
		curentColor = command.mid(index+1, numberOf-1).toInt(0,10);

                setCurentColor(curentColor);
        }
        else if (getMethodName(command) == "setfillcolor")
        {
                index = command.indexOf("(", 0);
                indexOfSimbol = command.indexOf(")", index+ 1);
                numberOf = indexOfSimbol - index;
                curentFillColor = command.mid(index+1, numberOf-1).toInt(0,10);

                setCurentFillColor(curentFillColor);
        }
        else if (getMethodName(command) == "setfillstyle")
        {
                index = command.indexOf("(", 0);
                indexOfSimbol = command.indexOf(")", index+ 1);
                numberOf = indexOfSimbol - index;
                fillPatern = command.mid(index+1, numberOf-1).toInt(0,10);

                setFillPatern(fillPatern);
        }
        else if (getMethodName(command) == "setlinestyle")
	{
		index = command.indexOf("(",0);
		indexOfSimbol = command.indexOf(",", index);
		numberOf = indexOfSimbol - index;
		lineStyle = command.mid(index+1, numberOf-1).toInt(0,10);

		index = indexOfSimbol;
		indexOfSimbol = command.indexOf(")", index+1);
		numberOf = indexOfSimbol - index;
		lineThickness= command.mid(index+1, numberOf-1).toInt(0,10);

                pen.setWidth(lineThickness);
                setLineStyle(lineStyle);
	}
        else if (getMethodName(command) == "settextstyle")
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
        update();
}

void graphics::paintEvent(QPaintEvent * /*event*/)
{
        QPainter painter(this);
        if (BGColorWasChanged)
            painter.drawPixmap(0, 0, pixBG);
        else
            BGColorWasChanged = false;
        painter.drawPixmap( 0 , 0 , pix);
}

void graphics::createPixmap(int width, int height)
{
        p.end();
        pix = QPixmap(width, height);
        pix.fill(Qt::transparent);
        p.begin(&pix);
}

void graphics::creatBGPixmap(int width, int height)
{
        pixBG = QPixmap(width, height);
        pixBG.fill(Qt::white);
        BGColorWasChanged = true;
}
QString graphics::getMethodName(QString command)
{
	QString methodName;
	index = command.indexOf("(",0);
	methodName = command.left(index);
	return methodName;
}

///**************set current BG color***************************************************
void graphics::setCurentBGColor(int curentBGColor)
{

                switch(curentBGColor)
                {
                        case 0: pixBG.fill(Qt::black); 				update();break;
                        case 1: pixBG.fill(Qt::darkBlue);                       update();break;
                        case 2: pixBG.fill(Qt::darkGreen);                      update();break;
                        case 3: pixBG.fill(Qt::darkCyan);       		update();break;
                        case 4: pixBG.fill(Qt::darkRed);			update();break;
                        case 5: pixBG.fill(QColor(139, 0, 139));        	update();break;
                        case 6: pixBG.fill(QColor(165, 42, 42));	 	update();break;
                        case 7: pixBG.fill(Qt::gray);	 			update();break;
                        case 8: pixBG.fill(Qt::darkGray);			update();break;
                        case 9: pixBG.fill(Qt::blue);    			update();break;
                        case 10:pixBG.fill(Qt::green);  			update();break;
                        case 11:pixBG.fill(Qt::cyan);   			update();break;
                        case 12:pixBG.fill(Qt::red); 	 			update();break;
                        case 13:pixBG.fill(QColor(255, 0, 255));        	update();break;
                        case 14:pixBG.fill(Qt::yellow);	 			update();break;
                        case 15:pixBG.fill(Qt::white);   			update();break;
                }
}


void graphics::drawFunc(int numOfPoints, double **arrayXY)
{
    const int l = 50;
    double minX;
    double minY;
    double maxX;
    double maxY;
    double multiplierX;
    double multiplierY;
    double mirrorX;
    double mirrorY;
    int stepX;
    int stepY;
    int step;
    double abscissa;
    double ordinate;

/******************************************************/

    minX = arrayXY[0][0];
    minY = arrayXY[1][0];

    maxX = arrayXY[0][numOfPoints - 1];
    maxY = arrayXY[1][0];

/************************FindMaxAndMin********************/

    for (int i = 0; i < numOfPoints ; i++)
    {
        if  (maxY < arrayXY[1][i])
        {
            maxY = arrayXY[1][i];
        }
        if  (minY > arrayXY[1][i])
        {
            minY = arrayXY[1][i];
        }
    }

/**********************FindMultiplierAndMirror*************************/

    if ((maxX - minX) !=0)
    {
        multiplierX = (width - 2.0 * l) / (maxX - minX);
    }
    if ((minY - maxY) != 0)
    {
        multiplierY = (height - 2.0 * l) / (minY - maxY);
    }
    if ((minX - maxX) != 0)
    {
        mirrorX = (width * minX - l * (minX + maxX)) / (minX - maxX);
    }
    if ((minY - maxY) != 0)
    {
        mirrorY = (height * maxY - l * (minY + maxY)) / (maxY - minY);
    }

/***********************FindStep***************************/

    stepX = ((width + .0) - 2.0 * (l + .0)) / grid;
    stepY = ((height + .0) - 2.0 * (l + .0)) / grid;

    if (stepX < stepY)
    {
        step = stepX;
    }

    if (stepX > stepY)
    {
        step = stepY;
    }

/******************FindAbscissAndOrdinate***************/

    if ((minX * maxX) <= 0)
    {
        abscissa = 0;
    }
    if ((minX * maxX) > 0)
    {
        abscissa = minX;
    }
    if (((minX * maxX) > 0) && (minX < 0))
    {
        abscissa = maxX;
    }
    if ((minY * maxY) <= 0)
    {
        ordinate = 0;
    }
    if (((minY * maxY) > 0) && (minY > 0))
    {
        ordinate = minY;
    }
    if (((minY * maxY) > 0) && (minY < 0))
    {
        ordinate = maxY;
    }

    pen.setStyle(Qt::DotLine);
    p.setPen(pen);
    pen.setColor(Qt::magenta);
    p.setPen(pen);
    int OX = 0;
    int OY = 0;
    int i = 0;
    int Y = 0;
    int X = 0;
/**********************OX**************************/

    OX = ceil(multiplierY * ordinate + mirrorY);

    while ((OX - i * step) >= l)
    {
        Y = OX - i * step;
        p.drawLine(l, Y, ceil((double)(width - l)), Y);
        i++;
    }
    i = 0;
    while ((OX + i * step) <= (height - l))
    {
        Y = OX + i * step;
        p.drawLine(l, Y, ceil((double)(width - l)), Y);
        i++;
    }
/***********************OY****************************/
    i = 0;
    OY = ceil(multiplierX * abscissa + mirrorX);

    while ((OY - i * step) >= l)
    {
        X = OY - i * step;
        p.drawLine(X ,l, X, ceil((double)(height - l)));
        i++;
    }
    i = 0;
    while ((OY + i * step) <= (width - l))
    {
        X = OY + i * step;
        p.drawLine(X ,l, X, ceil((double)(height - l)));
        i++;
    }

/**************************SeroundedFikked************************************/

    p.fillRect(0, 0, width, l, Qt::lightGray);
    p.fillRect(width - l, 0, l, height, Qt::lightGray);
    p.fillRect(0, height - l, width, l,  Qt::lightGray);
    p.fillRect(0, 0, l, height, Qt::lightGray);

/****************************DrawTextOY***********************************/
    int sum = 0.0;
    QString textSum = "";
    OX = ceil(multiplierY * ordinate + mirrorY);
    i = 0;
    while ((OX - i * step) >= l)
    {
        sum = (-i * step) / multiplierY;
        textSum = QVariant(sum).toString();
        Y = OX - i * step;
        p.drawText(l - textSum.length() * 9, Y + 3, textSum);
        i++;
    }

    i = 0;
    while ((OX + i * step) <= (height - l))
    {
        sum = (i * step) / multiplierY;
        textSum = QVariant(sum).toString();
        Y = OX + i * step;
        p.drawText(l - textSum.length() * 9, Y + 3, textSum);
        i++;
    }
/****************************DrawTextOX**************************/
    int h = 0;
    i = 0;
    OY = ceil(multiplierX * abscissa + mirrorX);

    while ((OY - i * step) >= l)
    {
        if (i % 2 == 0) h = 10;
        sum = (-i * step) / multiplierX;
        textSum = QVariant(sum).toString();
        X = OY - i * step;
        p.drawText(X - textSum.length() * 4,l - h - 3, textSum);
        i++;
        h = 0;
    }

    i = 0;
    while ((OY + i * step) <= (width - l))
    {
        if (i % 2 == 0) h = 10;
        sum = (i * step) / multiplierX;
        textSum = QVariant(sum).toString();
        X = OY + i * step;
        p.drawText(X - textSum.length() * 4,l - h - 3, textSum);
        i++;
        h = 0;
    }
/****************************DrawOrdinateAndAbsciss*********************************/


    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(Qt::black);
    p.setPen(pen);


    p.drawLine(l, ceil(multiplierY * ordinate + mirrorY), ceil((double)(width - l)), ceil(multiplierY * ordinate + mirrorY));
    p.drawLine(ceil(multiplierX * abscissa + mirrorX),l, ceil(multiplierX * abscissa + mirrorX), ceil((double)(height - l)));

/**************************DrawFunction********************************/
    setCurentColor(curentColor);
    for (int i = 0; i < numOfPoints - 1; ++i)
    {
        qDebug() << QVariant(arrayXY[1][i]).toString();
        p.drawLine(ceil(multiplierX * arrayXY[0][i] + mirrorX), ceil(multiplierY * arrayXY[1][i] + mirrorY),
                   ceil(multiplierX * arrayXY[0][i+1] + mirrorX), ceil(multiplierY * arrayXY[1][i+1] + mirrorY));
    }

}
///*******************seting color**********************************************
void graphics::setCurentColor(int curentColor)
{
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
}
///*******************seting fill color**********************************************
void graphics::setCurentFillColor(int curentFillColor)
{
    switch(curentFillColor)
                {
                        case 0: fillBrush->setColor(Qt::black);                  update();break;
                        case 1: fillBrush->setColor(Qt::darkBlue);		update();break;
                        case 2: fillBrush->setColor(Qt::darkGreen);		update();break;
                        case 3: fillBrush->setColor(Qt::darkCyan);		update();break;
                        case 4: fillBrush->setColor(Qt::darkRed);                update();break;
                        case 5: fillBrush->setColor(QColor(139, 0, 139));        update();break;
                        case 6: fillBrush->setColor(QColor(165, 42, 42)); 	update();break;
                        case 7: fillBrush->setColor(Qt::gray);           	update();break;
                        case 8: fillBrush->setColor(Qt::darkGray);		update();break;
                        case 9: fillBrush->setColor(Qt::blue);   		update();break;
                        case 10:fillBrush->setColor(Qt::green);  		update();break;
                        case 11:fillBrush->setColor(Qt::cyan);   		update();break;
                        case 12:fillBrush->setColor(Qt::red);    		update();break;
                        case 13:fillBrush->setColor(QColor(255, 0, 255));	update();break;
                        case 14:fillBrush->setColor(Qt::yellow);         	update();break;
                        case 15:fillBrush->setColor(Qt::white);  		update();break;
                }
}
///**********************seting fill patern***********************************************
void graphics::setFillPatern(int patern)
{
    switch(patern)
                {
                        case 0: fillBrush->setStyle(Qt::NoBrush);               update();break;
                        case 1: fillBrush->setStyle(Qt::SolidPattern);		update();break;
                        case 2: fillBrush->setStyle(Qt::HorPattern);		update();break;
                        case 3: fillBrush->setStyle(Qt::BDiagPattern);		update();break;
                        case 4: fillBrush->setStyle(Qt::BDiagPattern);          update();break;
                        case 5: fillBrush->setStyle(Qt::FDiagPattern);          update();break;
                        case 6: fillBrush->setStyle(Qt::FDiagPattern);  	update();break;
                        case 7: fillBrush->setStyle(Qt::Dense5Pattern);        	update();break;
                        case 8: fillBrush->setStyle(Qt::Dense4Pattern);		update();break;
                        case 9: fillBrush->setStyle(Qt::VerPattern);   		update();break;
                        case 10:fillBrush->setStyle(Qt::Dense1Pattern); 	update();break;
                        case 11:fillBrush->setStyle(Qt::Dense2Pattern); 	update();break;
                        case 12:fillBrush->setStyle(Qt::CrossPattern);  	update();break;
                        case 13:fillBrush->setStyle(Qt::LinearGradientPattern);	update();break;
                        case 14:fillBrush->setStyle(Qt::RadialGradientPattern); update();break;
                        case 15:fillBrush->setStyle(Qt::ConicalGradientPattern);update();break;
                }

}
///*********************************************************************
void graphics::setLineStyle(int lineStyle)
{
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
}
