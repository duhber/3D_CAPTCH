/*
 * main.cpp
 *
 *  Created on: 26-Jan-2015
 *      Author: duhber
 *
 *
 *
 * 		THIS IS A TEST MODEL
 *
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
 *	mode:
 *		s-> single model
 *		d-> two model
 *		t-> with background texture
 *		x-> no background texture
 *
 */

#include "objloader.h"
#include "readwritekeypoints.h"
#include "backgroundTexture.h"
#include<cmath>
#include<ctime>

#define pi 3.14

/** ***************** function definition *************/

void init();
void display();
void reshape(int w, int h);

void processSpecialKeys(int, int , int);
void processMouse(int, int , int ,int);
void processNormalKeys(unsigned char key, int x, int y);

void unProject();
void unProject(float x, float y);
void unProject(float x, float y, ofstream& o);

void Project();
void Project(GLdouble, GLdouble, GLdouble);

void capture_frame(unsigned int);

void setCamera();

bool isObjectPoint(double x, double y, double z);
void checkVisibility();


void genLightSource(int numOfLight);
void setLight(int i);
void offLight(int i);

void genViewPoints();

void idle();

void selectChallengePoint();

void writeDepthDiff();

void setInitialPos();

double isObjCloseToCam();

/* ************** global variables ********************** */

unsigned int model1,tex1, bbox;// object to render

char *filename, *texfile;
char *dirname;
char *pname, *unpname;

char *modelno;
char *mode;

char unpFileName[26];

objloader obj;
backgroundTexture bg1;

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

bool isUnProject=false;
bool isCapture=true;
bool isProject=false;

bool cpVisible=false;

float visibility;

float position[3];// position for model2

float  d12=0.0, R;// distance between the model

int maxAngle=31;

bool showBox=false;

float theta_init;
float phi_init;
float R_init;
int pointIndex;

double depth_diff;

int countLimit=0;

int return_val=0;

/** ******************************************************************************* **/


