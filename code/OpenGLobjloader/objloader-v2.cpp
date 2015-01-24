/*
 * objloader-v2.cpp
 *
 *  Created on: 24-Jan-2015
 *      Author: duhber
 */

#include<iostream>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<string>
#include<algorithm>
#include<vector>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
using namespace std;

/** function definition **/
void init();
void display();
void reshape(int w, int h);
int loadObject();
void rotate();
/** ---------------- ****/

struct coordinate{
    float x,y,z;
    coordinate(float a, float b, float c): x(a),y(b),z(c){}
};

struct face{
    int vnormal[3];
    int f[3];
    face(int v1,int n1,int v2,int n2,int v3,int n3){
        f[0]=v1;
        f[1]=v2;
        f[2]=v3;
        vnormal[0]=n1;
        vnormal[1]=n2;
        vnormal[2]=n3;
    }
};

/* global variables*/
int obj;// object to render
static GLfloat spin=0.0;
char *filename;

int main(int argc, char **argv){
	if(argc !=2){
		cout<<"usage: <filename> \n";
		return 1;
	}

    filename=argv[1];
    /* Initialize glut */
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,600);
    glutCreateWindow("Perspective's GLUT Template");

    init();// initialize openGL

    /* register glut call backs */

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(rotate);

    glutMainLoop();

    return 0;

}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_NORMALIZE);
    gluLookAt(5.0,0.0,-5.0,0.0,0.0,-1.0,0.0,1.0,0.0);

    GLfloat pos[]={0.0,0.0,-5.0,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,pos);

    /* render scene here */

    glPushMatrix();
        glRotatef(spin,0.0,1.0,0.0);
        glCallList(obj);
    glPopMatrix();

    /** ---------------- **/
    glutSwapBuffers();
}

void init(){
    glClearColor(0.0f,0.0f,0.0f,0.0f);

    glShadeModel(GL_SMOOTH);
    GLfloat mat_spec[]={0.0,0.0,1.0,1.0};
    GLfloat mat_shin[]={50.0};

    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_spec);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shin);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat col[]={1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,col);

    obj=loadObject();
}

void reshape(int w, int h){
    GLfloat fov=90.0f;

    glViewport(0,0,(GLsizei)w,(GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov,(GLfloat)w/(GLfloat)h,1.0,100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void rotate(void){
    spin=spin+1.0;
    if(spin>360.0)
        spin=spin-360.0;
    glutPostRedisplay();

}

int loadObject(){
    vector<coordinate*> vertex;
    vector<face*> faces;
    vector<coordinate*>normals;

    ifstream in(filename);
    if(!in.is_open()){
        cout<<"file can not be open"<<endl;
        return -1;
    }
    char line[256];
    while(!in.eof()){

        in.getline(line,256);

        if(line[0]=='#')
            continue;

        else if(line[0]=='v' && line[1]==' '){

            float tmpx, tmpy, tmpz;
            sscanf(line,"v %f %f %f",&tmpx,&tmpy,&tmpz);
            vertex.push_back(new coordinate(tmpx,tmpy,tmpz));

        }

	    else if(line[0]=='v' && line[1]=='n'){
            float tmpx, tmpy, tmpz;
            sscanf(line,"vn %f %f %f",&tmpx,&tmpy,&tmpz);
            normals.push_back(new coordinate(tmpx,tmpy,tmpz));
        }

        else if(line[0]=='f'){
            int v1,v2,v3,n1,n2,n3;

            sscanf(line,"f %d//%d %d//%d %d//%d",&v1,&n1,&v2,&n2,&v3,&n3);
            faces.push_back(new face(v1,n1,v2,n2,v3,n3));

        }


    }

    in.close();

    /** draw scene **/

    int num;

    num=glGenLists(1);

    glNewList(num,GL_COMPILE);
    	for(int i=0;i<faces.size();i++){
    		cout<<normals[faces[i]->vnormal[0]-1]->x<<" "<<normals[faces[i]->vnormal[0]-1]->y<<" "<<normals[faces[i]->vnormal[0]-1]->z<<endl;
			glBegin(GL_TRIANGLES);
            	glNormal3f(normals[faces[i]->vnormal[0]-1]->x,normals[faces[i]->vnormal[0]-1]->y,normals[faces[i]->vnormal[0]-1]->z);
                glVertex3f(vertex[faces[i]->f[0]-1]->x,vertex[faces[i]->f[0]-1]->y,vertex[faces[i]->f[0]-1]->z);
                glNormal3f(normals[faces[i]->vnormal[1]-1]->x,normals[faces[i]->vnormal[1]-1]->y,normals[faces[i]->vnormal[1]-1]->z);
                glVertex3f(vertex[faces[i]->f[1]-1]->x,vertex[faces[i]->f[1]-1]->y,vertex[faces[i]->f[1]-1]->z);
                glNormal3f(normals[faces[i]->vnormal[1]-1]->x,normals[faces[i]->vnormal[2]-1]->y,normals[faces[i]->vnormal[2]-1]->z);
                glVertex3f(vertex[faces[i]->f[2]-1]->x,vertex[faces[i]->f[2]-1]->y,vertex[faces[i]->f[2]-1]->z);
            glEnd();
        }
    glEndList();

    /* ---------------*/
    for(int i=0;i<faces.size();i++)
        delete faces[i];
    for(int i=0;i<vertex.size();i++)
        delete vertex[i];
    for(int i=0;i<normals.size();i++)
        delete normals[i];

    return num;

}
