#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

FILE *file;
char  command[100];
char text[100];
int width ;
int height;

#if !defined(__COLORS)
#define __COLORS
enum COLORS 
{
    BLACK,          /* dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,       /* light colors */
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};
#endif

enum DIRECTION
{
	HORIZ_DIR	,	/*left to right */
	VERT_DIR		/*bottom to top*/  
};

enum LINESTYLE
{
	SOLID_LINE,
	DOTTED_LINE,
	CENTER_LINE,
	DASHED_LINE,
	USERBIT_LINE
};
///*******************Declaration of the functions***************
void arc(int x, int y, int stAngel, int endAngle, int radius);
void bar(int x, int y,int x1,int  y1);
void bar3d(int x, int y, int x1, int y1, int z, bool top);
void circle(int x, int y, int radius);
void cleardevice();
void closegraph();
void ellipse(int x, int y, int rWidth, int rHeight);
int getmaxx();
int getmaxy();
void initgraph();
void initgraph(int setWidth, int setHeight);
void pieslice(int x, int y, int stAngle, int endAngle, int radius);
void putpixel(int x, int y);
void line(int x, int y, int x1, int y1);
void lineto(int x, int y);
void moveto(int x, int y);
void outtext(char *text);
void outtextxy(int x, int y, char *text);
void setcolor(COLORS color);
void setlinestyle(int linestyle, unsigned upattern, int thickness);
void settextstyle(char* font, int direction, int size);
///********************************************************************

//*********************************************************************
///***************arc********************************
void arc(int x, int y, int stAngel, int endAngle, int radius)
{
	sprintf(command, "arc(%i,%i,%i,%i,%i);\n",x , y, stAngel*2, endAngle, radius);
	fprintf(file, command);	
}
//********rectangle***********************************
void bar(int x, int y,int x1,int y1)
{	
	sprintf(command, "bar(%i,%i,%i,%i);\n",x , y, x1, y1);
	fprintf(file, command);
}
//***************3 demention rectangle**********************************
void bar3d(int x, int y, int x1, int y1, int z, bool top)
{
	char* topStr;
	if(top == true)
	{
		topStr = "true";
	}
	else
	{
		topStr = "false";
	}
	sprintf(command, "bar3D(%i,%i,%i,%i,%i,%s);\n",x , y, x1, y1, z, topStr);
	fprintf(file, command);
}
//**********circle**************************************************** 
void circle(int x, int y, int radius)
{
	sprintf(command, "circle(%i,%i,%i);\n",x , y, radius);
	fprintf(file, command);		
}
//**********clearDevise********************************************
void cleardevice()
{
	sprintf(command, "clearDevice();\n");
	fprintf(file, command);
}
//*********closeGraph***********************
void closegraph()
{
	sprintf(command, "closegraph();");
	fprintf(file, command);
	fflush(file);
	char* command = "/usr/share/kuzya/bin/kuzyagraph";
	pclose(file);
}
///******ellipse************************************
void ellipse(int x, int y, int rWidth, int rHeight)
{
	sprintf(command, "ellipse(%i,%i,%i,%i);\n",x , y, rWidth, rHeight);
	fprintf(file, command);
}
//*************getMaxX****************************
int getmaxx()
{
	return width;
}
//*************getMaxY****************************
int getmaxy()
{
	return height;
}
//********initGraph*************************
void initgraph()
{
	static char *command= "/usr/share/kuzya/bin/kuzyagraph";
	file = popen(command, "w");
}
//*********initGraph******************************
void initgraph(int setWidth, int setHeight)
{
	initgraph();
	width = setWidth;
	height = setHeight;
	sprintf(command, "initgraph(%i,%i);\n",width, height);
	fprintf(file, command);
	width = 600;
	height = 400;
}
//********piesLice*********************************
void pieslice(int x, int y, int stAngle, int endAngle, int radius)
{
	sprintf(command, "pieslice(%i,%i,%i,%i,%i);\n",x , y, stAngle, endAngle, radius);
	fprintf(file, command);		
}
//*******putPixel*********************************
void putpixel(int x, int y)
{	
	sprintf(command, "putPixel(%i,%i);\n",x , y);
	fprintf(file, command);
}
//*******line**************************************
void line(int x, int y, int x1, int y1)
{
	sprintf(command, "line(%i,%i,%i,%i);\n",x , y,x1,y1);
	fprintf(file, command);
}
//*********lineto*************************************
void lineto(int x, int y)
{
  	sprintf(command, "lineto(%i, %i);\n", x, y);
	fprintf(file, command);
}
//*********moveto*************************************
void moveto(int x, int y)
{
  	sprintf(command, "moveto(%i,%i);\n",x , y);
	fprintf(file, command);
}
//********outTextXY***********************************
void outtextxy(int x, int y, char *text)
{
	sprintf(command, "outTextXY(%i,%i,\"%s\");\n",x , y, text);
	fprintf(file, command);
}
//************outText*****************************************
void outtext(char *text)
{
	sprintf(command, "outText(\"%s\");\n", text);
	fprintf(file, command);	
}
//***********setColor************************************
void setcolor(COLORS color)
{
	sprintf(command, "setColor(%i);\n", color);
	fprintf(file, command);	
}
//***********setLineStyle**********************************
void setlinestyle(int linestyle, unsigned upattern, int thickness)
{
	sprintf(command, "setLineStyle(%i,%i);\n", linestyle, thickness);
	fprintf(file, command);	
}
//***********setTetxtStyle*******************************
void settextstyle(char *font, int direction, int size)
{
	sprintf(command, "setTextStyle(%s,%i,%i);\n", font, direction, size);
	fprintf(file, command);	
}