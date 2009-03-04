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
#include <stdlib.h>
#include <stdio.h>

FILE *file;
char  command[100];
char text[100];

///********initGraph*************************
void initGraph()
{
	static char *command= "/mnt/fun/PlugRepo/graphics/bin/graphics";
	file = popen(command, "w");
}
///*********closeGraph***********************
void closeGraph()
{
	fflush(file);
	char* command = "/mnt/fun/PlugRepo/graphics/bin/graphics";
	pclose(file);
}
///*******putPixel*********************************
void putPixel(int x, int y)
{	
	sprintf(command, "point(%i,%i);\n",x , y);
	fprintf(file, command);
}
///*******line**************************************
void line(int x, int y, int x1, int y1)
{
	sprintf(command, "line(%i,%i,%i,%i);\n",x , y,x1,y1);
	fprintf(file, command);
}
///******ellipse************************************
void ellipse(int x, int y, int x1, int y1)
{
	sprintf(command, "ellipse(%i,%i,%i,%i);\n",x , y,x1,y1);
	fprintf(file, command);

}
///********rectangle***********************************
void rectangle(int x, int y,int x1,int y1)
{	
	sprintf(command, "rectangle(%i,%i,%i,%i);\n",x , y, x1, y1);
	fprintf(file, command);
}
///********outTextXY***********************************
void outTextXY(int x, int y,char *text)
{
	sprintf(command, "point(%i,%i,\"%s\");\n",x , y, text);
	fprintf(file, command);
}
