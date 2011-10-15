#include <graphics.h>
#include <math.h>

void drawLeftBranch(int x, int y, int length)
{
	line(x, y, x - length*3.14/180*45, y + length*3.14/180*45);
}

void drawRightBranch(int x, int y, int length)
{
	line(x, y, x + length*3.14/180*45, y + length*3.14/180*45);
}

void fractal(int x, int y, int length)
{
	if (length < 2) {
		return;
	}
	drawLeftBranch(x, y, length);
	drawRightBranch(x, y, length);
	fractal(x - (int)length*3.14/180*45, y + (int)length*3.14/180*45, length/2);

	fractal(x + (int)length*3.14/180*45, y + (int)length*3.14/180*45, length/2);
}

int main ()
{
	bool leftSide = false;
	int x = getmaxx()/2;
	int y = 0;
	int branchLength = 200;
	initgraph();
	
	fractal(x, y, branchLength);
	
	closegraph();
	return 0;
}