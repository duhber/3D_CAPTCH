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
void rotate();

void processSpecialKeys(int, int , int);
void processMouse(int, int , int ,int);
void processNormalKeys(unsigned char key, int x, int y);

void unProject();
void Project();

void capture_frame(unsigned int);

void setCamera();

bool isVisible(double x, double y);

void genLightSource(int numOfLight);
void genViewPoints();
void reDraw();

void setObj2Pos(unsigned char c);

void idle();


/* ************** global variables ********************** */

unsigned int model1,model2,bgid,tex1,tex2;// object to render
static GLfloat spin=0.0;

char *filename1, *filename2, *texfile, *texfile2;
char *dirname;
char *pname;

char *modelno;
char *mode;

char unpFileName[26];

objloader obj,obj2;
backgroundTexture bg1,bg2;



bool deletemodel=0;

float s,s2;// scaling parameter

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

float position[3];// position for model2

float  d12=0.0, R;// distance between the model

int maxAngle=47;

float theta_init;
float phi_init;
/** ******************************************************************************* **/


int main(int argc, char **argv){
	if(argc <7){
		cout<<"usage: <filename1> <filename2> <imgno><mode><texfile1><texfile2>(optional)<projectedfiles> \n";
		return 1;
	}

    filename1=argv[1];
    filename2=argv[2];
    modelno=argv[3];
    mode=argv[4];
    cout<<argv[5]<<endl;
    texfile=argv[5];
    texfile2=argv[6];
    if(argc==8){
        pname=argv[7];
        int l;
        l=strlen(pname);
        if(pname[l-1]=='p'){
        	keyobj.readKeypoints(pname);
        	isUnProject=false;

        	obj2.includeTexture=true;
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


    if(mode[1]=='t' && isUnProject){
    	glPushMatrix();
    		glScalef(R*3,R*3,R*3);
    		//glTranslatef(-0.5,-0.5,-1.0);
        	glCallList(bgid);
        glPopMatrix();
    }

    glPushMatrix();
    	glScalef(s,s,s);
    	glTranslatef(-obj.center_of_body->x,-obj.center_of_body->y,-obj.center_of_body->z);

    	glCallList(model1);
    glPopMatrix();


    if(mode[0]=='d'){
    	//cout<<"true"<<endl;
    	glPushMatrix();
    		glScalef(s2,s2,s2);
    		glTranslatef(position[0],position[1],position[2]);

    		glCallList(model2);
    	glPopMatrix();

    }

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
			reDraw();
			framenum++;
		}
    }

    if(framenum==3){
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

    GLfloat pos0[]={50.0,50.0,50.0,0.0};
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

    obj2.loadObj(filename2);
    model2=obj2.drawModel();

    bg1.loadTexture(texfile);
    tex1=bg1.drawBG();

    bg2.loadTexture(texfile2);
    tex2=bg2.drawBG();

    //scale down if it is too big


    s=30.0/obj.dimension[0];
    /*obj.dimension[0]=s*obj.dimension[0];
    obj.dimension[1]=s*obj.dimension[1];
    obj.dimension[2]=s*obj.dimension[2];
    */
    //cout<<obj.dimension[0]<<" "<< obj.dimension[1]<<" "<<obj.dimension[2]<<endl;
    //float max_len1,max_len2;

    //max_len1=*max_element(obj.dimension,obj.dimension+3);




    // scale object2

    s2=30.0/obj2.dimension[0];
    /*obj2.dimension[0]=s2*obj2.dimension[0];
    obj2.dimension[1]=s2*obj2.dimension[1];
    obj2.dimension[2]=s2*obj2.dimension[2];
	*/
   //cout<<obj2.dimension[0]<<" "<< obj2.dimension[1]<<" "<<obj2.dimension[2]<<endl;

    //max_len2=*max_element(obj2.dimension,obj2.dimension+3);

    //R=max(max_len1*s,max_len2*s2)/2;
    R=(obj.radiusBV)*s;
    if(isUnProject){
    	srand(time(NULL));
    	keyobj.phi=rand()%135;
    	keyobj.theta=rand()%360;
    	setCamera();
    }
    theta_init=keyobj.theta;
    phi_init=keyobj.phi;
    setObj2Pos('w');
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

	switch(key){
		case 'p':
			Project();
			capture_frame(framenum);

			sprintf(mystr,"%s/frame_%04d.p",modelno,framenum);
			keyobj.writeKeypoints(mystr);
			framenum++;
		break;

		case 'c':
			capture_frame(framenum);
			sprintf(mystr,"%s/frame_%04d.p",modelno,framenum);
			keyobj.writeKeypoints(mystr);
			glutLeaveMainLoop();
		break;

		case 'q':
			glutLeaveMainLoop();
		break;

		case 'g':
			genViewPoints();
		break;

		case 'x':
			deletemodel=1;
			glutLeaveMainLoop();
		break;

		case 'i':
			idle();
		break;

		case 'w':
			setObj2Pos('w');
		break;

		case 'a':
			setObj2Pos('a');
		break;

		case 's':
			setObj2Pos('s');
		break;

		case 'd':
			setObj2Pos('d');
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

	del1=rand()%maxAngle;
	del2=rand()%maxAngle;

	keyobj.theta=theta_init;
	keyobj.phi=phi_init;

	delR=rand()%7;
	//R=R-delR;

	keyobj.theta+=del1*sign;
	keyobj.phi+=del2*sign2;
}

bool isVisible(double x, double y){

	if(x>0.0 && x <SCREEN_WIDTH && y>0.0 && y <SCREEN_HEIGHT)
		return true;
	else
		return false;
}


void idle(){
	isProject=true;
	bgid=tex1;
	if(framenum==2){
		bgid=tex2;
		genViewPoints();
	}
	setCamera();
	genLightSource(2);
	glutPostRedisplay();
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


void setObj2Pos(unsigned char pos){
	d12=max(obj.dimension[0],obj2.dimension[0]);
	position[0]=-obj2.center_of_body->x;
	position[1]=-obj2.center_of_body->y;
	position[2]=-obj2.center_of_body->z;

}

void reDraw(){
		obj.includeTexture=true;
		obj.loadObj(filename1);
		model1=obj.drawModel();
}


