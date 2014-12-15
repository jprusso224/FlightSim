/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/* File for "Terrain" lesson of the OpenGL tutorial on
 * www.videotutorialsrock.com
 */



#include <iostream>
#include <stdlib.h>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "vec3f.h"
#include "terrain.h"
#include "CSCIx229.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

using namespace std;

int fov=70;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=8;     //  Size of world
float sc = 10;

double th = 0.0;
double ph = 0.0;
double ps = 0.0;
// Set initial eye coordinates for fpv
double Ex = 0;
double Ey = 0.75;
double Ez = 0;

double Lx = 0;
double Ly = 0;
double Lz = 0;

double Ux = 0;
double Uy = 1;
double Uz = 0;
// inertial velocities
double dx = 0;
double dy = 0;
double dz = 0;

// body velocities
double du = 0;
double dv = 0;
double dw = 0;

// body angular velocities
double dph = 0;
double dth = 0;
double dps = 0;



Terrain* _terrain;
unsigned int water;
unsigned int ground;

void cleanup() {
	delete _terrain;
}



/*void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}*/

//void handleResize(int w, int h) {
	//glViewport(0, 0, w, h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
//}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
	 glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -10.0f);
   
   // glTranslatef(Ex,Ey,Ez);
    //glRotated(th,0,0,1);
    //glRotated(ps,0,1,0);
   // glRotated(ph,1,0,0);

	// Do the eye stuff
    gluLookAt(Ex,Ey,Ez ,Lx,Ly,Lz, Ux,Uy,Uz);
	
	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	
	float scale = sc / max(_terrain->width() - 1, _terrain->length() - 1);
	glScalef(scale, scale, scale);
	glTranslatef(-(float)(_terrain->width() - 1) / 2,
				 0.0f,
				 -(float)(_terrain->length() - 1) / 2);
	
	glEnable(GL_CULL_FACE);
	
	glColor3f(0.9f, 0.9f, 0.9f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,ground);
	for(int z = 0; z < _terrain->length() - 1; z++) {
		//Makes OpenGL draw a triangle at every three consecutive vertices
		glBegin(GL_TRIANGLE_STRIP);
		for(int x = 0; x < _terrain->width(); x++) {
			Vec3f normal = _terrain->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
			glTexCoord2f(x/64.,z/64.); glVertex3f(x, _terrain->getHeight(x, z), z);
			normal = _terrain->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
			glTexCoord2f(x/64.,(z+1)/64.); glVertex3f(x, _terrain->getHeight(x, z + 1), z + 1);
		}
		glEnd();
	}
	glDisable(GL_CULL_FACE);
	

	glBindTexture(GL_TEXTURE_2D,water);
	glColor3f(0.1,0.5,0.4);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0); glVertex3f(0,0,0);
	glTexCoord2f(_terrain->width()/64.,0); glVertex3f(_terrain->width(),0,0);
	glTexCoord2f(_terrain->width()/64.,_terrain->length()/64.); glVertex3f(_terrain->width(),0,_terrain->length());
	glTexCoord2f(0,_terrain->length()/64.); glVertex3f(0,0,_terrain->length());
	glEnd();
	glDisable(GL_TEXTURE_2D);
	
	 //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   SDL_GL_SwapBuffers();
}


void flight(){
	
	// Adjust euler angles
	th = th + dth;  // yaw
	ph = ph + dph;  // pitch
	ps = ps + dps;  // roll
	
	// Transform to inertial velocity
	dx = Cos(th)*Cos(ps)*du;
	dy = (Cos(ph)*Sin(th)  -  Cos(th)*Sin(ph)*Sin(ps))*du;
	dz = -(Sin(ph)*Sin(th)  +  Cos(ph)*Cos(th)*Sin(ps))*du; 
	
	// No errors with pitch (th)!!!
	
	// Yaw and Roll having issues.
	
	// Look at (independent of roll angle ph)
	Lx = Ex + Cos(ps)*Cos(th); 
    Ly = Ey + (-Sin(ph)*Sin(ps) + Sin(th)*Cos(ph)*Cos(ps));
    Lz = Ez - (Cos(ph)*Sin(ps) + Sin(ph)*Sin(th)*Cos(ps)); 
    
    // Up vector (independent of yaw angle ps)
   	Ux =  Cos(ps)*Sin(th); // Not this
    Uy = (Cos(ps)*Sin(ph) + Sin(th)*Cos(ph)*Sin(ps)); // Not this
	Uz = (Cos(ph)*Cos(ps) - Sin(ph)*Sin(th)*Sin(ps)); // This is the cause!!!
	
	// Adjust camera position;
	Ex = Ex + dx;   // For some reason the coordinates are funky
	Ey = Ey + dy;
	Ez = Ez + dz;
	
}

