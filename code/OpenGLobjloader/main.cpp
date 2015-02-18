/*
 * main.cpp
 *
 *  Created on: 26-Jan-2015
 *      Author: duhber
 *
 *      IMPORTANT KEYS:
 *	--> : turn right
 *	<-- : turn left
 *	up arrow: move forward
 *	dowm arrow: move backward
 *
 *	c   : capture frame before unproject
 *	p   : capture frame after unproject
 *
 *	q   : exit
 *
 *	w   : place model2 on neg z-axis
 *	s   : place nodel2 on pos z-axis
 *	a   : place model2 on neg x-axis
 *	d   : place model2 on pos x-axis
 *
 */

#include "objloader.h"
#include "readwritekeypoints.h"
#include<cmath>
/** ***************** function definition *************/

void init();
void display();
void reshape(int w, int h);
void rotate();
void processSpecialKeys(int, int , int);
void processMouse(int, int , int ,int);
void processNormalKeys(unsigned char key, int x, int y);

void unProject();
void Project();

void setPosition(unsigned char pos);

void capture_frame(unsigned int);

void cameraMotion(int value);
/** ----------**********************************------ ****/

/* ************** global variables ********************** */

unsigned int model1, model2;// object to render
static GLfloat spin=0.0;

char *filename1, *filename2;
char *dirname;
char *pname;

char *modelno;

char unpFileName[26];

objloader obj,obj2;

float R;

readwritekeypoints keyobj(0.0, 10.0, 15.0, 0.0, 90.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

int mx=0,my=0;
GLdouble posX=0.0, posY=0.0, posZ=0.0;

bool recording=true;
unsigned int framenum=0;
unsigned char *pRGB;

int SCREEN_WIDTH=600;
int SCREEN_HEIGHT=400;

bool isUnProject=true;


float position[3];// position for model2

float  d12=0.5; // distance between model 1 and model 2
/** ******************************************************************************* **/


int main(int argc, char **argv){
	if(argc <4){
		cout<<"usage: <filename1> <filename2> <imgno> \n";
		return 1;
	}
    filename1=argv[1];
    filename2=argv[2];
    modelno=argv[3];

    if(argc==5){
        pname=argv[4];
        int l;
        l=strlen(pname);
        if(pname[l-1]=='p'){
        	keyobj.readKeypoints(pname);
        	isUnProject=false;
        	obj.includeTexture=true;
        	obj2.includeTexture=true;
        }

        else
        	exit(0);
    }
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
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutMainLoop();
    cout<<"hey babe"<<endl;

    return 0;

}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_NORMALIZE);

    /*gluLookAt(	x, 1.0f, z,
    			x+lx, 1.0f,  z+lz,
    			0.0f, 1.0f,  0.0f);*/

    gluLookAt(keyobj.eyex, keyobj.eyey, keyobj.eyez,
    		keyobj.lx+keyobj.eyex, keyobj.ly+keyobj.eyey, keyobj.lz+keyobj.eyez,
			keyobj.upx, keyobj.upy, keyobj.upz);


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
    glPushMatrix();
    	glTranslatef(-obj.center_of_body->x,-obj.center_of_body->y+(obj.dimension[1])/2,-obj.center_of_body->z);

    	if(obj.dimension[0]<obj.dimension[2]){
    		glTranslatef(obj.center_of_body->x,obj.center_of_body->y,obj.center_of_body->z);
    		glRotatef(90.0,0,1,0);
    		glTranslatef(-obj.center_of_body->x,-obj.center_of_body->y,-obj.center_of_body->z);
    	}


    	glCallList(model1);
    glPopMatrix();
    glPushMatrix();

    	glTranslatef(position[0],(position[1]+(obj2.dimension[1])/2),position[2]);

    	if(obj2.dimension[0]<obj2.dimension[2]){
    	    glTranslatef(obj2.center_of_body->x,obj2.center_of_body->y,obj2.center_of_body->z);
    	    glRotatef(90.0,0,1,0);
    	    glTranslatef(-obj2.center_of_body->x,-obj2.center_of_body->y,-obj2.center_of_body->z);
    	}
    	glCallList(model2);
    glPopMatrix();


    if(!isUnProject){
    	unProject();

    	sprintf(unpFileName,"%s/frame_%04d.u",modelno,framenum);
    	keyobj.writeObjpoints(unpFileName);
    	framenum++;
    	isUnProject=true;
    }

    if(framenum==6){
    	glutLeaveMainLoop();

    }
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
    glLightfv(GL_LIGHT0,GL_AMBIENT,col0);

    glLightfv(GL_LIGHT2,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,col0);

    glLightfv(GL_LIGHT3,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT3,GL_AMBIENT,col0);

    glLightfv(GL_LIGHT4,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT4,GL_AMBIENT,col1);

    glLightfv(GL_LIGHT5,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT5,GL_AMBIENT,col2);

    glLightfv(GL_LIGHT6,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT6,GL_AMBIENT,col3);

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    obj.loadObj(filename1);
    model1=obj.drawModel();
    obj2.loadObj(filename2);
    model2=obj2.drawModel();
    setPosition('s');

    R=*max_element(obj.dimension,obj.dimension+3);

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

		case GLUT_KEY_PAGE_UP:
			keyobj.phi-=0.01f;
			keyobj.ly=cos(keyobj.phi);
			keyobj.lx = sin(keyobj.theta)*sin(keyobj.phi);
			keyobj.lz = -cos(keyobj.theta)*sin(keyobj.phi);
			break;

		case GLUT_KEY_PAGE_DOWN:
			keyobj.phi+=0.01f;
			keyobj.ly=cos(keyobj.phi);
			keyobj.lx = sin(keyobj.theta)*sin(keyobj.phi);
			keyobj.lz = -cos(keyobj.theta)*sin(keyobj.phi);
			break;

		case GLUT_KEY_LEFT :
			keyobj.theta -= 0.01f;
			keyobj.lx = sin(keyobj.theta)*sin(keyobj.phi);
			keyobj.lz = -cos(keyobj.theta)*sin(keyobj.phi);
			break;
		case GLUT_KEY_RIGHT :
			keyobj.theta += 0.01f;
			keyobj.lx = sin(keyobj.theta)*sin(keyobj.phi);
			keyobj.lz = -cos(keyobj.theta)*sin(keyobj.phi);
			break;
		case GLUT_KEY_UP :
			keyobj.eyex += keyobj.lx * fraction;
			keyobj.eyey += keyobj.ly * fraction;
			keyobj.eyez += keyobj.lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			keyobj.eyex -= keyobj.lx * fraction;
			keyobj.eyey -= keyobj.ly * fraction;
			keyobj.eyez -= keyobj.lz * fraction;

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

	if(key=='w'){

		setPosition('w');

	}
	else if(key=='a'){

		setPosition('a');
	}
	else if(key=='s'){

		setPosition('s');
	}

	else if(key=='d'){

		setPosition('d');
	}
	else if(key=='r'){
		glutTimerFunc(100,cameraMotion, 0);
		//cameraMotion();
	}

	else if(key=='p'){
		Project();
		capture_frame(framenum);
		char mystr[256];
		sprintf(mystr,"%s/frame_%04d.p",modelno,framenum);
		keyobj.writeKeypoints(mystr);
		framenum++;
	}

	else if(key=='c'){
		capture_frame(framenum);
		char mystr[256];
		sprintf(mystr,"%s/frame_%04d.p",modelno,framenum);
		keyobj.writeKeypoints(mystr);
		glutLeaveMainLoop();
	}

	else if(key=='q'){
		glutLeaveMainLoop();
	}

}

