/*
 * main.cpp
 *
 *  Created on: 26-Jan-2015
 *      Author: duhber
 */

#include "objloader.h"

#include<cmath>
/** function definition **/
void init();
void display();
void reshape(int w, int h);
void rotate();
void processSpecialKeys(int, int , int);
void processMouse(int, int , int ,int);
void processNormalKeys(unsigned char key, int x, int y);

void unProject(int x, int y);
void Project();

void capture_frame(unsigned int);
/** ---------------- ****/

/* global variables*/
unsigned int model;// object to render
static GLfloat spin=0.0;
char *filename;
char *dirname;
char *modelno;
objloader obj;
int mx=0,my=0;
GLdouble posX=0.0, posY=0.0, posZ=0.0;

float angle=0.0;
float lx=0.0f,lz=-1.0f;
float x=0.0f,z=15.0f;

bool recording=true;
unsigned int framenum=0;
unsigned char *pRGB;

int SCREEN_WIDTH=600;
int SCREEN_HEIGHT=400;
/** ***************** **/
int main(int argc, char **argv){
	if(argc !=3){
		cout<<"usage: <filename> <imgno> \n";
		return 1;
	}
    filename=argv[1];
    modelno=argv[2];
    /**********************************************/
    /* Initialize glut */
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH|GLUT_MULTISAMPLE);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(600,400);
    glutCreateWindow("Perspective's GLUT Template");

    init();// initialize openGL

    /* register glut call backs */

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(processSpecialKeys);
    glutMouseFunc(processMouse);
    glutKeyboardFunc(processNormalKeys);
    glutIdleFunc(rotate);

    glutMainLoop();

    return 0;

}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_NORMALIZE);

    gluLookAt(	x, 1.0f, z,
    			x+lx, 1.0f,  z+lz,
    			0.0f, 1.0f,  0.0f);

    /* Opengl light */
    GLfloat pos0[]={0.0,100.0,0.0,0.0};
    glLightfv(GL_LIGHT0,GL_POSITION,pos0);

    GLfloat pos1[]={100.0,0.0,0.0,1.0};
    glLightfv(GL_LIGHT1,GL_POSITION,pos1);

    GLfloat pos2[]={0.0,0.0,100.0,1.0};
    glLightfv(GL_LIGHT2,GL_POSITION,pos2);

    GLfloat pos3[]={100.0,0.0,100.0,1.0};
    glLightfv(GL_LIGHT3,GL_POSITION,pos3);

    GLfloat pos4[]={-100.0,0.0,0.0,1.0};
    glLightfv(GL_LIGHT4,GL_POSITION,pos4);

    GLfloat pos5[]={0.0,-100.0,0.0,0.0};
    glLightfv(GL_LIGHT5,GL_POSITION,pos5);

    GLfloat pos6[]={0.0,0.0,-100.0,0.0};
    glLightfv(GL_LIGHT6,GL_POSITION,pos6);

    /* multisampling parameters
    GLint	iMultiSample	= 0;
    GLint	iNumSamples		= 0;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
    glGetIntegerv(GL_SAMPLES, &iNumSamples);

    cout<<iMultiSample<<" "<<iNumSamples<<endl;
	*/

    /* *******************render scene here ***************************/
    glTranslatef(-obj.center_of_body->x,-obj.center_of_body->y,-obj.center_of_body->z);

    glCallList(model);


    /** ---------------- **/
    glutSwapBuffers();
}

void init(){
    //glClearColor(1.0f,1.0f,1.0f,1.0f);// white background
    glClearColor(0.0f,0.0f,0.0f,0.0f);//black background
    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHT6);

    GLfloat col0[]={1.0,1.0,1.0,1.0};
    GLfloat col1[]={0.0,0.0,1.0,1.0};
    GLfloat col2[]={0.0,1.0,0.0,1.0};
    GLfloat col3[]={1.0,0.0,0.0,1.0};

    glLightfv(GL_LIGHT0,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,col0);

    glLightfv(GL_LIGHT1,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,col1);

    glLightfv(GL_LIGHT2,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,col0);

    glLightfv(GL_LIGHT3,GL_DIFFUSE,col1);
    glLightfv(GL_LIGHT3,GL_AMBIENT,col1);

    glLightfv(GL_LIGHT4,GL_DIFFUSE,col3);
    glLightfv(GL_LIGHT4,GL_AMBIENT,col3);

    glLightfv(GL_LIGHT5,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT5,GL_AMBIENT,col0);

    glLightfv(GL_LIGHT6,GL_DIFFUSE,col2);
    glLightfv(GL_LIGHT6,GL_AMBIENT,col2);

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    obj.loadObj(filename);
    model=obj.drawModel();
}

void reshape(int w, int h){
    GLfloat fov=90.0f;

    glViewport(0,0,(GLsizei)w,(GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov,(GLfloat)w/(GLfloat)h,1.0,700.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void rotate(void){
    spin=spin+0.2;
    if(spin>360.0)
        spin=spin-360.0;
    glutPostRedisplay();

}
void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1f;

	switch (key) {
		case GLUT_KEY_LEFT :
			angle -= 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT :
			angle += 0.01f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			x -= lx * fraction;
			z -= lz * fraction;
			break;
	}
}

void processMouse(int button, int state, int x, int y){
	if(state==GLUT_DOWN){
		mx=x;
		my=y;
		cout<<x<<" "<<y<<endl;
	}
}

void processNormalKeys(unsigned char key, int x, int y){
	if(key=='u'){
		capture_frame(framenum++);
		unProject(mx,my);
		//capture_frame(framenum++);
	}
	else if(key=='p'){
		Project();
		capture_frame(framenum++);
	}

}

void unProject(int x, int y){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	cout<<posX<<" "<<posY<<" "<<posZ<<endl;

}

void Project(){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble winX, winY, winZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	gluProject( posX, posY, posZ, modelview, projection, viewport, &winX, &winY, &winZ);
	winY = (double)viewport[3] - winY;
	cout<<winX<<" "<<winY<<endl;
}

void capture_frame(unsigned int framenum){

  //global pointer float *pRGB
  pRGB = new unsigned char [3 * (SCREEN_WIDTH+1) * (SCREEN_HEIGHT + 1) ];


  // set the framebuffer to read
  //default for double buffered
  glReadBuffer(GL_BACK);

  glPixelStoref(GL_PACK_ALIGNMENT,1);//for word allignment

  glReadPixels(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, pRGB);
  char framename[200];
  sprintf(framename,"./frame/%s_frame_%04d.ppm",modelno,framenum);
  std::ofstream out(framename, std::ios::out);
  out<<"P6"<<std::endl;
  out<<SCREEN_WIDTH<<" "<<SCREEN_HEIGHT<<" 255"<<std::endl;
  out.write(reinterpret_cast<char const *>(pRGB), (3 * (SCREEN_WIDTH+1) * (SCREEN_HEIGHT + 1)) * sizeof(int));
  out.close();

  //function to store pRGB in a file named count
  delete [] pRGB;
}
