#include<windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <glut.h>
#include <stdlib.h>
#include<iostream>
#include<conio.h>
#include<math.h>

#include "edk.h"
#include "edkErrorCode.h"
#include "EmoStateDLL.h"

#pragma comment(lib, "../lib/edk.lib")
#pragma comment(lib, "../lib/glut32.lib")

#define PI 3.1418
//#define GLUT_USE_SGI_OPENGL

bool oneTime = true;//This variable is used to show the message of waiting 8 seconds in the program
bool outOfBound = false;
float currX = 0,currY = 0;
double maxRadius = 10000;
float xmax = 0, ymax = 0;
float preX = 0, preY = 0;
int incOrDec = 10;//No entiendo para que es esta variable
float oldXVal =0, oldYVal = 0;
unsigned long pass = 0,globalElapsed = 0;
int count = 0;
float x;

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}
/*
Draw the big circle
*/
void drawCircle( float Radius, int numPoints )
{
  glBegin( GL_LINE_STRIP );
    for( int i=0; i<numPoints; i++ )
    {
		float Angle = i * (2.0*PI/numPoints); // use 360 instead of 2.0*PI if
		float X = cos( Angle )*Radius;  // you use d_cos and d_sin
		float Y = sin( Angle )*Radius;
		glVertex2f( X, Y );
	}
  glEnd();
}
/*
Draw the medium circle
*/
void drawFilledCircle(float radius)
{
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(radius);
	//glPoint(0, 0, 0);
}
/*Initialize the display with the values*/
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();

   glColor3f(1.0,1.0,1.0);
   drawCircle(800,100);
   glColor3f(0.0,0.0,1.0);
   drawCircle(maxRadius-4000,800);
   glColor3f(0.0,1.0,1.0);
   drawCircle(maxRadius,1000);


   glColor3f(1.0, 0.0, 0.0);
   glRectf(currX-400.0, currY-400.0, currX+400.0, currY+400.0);

   glPopMatrix();
   glutSwapBuffers();
}

void changeXY(int x) // x = 0 : idle
{
	if( currX >0 )
	{
		float temp = currY/currX;
		currX -= incOrDec;
		currY = temp*currX;
	}
	else if( currX < 0)
	{
		float temp = currY/currX;
		currX += incOrDec;
		currY = temp*currX;
	}
	else
	{
		if( currY > 0 ) currY -= incOrDec;
		else if( currY <0 ) currY += incOrDec;
	}
	if( x == 0)
	{
		if( (abs(currX) <= incOrDec) && (abs(currY) <= incOrDec))//abs() returns the absolute value of an integer
		{
			xmax = 0;
			ymax = 0;
		}
		else
		{
			xmax = currX;
			ymax = currY;
		}
	}
	else
	{
		if( (abs(currX) <= incOrDec) && (abs(currY) <= incOrDec))
		{
			xmax = 0;
			ymax = 0;
		}
	}
}

/*
This part with the function GyroDelta obtains the value of the displacement on the x axis and the y axis
EE_HeadsetGyroDelta(Id of the user, Displacement in X, Displacement in Y);

*/
void updateDisplay(void)
{
   int gyroX = 0,gyroY = 0;
   EE_HeadsetGetGyroDelta(0,&gyroX,&gyroY);
   xmax += gyroX;//The displacement in x is asigned to the variable xmax
   ymax += gyroY;//The displacement in y is asigned to the variable ymax

   if( outOfBound )
   {
	   if( preX != gyroX && preY != gyroY )//Si los valores que se obtienen de la diadema son menores al radio entonces
	   {
		   xmax = currX;
		   ymax = currY;
	   }
   }

   double val = sqrt((float)(xmax*xmax + ymax*ymax));//val es igual a la hipotenusa generadaa por el cateto xmaxx y ymax que seria los puntos donde se encuentra el giroscopio

    std::cout <<"xmax : " << xmax <<" ; ymax : " << ymax << std::endl;//Imprime los valores de la posicion en donde esta el punto rojo desde una referencia 0

/*Si la distancia de la hipotenusa generada por x y y es mayor al radio maximo enviamos true a outOfBound y generamos que
x y y cambien de valor para que se queden en el circulo exterior y no lo sobrepasen.
*/
   if( val >= maxRadius )
   {
	   changeXY(1);
	   outOfBound = true;
	   preX = gyroX;
	   preY = gyroY;
   }
   else
   {
	   outOfBound = false;
		if(oldXVal == gyroX && oldYVal == gyroY)
		{
			++count;
			if( count > 10 )
			{
				changeXY(0);
			}
		}
		else
		{
			count = 0;
			currX = xmax;
			currY = ymax;
			oldXVal = gyroX;
			oldYVal = gyroY;
		}
   }
   Sleep(15);//Wait the execution of the program por 15 segundos
   glutPostRedisplay();//Le hace un refresh a la pantalla
}
/*
Ithink that this function is the one that change all the values if the windows change the size
*/
void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50000.0, 50000.0, -50000.0, 50000.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
void mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(updateDisplay);
         break;
      case GLUT_MIDDLE_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
         break;
      default:
         break;
   }
}
/*
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main(int argc, char** argv)
{

   EmoEngineEventHandle hEvent = EE_EmoEngineEventCreate();
   EmoStateHandle eState = EE_EmoStateCreate();
   unsigned int userID = -1;
   EE_EngineConnect  ();


int i = ES_GetHeadsetOn  ( eState );
    std::cout<< i << "\n";


   if(oneTime)
   {
      printf("Start after 8 seconds\n");
	  Sleep(8000);
	  oneTime = false;
   }

   globalElapsed = GetTickCount();

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (650, 650);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutIdleFunc(updateDisplay);
   glutMainLoop();

   EE_EngineDisconnect();
   EE_EmoStateFree(eState);
   EE_EmoEngineEventFree(hEvent);

   return 0;
}