void unProject(){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	float x,y;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	for(int i=0;i<keyobj.keypoints.size();i++){
		x=keyobj.keypoints[i]->x;
		y=keyobj.keypoints[i]->y;

		winX = (float)x;
		winY = (float)viewport[3] - (float)y;
		glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

		gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		keyobj.objpoints.push_back(new unprojectedKey(posX,posY,posZ));
	}
	//cout<<keyobj.keypoints.size()<<" "<<keyobj.objpoints.size()<<endl;
	for(int i=0;i<keyobj.keypoints.size();i++)
		delete keyobj.keypoints[i];
	keyobj.keypoints.clear();

}

void Project(){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble winX, winY, winZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	for(int i=0;i<keyobj.objpoints.size();i++){
		posX=keyobj.objpoints[i]->x;
		posY=keyobj.objpoints[i]->y;
		posZ=keyobj.objpoints[i]->z;

		gluProject( posX, posY, posZ, modelview, projection, viewport, &winX, &winY, &winZ);
		winY = (double)viewport[3] - winY;
		keyobj.keypoints.push_back(new projectedKey(winX,winY));
	}
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
  sprintf(framename,"%s/frame_%04d.ppm",modelno,framenum);
  std::ofstream out(framename, std::ios::out);
  out<<"P6"<<std::endl;
  out<<SCREEN_WIDTH<<" "<<SCREEN_HEIGHT<<" 255"<<std::endl;
  out.write(reinterpret_cast<char const *>(pRGB), (3 * (SCREEN_WIDTH+1) * (SCREEN_HEIGHT + 1)) * sizeof(int));
  out.close();

  //function to store pRGB in a file named count
  delete [] pRGB;
}

void setPosition(unsigned char pos){
	float lenx,lenz;

	lenx=obj2.dimension[0];
	lenz=obj2.dimension[2];

	if(obj2.dimension[0]<obj2.dimension[2]){

	    	float temp;
	    	temp=lenx;
	    	lenx=lenz;
	    	lenz=temp;
	}

	if(pos=='d'){
		position[0]=-obj2.center_of_body->x+(obj.dimension[0])/2+d12+(lenx)/2;
		position[1]=-obj2.center_of_body->y;
		position[2]=-obj2.center_of_body->z;
	}
	else if(pos=='a'){
		position[0]=-obj2.center_of_body->x-((obj.dimension[0])/2+d12+(lenx)/2);
		position[1]=-obj2.center_of_body->y;
		position[2]=-obj2.center_of_body->z;
	}
	else if(pos=='w'){
		position[0]=-obj2.center_of_body->x;
		position[1]=-obj2.center_of_body->y;
		position[2]=-obj2.center_of_body->z-((obj.dimension[2]/2)+d12+(lenz/2));
	}
	else if(pos=='s'){
		position[0]=-obj2.center_of_body->x;
		position[1]=-obj2.center_of_body->y;
		position[2]=-obj2.center_of_body->z+((obj.dimension[2]/2)+d12+(lenz/2));
	}
}


void cameraMotion(int value){

	//while(keyobj.theta<=360.0){
		keyobj.theta += 0.025f;

		keyobj.eyex=R*sin(keyobj.theta)*sin(keyobj.phi);

		keyobj.eyez=R*cos(keyobj.theta)*sin(keyobj.phi);

		keyobj.lx = sin(180+keyobj.theta)*sin(keyobj.phi);
		keyobj.lz = cos(180+keyobj.theta)*sin(keyobj.phi);


		glutPostRedisplay();
		glutTimerFunc(50,cameraMotion, 0);


}
