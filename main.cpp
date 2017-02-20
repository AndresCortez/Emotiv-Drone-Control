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

#include <iostream>
#include <fstream>

#pragma comment(lib, "../lib/edk.lib")
#pragma comment(lib, "../lib/glut32.lib")

#define PI 3.1418


bool oneTime = true;
bool outOfBound = false;
float currX = 0,currY = 0;
double maxRadius = 10000;
float xmax = 0, ymax = 0;
float preX = 0, preY = 0;
int incOrDec = 10;
float oldXVal =0, oldYVal = 0;
unsigned long pass = 0,globalElapsed = 0;
int count = 0;
float x;

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

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

void drawFilledCircle(float radius)
{
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(radius);
	//glPoint(0, 0, 0); 
}

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
		if( (abs(currX) <= incOrDec) && (abs(currY) <= incOrDec))
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


void updateDisplay(void)
{	  
   int gyroX = 0,gyroY = 0;
   EE_HeadsetGetGyroDelta(0,&gyroX,&gyroY);
   xmax += gyroX;
   ymax += gyroY;

   if( outOfBound )
   {
	   if( preX != gyroX && preY != gyroY )
	   {
		   xmax = currX;
		   ymax = currY;
	   }
   }

   double val = sqrt((float)(xmax*xmax + ymax*ymax));
  
    std::cout <<"xmax : " << xmax <<" ; ymax : " << ymax << std::endl;

   
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

   //////////////////////////////////Part to add to the original program ///////////////////////////////////
   std::ofstream fs("cvdrone-master/build/vs2015/nombre.txt");// Crea un archivo de texto
   if (currX > 800) {
	   printf("Mover hacia la derecha");
	   fs << "R" << std::endl;
	   fs.close();
   }
   else if (currY >800) {
	   printf("Mover hacia arriba");
	   fs << "F" << std::endl;
   }
   else if (currX < -800) {
	   printf("Mover hacia la izquierda");
	   fs << "L" << std::endl;
   }
   else if (currY < -800) {
	   printf("Mover hacia atras");
	   fs << "B" << std::endl;
   }
   else {
	   printf("No hacer nada");
	   fs << "S" << std::endl;
   }
   fs.close();
   /////////////////////////////////////////////////////////////////////////////

   Sleep(100);//era 15 antes
   glutPostRedisplay(); 
}
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
   EE_EngineConnect();	
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