#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

FILE *file;
char  command[100];
char text[100];
int width =  640 ;
int height = 480;

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
void initgraph(int drv, int drm, char *text);
void initgraph(int setWidth, int setHeight);
void pieslice(int x, int y, int stAngle, int endAngle, int radius);
void putpixel(int x, int y);
void putpixel(int x, int y, COLORS color);
void line(int x, int y, int x1, int y1);
void lineto(int x, int y);
void moveto(int x, int y);
void outtext(char *text);
void outtextxy(int x, int y, char *text);
void setbkcolor(COLORS color);
void setbkcolor(int color);
void setcolor(COLORS color);
void setcolor(int color);
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
	sprintf(command, "bar3d(%i,%i,%i,%i,%i,%s);\n",x , y, x1, y1, z, topStr);
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
	sprintf(command, "cleardevice();\n");
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
//********initGraph*************************
void initgraph(int *drv, int *drm, char *text)
{
	initgraph();
}
//*********initGraph******************************
void initgraph(int setWidth, int setHeight)
{
	initgraph();
	width = setWidth;
	height = setHeight;
	sprintf(command, "initgraph(%i,%i);\n",width, height);
	fprintf(file, command);
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
	sprintf(command, "putpixel(%i,%i);\n",x , y);
	fprintf(file, command);
}
//*******putPixel___ With color*********************************
void putpixel(int x, int y, COLORS color)
{
	setcolor(color);
	sprintf(command, "putpixel(%i,%i);\n",x , y);
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
	sprintf(command, "outtextxy(%i,%i,\"%s\");\n",x , y, text);
	fprintf(file, command);
}
//************outText*****************************************
void outtext(char *text)
{
	sprintf(command, "outtext(\"%s\");\n", text);
	fprintf(file, command);	
}
//***********setBGColor___COLORS************************************
void setbkcolor(COLORS color)
{
	sprintf(command, "setbgcolor(%i);\n", color);
	fprintf(file, command);	
}
//***********setBGColor___NUMBERS************************************
void setbkcolor(int color)
{
	sprintf(command, "setbgcolor(%i);\n", color);
	fprintf(file, command);	
}
//***********setColor___COLORS************************************
void setcolor(COLORS color)
{
	sprintf(command, "setcolor(%i);\n", color);
	fprintf(file, command);	
}
//***********setColor___NUMBERS************************************
void setcolor(int color)
{
	sprintf(command, "setcolor(%i);\n", color);
	fprintf(file, command);	
}
//***********setLineStyle**********************************
void setlinestyle(int linestyle, unsigned upattern, int thickness)
{
	sprintf(command, "setlinestyle(%i,%i);\n", linestyle, thickness);
	fprintf(file, command);	
}
//***********setTetxtStyle*******************************
void settextstyle(char *font, int direction, int size)
{
	sprintf(command, "settextstyle(%s,%i,%i);\n", font, direction, size);
	fprintf(file, command);	
}