int main(int argc, char **argv){
	if(argc <4){
		cout<<"usage: <filename><imgno><mode>(optional)<texfile><projectedfiles> \n";
		return 1;
	}

    filename=argv[1];
    modelno=argv[2];
    mode=argv[3];
    cout<<filename<<" "<<modelno<<" "<<mode<<endl;
    obj.includeTexture=true;
    if(mode[0]=='2'){

        pname=argv[4];
        int l;
        l=strlen(pname);
        if(pname[l-1]=='p'){
        	keyobj.readKeypoints(pname);
        	//isUnProject=true;
        	//isCapture=false;

        }

        else
        	exit(0);
    }
    if(mode[0]=='3'){
    	texfile=argv[5];
    	unpname=argv[4];
    	obj.dither=false;
    	keyobj.readObjpoints(unpname);
    	pointIndex=atoi(argv[6]);
    	//cout<<"point Index:---->  "<<pointIndex<<endl;
    	framenum=1;
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
    if(mode[0]!='0')
    	glutIdleFunc(idle);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                  GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutMainLoop();

    return return_val;
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


    if(mode[0]=='3' && (framenum==2)){
    	glPushMatrix();
        	glCallList(tex1);
        glPopMatrix();
    }

    glPushMatrix();
    	glScalef(s,s,s);
    	glTranslatef(-obj.center_of_body->x,-obj.center_of_body->y,-obj.center_of_body->z);

    	glCallList(model1);
    glPopMatrix();
    if(showBox){
    	glPushMatrix();
    		glScalef(s,s,s);
    		glTranslatef(-obj.center_of_body->x,-obj.center_of_body->y,-obj.center_of_body->z);
    		glCallList(bbox);
    	glPopMatrix();
    }

    if(mode[0]=='2'){

    	unProject();

    	sprintf(unpFileName,"%s/frame_%04d.u",modelno,framenum);
    	keyobj.writeObjpoints(unpFileName);
    	selectChallengePoint();
		glutLeaveMainLoop();
    	//isUnProject=false;
    }

    if(mode[0]=='1' &&  isObjCloseToCam()<10.0){
		capture_frame(framenum);
		char mystr[256];
		sprintf(mystr,"%s/frame_%04d.p",modelno,framenum);
		keyobj.writeKeypoints(mystr);
		glutLeaveMainLoop();

    }

    if(mode[0]=='3'  ){ //isObjCloseToCam()>5.0 ){

    	//Project();

    	checkVisibility();
		if( cpVisible){
			Project();
			capture_frame(framenum);
			char mystr[256];
			sprintf(mystr,"%s/frame_%04d.p",modelno,framenum);
			keyobj.writeKeypoints(mystr);
			//reDraw();

			framenum++;
		}
/*		else{
			for(int i=0;i<keyobj.keypoints.size();i++)
				delete keyobj.keypoints[i];
			keyobj.keypoints.clear();
		}*/
    }

    if(framenum==3){

    	//cout<<filename<<endl;
    	//writeDepthDiff();
    	glutLeaveMainLoop();

    }

    if(countLimit>500){
    	return_val=1;
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
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    //glEnable(GL_LIGHT3);
    //glEnable(GL_LIGHT4);
   // glEnable(GL_LIGHT5);
    glEnable(GL_LIGHT6);


    /********************************/
    obj.loadObj(filename);
    model1=obj.drawModel();

    //scale down if it is too big
    s=30.0/obj.dimension[0];

    R=(obj.radiusBV)*s*0.75;
    /*******************************/

    GLfloat col0[]={1.0,1.0,1.0,1.0};
    GLfloat col1[]={0.0,0.0,1.0,1.0};
    GLfloat col2[]={0.0,1.0,0.0,1.0};
    GLfloat col3[]={1.0,0.0,0.0,1.0};

   glLightfv(GL_LIGHT0,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,col0);

    //glLightfv(GL_LIGHT1,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT1,GL_AMBIENT,col0);

    glLightfv(GL_LIGHT2,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT2,GL_AMBIENT,col0);

   // glLightfv(GL_LIGHT3,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT3,GL_AMBIENT,col0);

    //glLightfv(GL_LIGHT4,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT4,GL_AMBIENT,col0);

    //glLightfv(GL_LIGHT5,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT5,GL_AMBIENT,col0);

    glLightfv(GL_LIGHT6,GL_DIFFUSE,col0);
    glLightfv(GL_LIGHT6,GL_AMBIENT,col0);

    GLfloat pos0[]={R,R,R,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,pos0);

    GLfloat pos1[]={R,0.0,0.0,1.0};
    glLightfv(GL_LIGHT1,GL_POSITION,pos1);

    GLfloat pos2[]={0.0,R,0.0,1.0};
    glLightfv(GL_LIGHT2,GL_POSITION,pos2);

    GLfloat pos3[]={0.0,0.0,R,1.0};
    glLightfv(GL_LIGHT3,GL_POSITION,pos3);

    GLfloat pos4[]={-1*R,0.0,0.0,1.0};
    glLightfv(GL_LIGHT4,GL_POSITION,pos4);

    GLfloat pos5[]={0.0,0.0,-1*R,1.0};
    glLightfv(GL_LIGHT5,GL_POSITION,pos5);

    GLfloat pos6[]={0.0,-1*R,0.0,1.0};
    glLightfv(GL_LIGHT6,GL_POSITION,pos6);

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);



    bbox=obj.drawBoundingBox();



    if(mode[0]=='1'){
    	srand(time(NULL));
    	keyobj.phi=90-rand()%45;
    	keyobj.theta=rand()%360;
    	setCamera();
    	setLight(0);
    }
    theta_init=keyobj.theta;
    phi_init=keyobj.phi;
    R_init=R;
    if(mode[0]=='3'){
        bg1.loadTexture(texfile);
        tex1=bg1.drawBG(R*7);

    	genViewPoints();

    	setCamera();
    	setLight(0);
    	//genLightSource(3);
    }

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

void processSpecialKeys(int key, int xx, int yy) {

	float fraction = 0.1f;

	switch (key) {

		case GLUT_KEY_PAGE_UP:
			keyobj.phi-=1.0f;
			setCamera();
			break;

		case GLUT_KEY_PAGE_DOWN:
			keyobj.phi+=1.0f;
			setCamera();
			break;

		case GLUT_KEY_LEFT :
			keyobj.theta -= 1.0f;
			setCamera();
			break;
		case GLUT_KEY_RIGHT :
			keyobj.theta += 1.0f;
			setCamera();
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
	char mystr[256];
	GLboolean status;
	switch(key){

		case '0':

			glDisable(GL_LIGHT0);
		break;
		case '1':
			offLight(1);
		break;
		case '2':
			offLight(2);
		break;
		case '3':
			offLight(3);
		break;
		case '4':
			offLight(4);
		break;
		case '5':
			offLight(5);
		break;
		case '6':
			offLight(6);
		break;

		case 'p':
			Project(posX,posY,posZ);
			/*Project();
			capture_frame(framenum);

			sprintf(mystr,"%s/frame_%04d.p",modelno,framenum);
			keyobj.writeKeypoints(mystr);
			framenum++;*/
		break;

		case 'u':
			unProject(mx,my);
		break;

		case 'c':
			capture_frame(framenum);
			sprintf(mystr,"%s/frame_%04d.p",modelno,framenum);
			keyobj.writeKeypoints(mystr);
            cout<<"This is model number  "<<modelno<<endl;
			glutLeaveMainLoop();
		break;

		case 'q':
			glutLeaveMainLoop();
		break;
		case 's':
			cout<<"\n"<<keyobj.theta<<" "<<keyobj.phi<<endl;
		break;
		case 'r':
			cout<<R<<endl;
		break;

		case 'g':
			isObjCloseToCam();
		break;

		case 'i':
			idle();
		break;

		case 'b':
			if(showBox==true){
				showBox=false;
			}
			else
				showBox=true;
		break;

		case 'x':
			return_val=1;
			glutLeaveMainLoop();
		break;

		case 'z':
			cout<<keyobj.eyex<<" "<<keyobj.eyey<<" "<<keyobj.eyez<<" ";

			cout<<keyobj.theta<<" "<<keyobj.phi<<" ";

			cout<<keyobj.lx<<" "<<keyobj.ly<<" "<<keyobj.lz<<"\n ";
		break;
	}
	glutPostRedisplay();
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

}

void Project(){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble winX, winY, winZ;

	GLfloat zdepth=0.0;
	GLdouble epsilon=0.003;
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	for(int i=0;i<keyobj.objpoints.size();i++){
		posX=keyobj.objpoints[i]->x;
		posY=keyobj.objpoints[i]->y;
		posZ=keyobj.objpoints[i]->z;

		gluProject( posX, posY, posZ, modelview, projection, viewport, &winX, &winY, &winZ);
		glReadPixels( winX, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zdepth );

		winY = (double)viewport[3] - winY;
		keyobj.keypoints.push_back(new projectedKey(winX,winY));

	}

}
void checkVisibility(){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble winX, winY, winZ;

	GLfloat zdepth=0.0;
	GLdouble epsilon=0.003;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );


	char cpointname[256];
	sprintf(cpointname,"%s/anotherfile",modelno);
	ifstream in(cpointname);

	if(!in.is_open()){
        cout<<cpointname<<" file can not be open"<<endl;
        exit(0);
	}

	int temp;
	in>>temp;
	int cntVisible=0;
	int countTotal=0;
	while(!in.eof()){
		in>>posX>>posY>>posZ;

		if(!isObjectPoint(posX,posY,posZ)){
			continue;
		}

		gluProject( posX, posY, posZ, modelview, projection, viewport, &winX, &winY, &winZ);
		glReadPixels( winX, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zdepth );
		winY = (double)viewport[3] - winY;

		depth_diff=fabs(winZ-zdepth);

		countTotal++;

		if(depth_diff<0.001  and winX>0 and winY>0 )
			cntVisible++;

	}
	visibility=((float)cntVisible/(float)countTotal)*100.0;
	if(visibility>80.0)
		cpVisible=true;
	else
		cpVisible=false;
	cout<<"visibility "<<cntVisible<<" "<<countTotal<<" "<<visibility<<endl;
}

void Project(GLdouble pX, GLdouble pY, GLdouble pZ){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLdouble winX, winY, winZ;
	GLfloat zdepth=0.0;
	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	gluProject( pX, pY, pZ, modelview, projection, viewport, &winX, &winY, &winZ);


	glReadPixels( winX, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zdepth );
	winY = (double)viewport[3] - winY;

	cout<<winX<<" "<<winY<<" "<<endl;
	cout<<"winZ = "<<winZ<<" "<<"zdepth = "<<zdepth<<endl;
	cout<<"diff = "<<fabs(winZ-zdepth)<<endl;
}

void unProject(float x, float y){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	cout<<y<<endl;
	winY = (float)viewport[3] - (float)y;
	cout<<winY<<endl;

	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	//cout<<winZ<<endl;
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	cout<<posX<<" "<<posY<<" "<<posZ<<endl;
}
void unProject(float x, float y, ofstream& o){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	//cout<<y<<endl;
	winY = (float)viewport[3] - (float)y;
	//cout<<winY<<endl;

	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	//cout<<winZ<<endl;
	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	o<<posX<<" "<<posY<<" "<<posZ<<endl;

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

void setCamera(){
			//R=sqrt(keyobj.eyex*keyobj.eyex+keyobj.eyey*keyobj.eyey+keyobj.eyez*keyobj.eyez);
			keyobj.eyex=R*sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.eyey=R*cos(keyobj.phi*pi/180);
			keyobj.eyez=R*cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);

			keyobj.ly=-cos(keyobj.phi*pi/180);
			keyobj.lx = -sin(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
			keyobj.lz = -cos(keyobj.theta*pi/180)*sin(keyobj.phi*pi/180);
}

void genViewPoints(){
	srand(time(NULL));
	int sign=rand()%2;
	if(!sign)
		sign=-1;
	int	sign2=rand()%2;
	if(!sign2)
		sign2=-1;

	float del1,del2, delR;

	del1=rand()%60+30;
	del2=rand()%5+10;
	delR=rand()%5;

	R=R_init+sign2*delR;
	keyobj.theta=theta_init+del1*sign;
	keyobj.phi=phi_init-del2;
	//cout<<keyobj.theta<<" "<<keyobj.phi<<endl;
}

bool isObjectPoint(double x, double y, double z){
	double d;
	d=sqrt(pow(x,2)+pow(y,2)+pow(z,2));
	if(d>R)
		return false;
	else
		return true;
}


void idle(){
	if(mode[0]!='1' && framenum==1){
		countLimit++;
		cout<<"generating viewpoints --->>"<<countLimit<<endl;
    	genViewPoints();
        setCamera();
        setLight(0);
    }
	else
		if(mode[0]=='1'){
		setInitialPos();
	    setCamera();
	    setLight(0);
	}

    //genLightSource(3);
    glutPostRedisplay();
}

void genLightSource(int numOfLight){
	int theta,phi;
	float r, temp;
	temp=max(obj.dimension[0],obj.dimension[2]);
	//r=max(temp,obj.dimension[1])+5.0;
	r=R;
	float lightpos[4];
	lightpos[3]=1.0;
	int theta_list[]={90,180,270,360};
	for(int i=0;i<numOfLight;i++){

		theta=keyobj.theta+theta_list[i];
		phi=30;

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

void setLight(int i){
	int theta,phi;
	theta=keyobj.theta;
	phi=keyobj.phi;
	float lightpos[4];
	lightpos[3]=1.0;
	lightpos[0]=R*sin(float(theta)*pi/180.0)*sin(float(phi)*pi/180.0);
	lightpos[1]=R*cos(float(phi)*pi/180.0);
	lightpos[2]=R*cos(float(theta)*pi/180.0)*sin(float(phi)*pi/180.0);
	glEnable(GL_LIGHT0+i);
	glLightfv(GL_LIGHT0+i,GL_POSITION,lightpos);

}

void offLight(int i){
	glDisable(GL_LIGHT0+i);
}



void selectChallengePoint(){
	int totalPoint;
	totalPoint=keyobj.objpoints.size();
	//cout<<totalPoint<<endl;
	srand(time(NULL));
	while(1){
		pointIndex=rand()%totalPoint;
		//cout<<keyobj.objpoints[pointIndex]->x<<" "<<keyobj.objpoints[pointIndex]->y<<" "<<keyobj.objpoints[pointIndex]->z<< endl;
		if (isObjectPoint(keyobj.objpoints[pointIndex]->x, keyobj.objpoints[pointIndex]->y, keyobj.objpoints[pointIndex]->z))
			break;
	}
	//cout<<pointIndex<<endl;
	char cpointname[256], anotherfile[256];
	sprintf(cpointname,"%s/challengepoint",modelno);
	ofstream o(cpointname);
	o<<pointIndex<<endl;
	o.close();


	sprintf(anotherfile,"%s/anotherfile",modelno);
	ofstream o2(anotherfile);

	int a[]={-3,-2,-1,0,1,2,3};

	for(int i=0;i<7;i++)
		for(int j=0;j<7;j++)
			unProject(keyobj.keypoints[pointIndex]->x-a[j],keyobj.keypoints[pointIndex]->y-a[i],o2);
	o2.close();


	for(int i=0;i<keyobj.keypoints.size();i++)
		delete keyobj.keypoints[i];
	keyobj.keypoints.clear();
}

void writeDepthDiff(){
	ofstream dfile;
	dfile.open("../frame/depth_difference.dat",ofstream::out|ofstream::app);

	dfile<<modelno<<" "<<depth_diff<<endl;
	dfile.close();
}

double isObjCloseToCam(){
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );
	double _X, _Y,_Z;
	int totCount=0;
	double dist;

	int npCount=0;

	for(int xx=25;xx<=600;xx+=25){
		for(int yy=25;yy<=400;yy+=25){


			winX = (float)xx;
			winY = (float)viewport[3] - (float)yy;

			glReadPixels( xx, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

			//cout<<winZ<<endl;
			gluUnProject( winX, winY, winZ, modelview, projection, viewport,&_X, &_Y, &_Z);

			if(isObjectPoint(_X, _Y,_Z)){

				dist=sqrt(pow(keyobj.eyex-_X,2)+pow(keyobj.eyey-_Y,2)+pow(keyobj.eyez-_Z,2));
				totCount++;

				if(dist<6.0)
					npCount++;
			}
		}
	}
	double per=(double(npCount)/double(totCount))*100.0;
	cout<<per<<endl;
	return per;
}

void setInitialPos(){
	srand(time(NULL));
	keyobj.phi=90-rand()%45;
	keyobj.theta=rand()%360;

}

