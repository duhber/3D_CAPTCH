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
 *	use when rendering two models
 *
 *	w   : place model2 on neg z-axis
 *	s   : place nodel2 on pos z-axis
 *	a   : place model2 on neg x-axis
 *	d   : place model2 on pos x-axis
 *
 *	1-8 : different camera
 *
 *	g   : generate random viewpoints
 *
 *	x   : quit and return with values -1 to tell other programs that it has error (used when screen renders bad models)
 *
 *	pgup: look up camera
 *
 *	pgdn: look down camera
 *
 */

#include "objloader.h"
#include "readwritekeypoints.h"
#include<cmath>
#include<ctime>

#define pi 3.14
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

void capture_frame(unsigned int);


void setCamera(unsigned char c);

bool isVisible(double x, double y);

void light_switched();

void genLightSource(int numOfLight);
void genViewPoints();
int  getPrime(int k);

void idle();
/** ----------**********************************------ ****/

/* ************** global variables ********************** */

unsigned int model1;// object to render
static GLfloat spin=0.0;

char *filename1;
char *dirname;
char *pname;

char *modelno;

char unpFileName[26];

objloader obj;

bool deletemodel=0;

float s;// scaling parameter

unsigned char axis;// 1-> x-axis  2->z-axis

readwritekeypoints keyobj(15.0, 1.0, 15.0, 0.0, 90.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

int mx=0,my=0;
GLdouble posX=0.0, posY=0.0, posZ=0.0;

bool recording=true;
unsigned int framenum=0;
unsigned char *pRGB;

int SCREEN_WIDTH=600;
int SCREEN_HEIGHT=400;

bool isUnProject=true;
bool isCapture=true;
bool isProject=false;


float visibility;

/** ******************************************************************************* **/


int main(int argc, char **argv){
	if(argc <3){
		cout<<"usage: <filename1> <imgno> \n";
		return 1;
	}

    filename1=argv[1];
    modelno=argv[2];
    if(argc==4){
        pname=argv[3];
        int l;
        l=strlen(pname);
        if(pname[l-1]=='p'){
        	keyobj.readKeypoints(pname);
        	isUnProject=false;
        	obj.includeTexture=true;
        	//isProject=true;
        	isCapture=false;

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
    glutIdleFunc(idle);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutMainLoop();

    if(deletemodel)
    	return -1;
    else
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

    /* multisampling parameters
    GLint	iMultiSample	= 0;
    GLint	iNumSamples		= 0;
    glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
    glGetIntegerv(GL_SAMPLES, &iNumSamples);

    cout<<iMultiSample<<" "<<iNumSamples<<endl;
	*/

    /* *******************render scene here ***************************/
    glPushMatrix();
    	glScalef(s,s,s);
    	glTranslatef(-obj.center_of_body->x,-obj.center_of_body->y+(obj.dimension[1])/2,-obj.center_of_body->z);

    	glCallList(model1);
    glPopMatrix();

    if(!isUnProject){
    	unProject();

    	sprintf(unpFileName,"%s/frame_%04d.u",modelno,framenum);
    	keyobj.writeObjpoints(unpFileName);
    	framenum++;
    	isUnProject=true;
    }

    if(isCapture){
		capture_frame(framenum);
		char mystr[256];
		sprintf(mystr,"%s/frame_%04d.p",modelno,framenum);
		keyobj.writeKeypoints(mystr);
		glutLeaveMainLoop();

    }

    if(isProject){
		Project();
		capture_frame(framenum);
		char mystr[256];
		sprintf(mystr,"%s/frame_%04d.p",modelno,framenum);
		keyobj.writeKeypoints(mystr);
		if(visibility>50.0){
			framenum++;
		}
    }

    if(framenum==2){
    	cout<<filename1<<endl;
    	glutLeaveMainLoop();

    }
    /** ---------------- **/
    glutSwapBuffers();
}

void init(){
    //glClearColor(1.0f,1.0f,1.0f,1.0f);// white background
    glClearColor(0.0f,0.0f,0.0f,0.0f);//black background
    glShadeModel(GL_SMOOTH);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    /*glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHT6);*/

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

    GLfloat pos0[]={0.0,50.0,0.0,0.0};
    glLightfv(GL_LIGHT0,GL_POSITION,pos0);

    GLfloat pos1[]={100.0,0.0,0.0,1.0};
    glLightfv(GL_LIGHT1,GL_POSITION,pos1);

    GLfloat pos2[]={0.0,100.0,0.0,1.0};
    glLightfv(GL_LIGHT2,GL_POSITION,pos2);

    GLfloat pos3[]={100.0,0.0,100.0,1.0};
    glLightfv(GL_LIGHT3,GL_POSITION,pos3);

    GLfloat pos4[]={-100.0,0.0,0.0,1.0};
    glLightfv(GL_LIGHT4,GL_POSITION,pos4);

    GLfloat pos5[]={0.0,-100.0,0.0,0.0};
    glLightfv(GL_LIGHT5,GL_POSITION,pos5);

    GLfloat pos6[]={0.0,0.0,-100.0,0.0};
    glLightfv(GL_LIGHT6,GL_POSITION,pos6);

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    obj.loadObj(filename1);
    model1=obj.drawModel();

    //scale down if it is too big

    if(obj.dimension[0]>obj.dimension[2]){
    	s=30.0/obj.dimension[0];
    	axis='1';

    }
    else{
    	s=30.0/obj.dimension[2];
    	axis='2';
    }
    obj.dimension[0]=s*obj.dimension[0];
    obj.dimension[1]=s*obj.dimension[1];
    obj.dimension[2]=s*obj.dimension[2];
    if(isUnProject)
    	setCamera(axis);
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
			keyobj.phi-=1.0f;
			keyobj.ly=cos(keyobj.phi*pi/180);
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;

		case GLUT_KEY_PAGE_DOWN:
			keyobj.phi+=1.0f;
			keyobj.ly=cos(keyobj.phi*pi/180);
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;

		case GLUT_KEY_LEFT :
			keyobj.theta -= 1.0f;
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;
		case GLUT_KEY_RIGHT :
			keyobj.theta += 1.0f;
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
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
	glutPostRedisplay();
}

void processMouse(int button, int state, int x, int y){
	if(state==GLUT_DOWN){
		mx=x;
		my=y;
		cout<<x<<" "<<y<<endl;
	}
}

void processNormalKeys(unsigned char key, int x, int y){


	if(key=='p'){
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

	else if(key=='1')
		setCamera('1');

	else if(key=='2')
		setCamera('2');

	else if(key=='3')
		setCamera('3');

	else if(key=='4')
		setCamera('4');

	else if(key=='5')
		setCamera('5');

	else if(key=='6')
		setCamera('6');

	else if(key=='7')
		setCamera('7');

	else if(key=='8')
		setCamera('8');

	else if(key=='g')
		genViewPoints();

	else if(key=='x'){
		deletemodel=1;
		glutLeaveMainLoop();
	}

	else if(key=='i'){
		idle();
	}

	else if(key=='s'){
		cout<<keyobj.eyex<<" "<<keyobj.eyey<<" "<<keyobj.eyez<<" ";

		cout<<keyobj.theta<<" "<<keyobj.phi<<" ";

		cout<<keyobj.lx<<" "<<keyobj.ly<<" "<<keyobj.lz<<"\n ";
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

	visibility=0.0;

	for(int i=0;i<keyobj.objpoints.size();i++){
		posX=keyobj.objpoints[i]->x;
		posY=keyobj.objpoints[i]->y;
		posZ=keyobj.objpoints[i]->z;

		gluProject( posX, posY, posZ, modelview, projection, viewport, &winX, &winY, &winZ);
		winY = (double)viewport[3] - winY;
		keyobj.keypoints.push_back(new projectedKey(winX,winY));

		if(isVisible(winX,winY)){
			visibility+=1.0;
		}
	}
	visibility=(visibility*100.0)/keyobj.objpoints.size();
	cout<<modelno<<" "<<visibility<<endl;
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

void setCamera(unsigned char c){
	switch(c){
		case '1':
			keyobj.eyex=0.0;
			keyobj.eyey=obj.dimension[1]/2;
			keyobj.eyez=obj.dimension[0]>obj.dimension[2]?obj.dimension[0]/2:obj.dimension[0]/2+obj.dimension[2]/2;

			keyobj.theta=0.0;
			keyobj.phi=90.0;

			keyobj.ly=cos(keyobj.phi*pi/180);
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;
		case '2':
			keyobj.eyez=0.0;
			keyobj.eyey=obj.dimension[1]/2;
			keyobj.eyex=obj.dimension[2]>obj.dimension[0]?obj.dimension[2]/2:obj.dimension[0]/2+obj.dimension[2]/2;

			keyobj.theta=-90.0;
			keyobj.phi=90.0;

			keyobj.ly=cos(keyobj.phi*pi/180);
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;
		case '3':
			keyobj.eyex=0.0;
			keyobj.eyey=obj.dimension[1]/2;
			keyobj.eyez=-1*(obj.dimension[0]>obj.dimension[2]?obj.dimension[0]/2:obj.dimension[0]/2+obj.dimension[2]/2);

			keyobj.theta=-180.0;
			keyobj.phi=90.0;

			keyobj.ly=cos(keyobj.phi*pi/180);
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;
		case '4':
			keyobj.eyez=0.0;
			keyobj.eyey=obj.dimension[1]/2;
			keyobj.eyex=-1*(obj.dimension[2]>obj.dimension[0]?obj.dimension[2]/2:obj.dimension[0]/2+obj.dimension[2]/2);

			keyobj.theta=90.0;
			keyobj.phi=90.0;

			keyobj.ly=cos(keyobj.phi*pi/180);
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;
		case '5':
			keyobj.eyex=0.0;
			keyobj.eyey=obj.dimension[1];
			keyobj.eyez=obj.dimension[0]>obj.dimension[2]?obj.dimension[0]/2:obj.dimension[0]/2+obj.dimension[2]/2;

			keyobj.theta=0.0;
			keyobj.phi=120.0;

			keyobj.ly=cos(keyobj.phi*pi/180);
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;
		case '6':
			keyobj.eyez=0.0;
			keyobj.eyey=obj.dimension[1];
			keyobj.eyex=obj.dimension[2]>obj.dimension[0]?obj.dimension[2]/2:obj.dimension[0]/2+obj.dimension[2]/2;


			keyobj.theta=-90.0;
			keyobj.phi=120.0;

			keyobj.ly=cos(keyobj.phi*pi/180);
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;
		case '7':
			keyobj.eyex=0.0;
			keyobj.eyey=obj.dimension[1];
			keyobj.eyez=-1*(obj.dimension[0]>obj.dimension[2]?obj.dimension[0]/2:obj.dimension[0]/2+obj.dimension[2]/2);

			keyobj.theta=-180.0;
			keyobj.phi=120.0;

			keyobj.ly=cos(keyobj.phi*pi/180);
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;
		case '8':
			keyobj.eyez=0.0;
			keyobj.eyey=obj.dimension[1];
			keyobj.eyex=-1*(obj.dimension[2]>obj.dimension[0]?obj.dimension[2]/2:obj.dimension[0]/2+obj.dimension[2]/2);

			keyobj.theta=90.0;
			keyobj.phi=120.0;

			keyobj.ly=cos(keyobj.phi*pi/180);
			keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			break;
	}
	//glutPostRedisplay();
}

void genViewPoints(){
	int delx, delz, dely, delphi, deltheta;
	int sign;
	srand(time(NULL));



	sign=rand()%2;

	int k=(int)ceil(obj.dimension[1]);
	k=getPrime(k);

	dely=rand()%(k);
	delphi=rand()%31+10;

	if(sign==0){
		dely-=2;
		keyobj.phi-=delphi;
	}
	else{
		dely+=2.0;
		keyobj.phi+=delphi;
	}

	keyobj.eyey=dely;

	sign=rand()%2;

	if(axis=='1'){

		delz=rand()%10+5;
		k=(int)ceil(obj.dimension[0]/2);
		k=getPrime(k);
		delx=rand()%(k)+5;
		if(sign==0){
			keyobj.eyex=-delx;
		}
		else{
			keyobj.eyex=delx;

		}
		keyobj.eyez=delz;
	}
	else{
		delx=rand()%10+5;
		k=(int)ceil(obj.dimension[2]/2);
		k=getPrime(k);
		delz=rand()%(k)+5;
		if(sign==0){
			keyobj.eyez=-delz;
		}
		else{
			keyobj.eyez=delz;

		}
		keyobj.eyex=delx;

	}

	sign=rand()%2;

	deltheta=rand()%31+10;
	if(sign==0){
		keyobj.theta-=deltheta;
	}
	else{
		keyobj.theta+=deltheta;
	}

	keyobj.ly=cos(keyobj.phi*pi/180);
	keyobj.lx = sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
	keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);

}

bool isVisible(double x, double y){

	if(x>0.0 && x <SCREEN_WIDTH && y>0.0 && y <SCREEN_HEIGHT)
		return true;
	else
		return false;
}


void idle(){
	isProject=true;
	setCamera(axis);
	genViewPoints();
	genLightSource(2);
	glutPostRedisplay();
}

int getPrime(int k){

    while(1){

        int i;
        for(i=2;i*i<=k;i++){
            if(k%i==0){
                k=k+1;
                break;
            }
        }
        if(i*i>k)
            break;

    }

    return k;

}


void light_switched(){
	int i;

	for(int j=0;j<7;j++){
		i=rand()%2;
		if(i==1)
			glEnable(GL_LIGHT0+j);
		else
			glDisable(GL_LIGHT0+j);
	}
}

void genLightSource(int numOfLight){
	int theta,phi;
	float r, temp;
	temp=max(obj.dimension[0],obj.dimension[2]);
	r=max(temp,obj.dimension[1])+5.0;
	float lightpos[4];
	lightpos[3]=1.0;
	for(int i=0;i<numOfLight;i++){

		theta=rand()%360;
		phi=rand()%360;

		lightpos[0]=r*sin(float(theta)*pi/180.0)*sin(float(phi)*pi/180.0);
		lightpos[1]=r*cos(float(phi)*pi/180.0);
		lightpos[2]=r*cos(float(theta)*pi/180.0)*sin(float(phi)*pi/180.0);
		glEnable(GL_LIGHT0+i);
		glLightfv(GL_LIGHT0+i,GL_POSITION,lightpos);
	}

	for(int i=numOfLight;i<7;i++){
		glDisable(GL_LIGHT0+i);
	}

}