int keyDown(){
   Uint8* keys = SDL_GetKeyState(NULL);
//int shift = SDL_GetModState()&KMOD_SHIFT;
   //  Exit on ESC
   if (keys[SDLK_ESCAPE]){
      cleanup();
     return 0;
  }
     // Check keys
     
   // roll(ph)
   if (keys[SDLK_l])
     dph-=0.7;// dth = dth-0.1;
   else if (keys[SDLK_j])
     dph+=0.7;//dth = dth+0.1;
   // pitch (theta) 
   if (keys[SDLK_i])
     dth-=0.7;//dph = dph-0.1;
   else if (keys[SDLK_k])
    dth+=0.7;//  dph = dph+0.1;
   // yaw(psi)
   if (keys[SDLK_a])
     dps+=0.2;// dps = dps+0.1;
   else if (keys[SDLK_d])
      dps-=0.2;//dps = dps-0.1;
      
   if (keys[SDLK_w])
	  du = du + 0.01;
   else if (keys[SDLK_s])
	  du = du - 0.01;
	  
	// Test
   else if (keys[SDLK_t])
	  Ex = Ex + 1;
   else if (keys[SDLK_f])
	  Ey = Ey + 1;
  else if (keys[SDLK_g])
	  Ez = Ez + 1;
	  
  return 1;
	
}

void correctAngles(){
	
		if (th < 0)
			th = 360 + th;
		if (ph < 0)
			ph = 360 + ph;
		if (ps < 0)
			ps = 360 + ps;
	    if (th >= 360)
			th = 0;
		if (ph >= 360)
			ph = 0;
		if (ps >= 360)
			ps = 0;
}

int keyUp(SDL_Event event){
  
                   if(event.key.keysym.sym == SDLK_i)
                        dth = 0;
                   if(event.key.keysym.sym == SDLK_k)
                        dth = 0;
                        
                   if(event.key.keysym.sym == SDLK_j)
                        dph = 0;
                        
                   if(event.key.keysym.sym == SDLK_l)
                        dph = 0;
                       
                  
                
                return 1;
}

/*
 *  Call this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(fov,asp,dim);
}

int main(int argc, char** argv) {
	
	int run=1;
   double t0=0;
   SDL_Surface* screen;

   //  Initialize SDL
   SDL_Init(SDL_INIT_VIDEO);
   //  Set size, resizable and double buffering
   screen = SDL_SetVideoMode(600,600,0,SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF);
   if (!screen ) Fatal("Cannot set SDL video mode\n");
   //  Set window and icon labels
   SDL_WM_SetCaption("CSCI4229 Project","sdl20");
   //  Set screen size
   reshape(screen->w,screen->h);

   //  Load textures
   	_terrain = loadTerrain("test5.bmp", 20);
	water = LoadTexBMP("water.bmp");
	ground = LoadTexBMP("ground1.bmp");
	
   //  SDL event loop
   ErrCheck("init");
   while (run)
   {
	  
      //  Elapsed time in seconds
      double t = SDL_GetTicks()/1000.0;
      //  Process all pending events
      SDL_Event event;
      while (SDL_PollEvent(&event))
         switch (event.type)
         {
            case SDL_VIDEORESIZE:
               screen = SDL_SetVideoMode(event.resize.w,event.resize.h,0,SDL_OPENGL|SDL_RESIZABLE|SDL_DOUBLEBUF);
               reshape(screen->w,screen->h);
               break;
            case SDL_QUIT:
               run = 0;
               break;
            case SDL_KEYDOWN:
               run = keyDown();
               t0 = t+0.5;  // Wait 1/2 s before repeating
               break;
           case SDL_KEYUP:
              run = keyUp(event);
            default:
               //  Do nothing
         
               break;
         }
      correctAngles();
      //  Repeat key every 50 ms
      if (t-t0>0.05)
      {
        // run = key();
         t0 = t;
      }
      //  Display
      //Th = fmod(90*t,360.0);
      flight();
      display();
      //  Slow down display rate to about 100 fps by sleeping 5ms
      //SDL_Delay(5);
   }
   //  Shut down SDL
   SDL_Quit();
   return 0;
}









