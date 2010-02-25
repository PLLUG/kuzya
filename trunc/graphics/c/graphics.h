#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#if !defined(__COLORS)
#define __COLORS
enum COLORS 
{
    BLACK,          /*dark colors */
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    LIGHTGRAY,
    DARKGRAY,       /*light colors */
    LIGHTBLUE,
    LIGHTGREEN,
    LIGHTCYAN,
    LIGHTRED,
    LIGHTMAGENTA,
    YELLOW,
    WHITE
};
#endif

enum PATERN
{
  EMPTY_FILL,      	///  0   Background color
  SOLID_FILL,      	///  1   Solid fill
  LINE_FILL,       	///  2   ---
  LTSLASH_FILL,    	///  3   ///
  SLASH_FILL,      	///  4   ///, thick lines
  BKSLASH_FILL,    	///  5   \\\, thick lines
  LTBKSLASH_FILL,  	///  6   \\\/
  HATCH_FILL,      	///  7   Light hatch
  XHATCH_FILL,     	///  8   Heavy crosshatch
  INTERLEAVE_FILL, 	///  9   Interleaving lines
  WIDE_DOT_FILL,   	/// 10   Widely spaced dots
  CLOSE_DOT_FILL,  	/// 11   Closely spaced dots
  USER_FILL,       	/// 12   User-defined fill pattern
};

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

namespace KGNS{
  FILE *kuzyaGraphFile;
  char  command[100];
  char text[100];
  COLORS curentColor;
  int isCOLOR = 2;        ///0 colors 1 int 2 nothing for color
  int curentIntColor;
  
  COLORS curentFillColor;
  int isFillCOLOR = 2;        ///0 colors 1 int 2 nothing for fill color
  int curentFillIntColor;

}

int width =  640;
int height = 480;

///*******************Declaration of the functions***************
void arc(int x, int y, int stAngel, int endAngle, int radius);
void bar(int x, int y, int x1, int y1);
void bar(int x, int y, int x1, int y1,COLORS color);
void bar(int x, int y, int x1, int y1, int color);
void bar3d(int x, int y, int x1, int y1, int z, bool top);
void circle(int x, int y, int radius);
void cleardevice();
void close();
void closegraph();
void drawpoly(int numOfPoints, int* array);
void drawpoly(int numOfPoints, int* arrayX, int* arrayY);
void drawpoly(int numOfPoints, int** arrayXY);
void ellipse(int x, int y, int rWidth, int rHeight);
int getmaxx();
int getmaxy();
void image(int x, int y, char* path);
void initgraph();
void initgraph(int drv, int drm, char *text);
void initgraph(int setWidth, int setHeight);
void pieslice(int x, int y, int stAngle, int endAngle, int radius);
void putpixel(int x, int y);
void putpixel(int x, int y, COLORS color);
void putpixel(int x, int y, int color);
void line(int x, int y, int x1, int y1);
void line(int x, int y, int x1, COLORS color);
void lineto(int x, int y);
void moveto(int x, int y);
void outtext(const char* text);
void outtextxy(int x, int y, const char* text);
void rectangle(int x, int y,int x1,int  y1);
void rectangle(int x, int y,int x1,int y1, COLORS color);
void rectangle(int x, int y,int x1,int y1, int color);
void save(const char* path);
void setbkcolor(COLORS color);
void setbkcolor(int color);
void setcolor(COLORS color);
void setcolor(int color);
void setfillcolor(int color);
void setfillcolor(COLORS color);
void setfillstyle(PATERN patern, COLORS color);
void setfillstyle(int patern, COLORS color);
void setfillstyle(PATERN patern, int color);
void setfillstyle(int patern, int color);
void setlinestyle(int linestyle, unsigned upattern, int thickness);
void settextstyle(char* font, int direction, int size);
///********************************************************************

//*********************************************************************
///***************arc********************************
void arc(int x, int y, int stAngel, int endAngle, int radius)
{
	sprintf(KGNS::command, "arc(%i,%i,%i,%i,%i);\n",x , y, stAngel*2, endAngle, radius);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}
