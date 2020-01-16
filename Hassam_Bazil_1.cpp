/*********
CTIS164 - HOMEWORK
----------
STUDENT : Hassam Bazil
SECTION : 02
HOMEWORK: 01
STUDENT NO : 21701658
----------
PROBLEMS: If your program does not function correctly,
explain here which parts are not running.
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WW  800
#define WH 600

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


/* Global Variables */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
int winpos = 0, tx = -400, ty = 125, stopwatch = 0, stopsec = 0;
bool activator = false;

typedef struct { //Structure
	int tx, ty;
	bool fwd = true;

}truck_t;

truck_t truck[5]; //Structure Array

void truck_values()
{
	int k;

	for (k = 0; k < 5; k++)
	{
		truck[k].tx = tx;
		truck[k].ty = ty - (100 * k);
	}

}
//
						  // to draw circle, center at (x,y)
						  // radius r
						  //
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
// To display onto window using OpenGL commands
//

void truck_r(int tx, int ty,int i) // Reverse Truck 
{

	glColor3ub(100, 30, 0);
	glRectf(tx, ty, tx - 90, ty + 50);
	glColor3ub(100, 100, 0);
	circle(tx - 119, ty + 10, 3);
	glColor3ub(100, 50, 0);
	glBegin(GL_POLYGON);
	glVertex2f(tx - 90, ty + 22);
	glVertex2f(tx - 115, ty + 22);
	glVertex2f(tx - 120, ty + 12);
	glVertex2f(tx - 120, ty);
	glVertex2f(tx - 90, ty);
	glEnd();
	glColor3ub(65, 0, 10);
	glRectf(tx - 12, ty + 15, tx - 80, ty + 38);

	glColor3ub(80, 0, 10);
	glBegin(GL_TRIANGLES);
	glVertex2f(tx - 90, ty + 50);
	glVertex2f(tx - 110, ty + 60);
	glVertex2f(tx - 90, ty + 28);
	glEnd();
	glColor3ub(0, 0, 0);
	circle(tx - 25, ty, 10);
	circle(tx - 90, ty, 10);
	glColor3ub(100, 150, 0);
	vprint(tx - 57, ty + 22, GLUT_BITMAP_9_BY_15, "#%d", i+1);

}
void truck_display(int tx, int ty, int i)
{
	
	if (truck[i].fwd == false)
	{
		truck_r(tx + 125, ty, i);
	}
	else {
		glColor3ub(100, 30, 0);
		glRectf(tx, ty, tx + 90, ty + 50);
		glColor3ub(100, 100, 0);
		circle(tx + 119, ty+10, 3);
		glColor3ub(100, 50, 0);
		glBegin(GL_POLYGON);
		glVertex2f(tx + 90, ty + 22);
		glVertex2f(tx + 115, ty + 22);
		glVertex2f(tx + 120, ty + 12);
		glVertex2f(tx + 120, ty);
		glVertex2f(tx + 90, ty);
		glEnd();
		glColor3ub(65, 0, 10);
		glRectf(tx + 12, ty + 15, tx + 80, ty + 38);

		glColor3ub(80, 0, 10);
		glBegin(GL_TRIANGLES);
		glVertex2f(tx + 90, ty + 50);
		glVertex2f(tx + 110, ty + 60);
		glVertex2f(tx + 90, ty + 28);
		glEnd();
		glColor3ub(0, 0, 0);
		circle(tx + 25, ty, 10);
		circle(tx + 90, ty, 10);
		glColor3ub(100, 150, 0);
		vprint(tx + 37, ty + 22, GLUT_BITMAP_9_BY_15, "#%d", i+1);
	}
	

}


int winner() { //finding the winner

	for(int i = 4; i >= 0; i--)
	{
		if ((truck[winpos].tx < truck[i].tx) && truck[i].fwd == true)
		{
			winpos = i;
		}
		if ((truck[winpos].tx  > truck[i].tx) && truck[i].fwd == false)
		{
			winpos = i;
		}
	}
	return(winpos);
}
void displaybase() // Displaying the background
{
	int i;
	glColor3ub(50, 0, 0);
	glRectf(-400, 200, 400, 300);
	glColor3ub(70, 50, 0);
	glRectf(-200, 280, 200, 240);
	glColor3ub(10, 0, 0);
	vprint(-140, 253, GLUT_BITMAP_TIMES_ROMAN_24, "Truck Racer by Hassam Bazil");

	glColor3ub(70, 50, 0);
	vprint(-125, 205, GLUT_BITMAP_9_BY_15, "Press F1 to restart the game.");
	vprint(-155, 225, GLUT_BITMAP_9_BY_15, "Press spacebar to start/stop the game.");
	glColor3ub(250, 100, 0);

	for (i = 1; i >= -2; i--) {

		glBegin(GL_LINES);
		glVertex2f(-400, 100 * i);
		glVertex2f(400, 100 * i);
		glEnd();
		}

	vprint(-340, 272, GLUT_BITMAP_HELVETICA_18, "Timer"); 
	vprint(300, 270, GLUT_BITMAP_HELVETICA_18, "Winner");
	vprint(-350, 245, GLUT_BITMAP_HELVETICA_18, "00.%02d.%02d", stopsec, stopwatch); //Displaying Stop Watch
	}


void display() {
	int i;
	
	glClearColor(0.19, 0.19, 0.19, 0); // Giving color to the track 
	glClear(GL_COLOR_BUFFER_BIT);

	displaybase();
	for (i = 0; i < 5; i++)
	{
		truck_display(truck[i].tx, truck[i].ty, i);
		
	}
	int win=winner();
	glColor3f(1, 1, 1);
	if (stopwatch != 0)
		vprint(320, 240,GLUT_BITMAP_HELVETICA_18,"%d",win + 1);

	glutSwapBuffers();

}


//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == 32) // start the game when 'spacebar' is pressed.
		activator = !activator;
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	if (key == GLUT_KEY_F1)
	{
		for (int i = 0; i < 5; i++)
		{
			truck[i].tx = -400;
			truck[i].fwd = true;
		}
		stopwatch = stopsec = 0;  
		activator = false; 
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (activator) // if the value is true
	{
		stopwatch++; 
		if (stopwatch % 100 == 0)
		{
			stopsec++;
			stopwatch = 0;
		}

		for (int i = 0; i < 5; i++)
		{
			if (truck[i].tx + 125 < WW / 2)
			{
				if (truck[i].fwd == true)
					truck[i].tx = truck[i].tx + rand() % 5;
			}
			else 
				truck[i].fwd = false;

		}


		if (truck[0].tx > -WW / 2 && truck[1].tx  > -WW / 2 &&  truck[2].tx  > -WW / 2  && truck[3].tx  > -WW / 2  && truck[4].tx  > -WW / 2)
				{
					for (int i = 0; i < 5; i++)
							if (truck[i].tx  > -WW / 2)
							{
								if (truck[i].fwd == false)
								
										truck[i].tx = truck[i].tx - rand() % 5;
								
								}
						}
		if (truck[0].tx <= -400 || truck[1].tx <= -400 || truck[2].tx <= -400 || truck[3].tx <= -400 || truck[4].tx <= -400) // stopping the timer when the first bus comes back to the start and stop
		{
			for (int i = 0; i < 5; i++)
			{
				if (truck[i].fwd == false)
					activator = false;
			}

		}
		

	}


	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {


	truck_values();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WW, WH);
	glutCreateWindow("Bazilater Racer");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif
	srand(time(NULL));
	Init();

	glutMainLoop();
}