///*****************Draws bar filled in with th help of fillcolor*****************
void bar(int x, int y, int x1, int y1)
{
 	sprintf(KGNS::command, "bar(%i,%i,%i,%i);\n",x , y, x1, y1);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
void bar(int x, int y, int x1, int y1,COLORS color)
{
 	sprintf(KGNS::command, "setfillcolor(%i);\n", color);  /// color to fill bar
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
  
	sprintf(KGNS::command, "bar(%i,%i,%i,%i);\n",x , y, x1, y1);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	if (0 == KGNS::isCOLOR)                             ///previously seted color to draw figurs
	{
	  setcolor(KGNS::curentColor);
	}
	else if (1 == KGNS::isCOLOR)
	{
	  setcolor(KGNS::curentIntColor);
	}
}
void bar(int x, int y, int x1, int y1, int color)
 {
	sprintf(KGNS::command, "setfillcolor(%i);\n", color);  /// color to fill bar
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
  
	sprintf(KGNS::command, "bar(%i,%i,%i,%i);\n",x , y, x1, y1);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	if (0 == KGNS::isCOLOR)                             ///previously seted color to draw figurs
	{
	  setcolor(KGNS::curentColor);
	}
	else if (1 == KGNS::isCOLOR)
	{
	  setcolor(KGNS::curentIntColor);
	}
}
//***************3 demention rectangle**********************************
void bar3d(int x, int y, int x1, int y1, int z, bool top)
{
	char* topStr;
	if(top == true)
	{
		bar(x, y, x1, y1);
	}
	else
	{
		rectangle(x, y, x1, y1);
	}
	sprintf(KGNS::command, "bar3d(%i,%i,%i,%i,%i);\n",x , y, x1, y1, z);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
//**********circle**************************************************** 
void circle(int x, int y, int radius)
{
	sprintf(KGNS::command, "circle(%i,%i,%i);\n",x , y, radius);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);		
}
//**********clearDevise********************************************
void cleardevice()
{
	sprintf(KGNS::command, "cleardevice();\n");
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
///**************this function close kuzyagraph window**************
void close()
{
  	sprintf(KGNS::command, "close();\n");
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
//*********closeGraph***********************
void closegraph()
{
	sprintf(KGNS::command, "closegraph();");
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	fflush(KGNS::kuzyaGraphFile);
	pclose(KGNS::kuzyaGraphFile);
}
///********draw polygon were 0 element is x and 1 is y and so on**********************
void drawpoly(int numOfPoints,int* array)
{
	char *command = new char [numOfPoints * sizeof(double) * 2 + 12 + sizeof(int)]; 
	char newText[20];
	
	sprintf(newText, "drawpoly(%i,", numOfPoints);
	strcat(command, newText);
	
	for (int i = 0; i <= numOfPoints - 3; i += 2)
	{
		sprintf(newText, "%i,", array[i]);
		strcat(command, newText);
		sprintf(newText, "%i,", array[i + 1]);
		strcat(command, newText);
	}
	sprintf(newText, "%i,", array[numOfPoints - 2]);
	strcat(command, newText);
	sprintf(newText, "%i", array[numOfPoints - 1]);
	strcat(command, newText);
	strcat(command, ");\n");
	fprintf(KGNS::kuzyaGraphFile, command);	
	
	//delete [] command;
}

///********draw polygon were 0 element is x and 1 is y and so on**********************
void drawpoly(int numOfPoints,int *arrayX,int* arrayY)
{
	char *command = new char [numOfPoints * sizeof(double) * 2 + 12 + sizeof(int)]; 
	char newText[20];
	
	sprintf(newText, "drawpoly(%i,", numOfPoints);
	strcat(command, newText);
	
	for (int i = 0; i <= numOfPoints - 2; ++i)
	{
		sprintf(newText, "%i,", arrayX[i]);
		strcat(command, newText);
		sprintf(newText, "%i,", arrayY[i]);
		strcat(command, newText);
	}
	sprintf(newText, "%i,", arrayX[numOfPoints - 1]);
	strcat(command, newText);
	sprintf(newText, "%i", arrayY[numOfPoints - 1]);
	strcat(command, newText);
	strcat(command, ");\n");
	fprintf(KGNS::kuzyaGraphFile, command);	
	
	//delete [] command;
}

///********draw polygon were 0 element is x and 1 is y and so on**********************
void drawpoly(int numOfPoints, int** arrayXY)
{
	char *command = new char [numOfPoints * sizeof(double) * 2 + 12 + sizeof(int)]; 
	char newText[20];
	
	sprintf(newText, "drawpoly(%i,", numOfPoints);
	strcat(command, newText);
	
	for (int i = 0; i <= numOfPoints - 2; ++i)
	{
		sprintf(newText, "%i,", arrayXY[0][i]);
		strcat(command, newText);
		sprintf(newText, "%i,", arrayXY[1][i]);
		strcat(command, newText);
	}
	sprintf(newText, "%i,", arrayXY[0][numOfPoints - 1]);
	strcat(command, newText);
	sprintf(newText, "%i", arrayXY[1][numOfPoints - 1]);
	strcat(command, newText);
	strcat(command, ");\n");
	fprintf(KGNS::kuzyaGraphFile, command);	
	
	//delete [] command;
}

///******ellipse************************************
void ellipse(int x, int y, int rWidth, int rHeight)
{
	sprintf(KGNS::command, "ellipse(%i,%i,%i,%i);\n",x , y, rWidth, rHeight);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
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
///********* draw image ******************************
void image(int x, int y, const char* path)
{
	sprintf(KGNS::command, "image(%i,%i,\"%s\");\n",x , y, path);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
//********initGraph*************************
void initgraph()
{
	static char *command= "kuzyagraph";
	KGNS::kuzyaGraphFile = popen(command, "w");

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
	sprintf(KGNS::command, "initgraph(%i,%i);\n",width, height);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
//*******line**************************************
void line(int x, int y, int x1, int y1)
{
	sprintf(KGNS::command, "line(%i,%i,%i,%i);\n",x , y,x1,y1);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
//**********line***********************************
void line(int x, int y, int x1, int y1, COLORS color)
{
	sprintf(KGNS::command, "setcolor(%i);\n", color); ///color to draw line
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	sprintf(KGNS::command, "line(%i,%i,%i,%i);\n",x , y,x1,y1);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	if (0 == KGNS::isCOLOR)
	{
	  setcolor(KGNS::curentColor);
	}
	else if (1 == KGNS::isCOLOR)
	{
	  setcolor(KGNS::curentIntColor);
	}
}

//**********line***********************************
void line(int x, int y, int x1, int y1, int color)
{
	sprintf(KGNS::command, "setcolor(%i);\n", color);  /// color to draw line
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
  
	sprintf(KGNS::command, "line(%i,%i,%i,%i);\n",x , y,x1,y1);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	if (0 == KGNS::isCOLOR)                             ///previously seted color to draw figurs
	{
	  setcolor(KGNS::curentColor);
	}
	else if (1 == KGNS::isCOLOR)
	{
	  setcolor(KGNS::curentIntColor);
	}
}
//*********lineto*************************************
void lineto(int x, int y)
{
  	sprintf(KGNS::command, "lineto(%i, %i);\n", x, y);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
//*********moveto*************************************
void moveto(int x, int y)
{
  	sprintf(KGNS::command, "moveto(%i,%i);\n",x , y);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
//********outTextXY***********************************
void outtextxy(int x, int y, const char* text)
{
	sprintf(KGNS::command, "outtextxy(%i,%i,\"%s\");\n",x , y, text);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
//************outText*****************************************
void outtext(const char* text)
{
	sprintf(KGNS::command, "outtext(\"%s\");\n", text);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}
//********piesLice*********************************
void pieslice(int x, int y, int stAngle, int endAngle, int radius)
{
	sprintf(KGNS::command, "pieslice(%i,%i,%i,%i,%i);\n",x , y, stAngle, endAngle, radius);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);		
}
//*******putPixel*********************************
void putpixel(int x, int y)
{	
	sprintf(KGNS::command, "putpixel(%i,%i);\n",x , y);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}
//*******putPixel___ With color*********************************
void putpixel(int x, int y, COLORS color)
{
	sprintf(KGNS::command, "setcolor(%i);\n", color);     ///color to draw dot
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
  
	sprintf(KGNS::command, "putpixel(%i,%i);\n",x , y);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	if (0 == KGNS::isCOLOR)				     ///previously seted color to draw figurs
	{
	  setcolor(KGNS::curentColor);
	}
	else if (1 == KGNS::isCOLOR)
	{
	  setcolor(KGNS::curentIntColor);
	}
}
//********putPixel_with_color_named_by_number******************
void putpixel(int x, int y, int color)
{
	sprintf(KGNS::command, "setcolor(%i);\n", color);      ///color to draw dot
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	sprintf(KGNS::command, "putpixel(%i,%i);\n",x , y);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	if (0 == KGNS::isCOLOR)				      ///previously seted color to draw figurs
	{
	  setcolor(KGNS::curentColor);
	}
	else if (1 == KGNS::isCOLOR)
	{
	  setcolor(KGNS::curentIntColor);
	}
}
///********rectangle***********************************
void rectangle(int x, int y,int x1,int y1)
{	
	sprintf(KGNS::command, "rectangle(%i,%i,%i,%i);\n",x , y, x1, y1);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
}

///********rectangle with setting color*******************************
void rectangle(int x, int y,int x1,int y1, COLORS color)
{	
	sprintf(KGNS::command, "setcolor(%i);\n", color);      ///color to draw dot
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	sprintf(KGNS::command, "rectangle(%i,%i,%i,%i);\n",x , y, x1, y1);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	if (0 == KGNS::isCOLOR)				      ///previously seted color to draw figurs
	{
	  setcolor(KGNS::curentColor);
	}
	else if (1 == KGNS::isCOLOR)
	{
	  setcolor(KGNS::curentIntColor);
	}
}

///********rectangle with setting int color*******************************
void rectangle(int x, int y,int x1,int y1, int color)
{	
	sprintf(KGNS::command, "setcolor(%i);\n", color);      ///color to draw dot
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	sprintf(KGNS::command, "rectangle(%i,%i,%i,%i);\n",x , y, x1, y1);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);
	
	
	if (0 == KGNS::isCOLOR)				      ///previously seted color to draw figurs
	{
	  setcolor(KGNS::curentColor);
	}
	else if (1 == KGNS::isCOLOR)
	{
	  setcolor(KGNS::curentIntColor);
	}
}
///*******************save image in the given path with given name*****
void save(const char* path)
{
  	sprintf(KGNS::command, "save(\"%s\");\n", path);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}
//***********setBGColor___COLORS************************************
void setbkcolor(COLORS color)
{
	sprintf(KGNS::command, "setbkcolor(%i);\n", color);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}
//***********setBGColor___NUMBERS************************************
void setbkcolor(int color)
{
	sprintf(KGNS::command, "setbkcolor(%i);\n", color);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}
//***********setColor___COLORS************************************
void setcolor(COLORS color)
{
	KGNS::curentColor = color;
	sprintf(KGNS::command, "setcolor(%i);\n", color);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
	KGNS::isCOLOR = 0;
}
//***********setColor___NUMBERS************************************
void setcolor(int color)
{
	KGNS::curentIntColor = color;
	sprintf(KGNS::command, "setcolor(%i);\n", color);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
	KGNS::isCOLOR = 1;
}
///***********setColor___COLORS************************************
void setfillcolor(COLORS color)
{
	KGNS::curentFillColor = color;
	sprintf(KGNS::command, "setfillcolor(%i);\n", color);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
	KGNS::isFillCOLOR = 0;
}
///****************set color to fill polygon*********************************
void setfillcolor(int color)
{
	KGNS::curentFillIntColor = color;
	sprintf(KGNS::command, "setfillcolor(%i);\n", color);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
	KGNS::isFillCOLOR = 1;
}
///**************set PATERN patern and COLORS color to fill some polygon*********
void setfillstyle(PATERN patern, COLORS color)
{
	setfillcolor(color);
	sprintf(KGNS::command, "setfillstyle(%i);\n", patern);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}
///**************set int patern and COLORS color to fill some polygon*********
void setfillstyle(int patern, COLORS color)
{
	setfillcolor(color);
	sprintf(KGNS::command, "setfillstyle(%i);\n", patern);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}
///**************set int PATERN patern and int color to fill some polygon*********
void setfillstyle(PATERN patern, int color)
{
	setfillcolor(color);
	sprintf(KGNS::command, "setfillstyle(%i);\n", patern);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}
///**************set int patern and int color to fill some polygon*********
void setfillstyle(int patern, int color)
{
	setfillcolor(color);
	sprintf(KGNS::command, "setfillstyle(%i);\n", patern);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}
//***********setLineStyle**********************************
void setlinestyle(int linestyle, unsigned upattern, int thickness)
{
	sprintf(KGNS::command, "setlinestyle(%i,%i);\n", linestyle, thickness);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}
//***********setTetxtStyle*******************************
void settextstyle(char *font, int direction, int size)
{
	sprintf(KGNS::command, "settextstyle(%s,%i,%i);\n", font, direction, size);
	fprintf(KGNS::kuzyaGraphFile, KGNS::command);	
}