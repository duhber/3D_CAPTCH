/*
 * backgroundTexture.cpp
 *
 *  Created on: 20-Mar-2015
 *      Author: duhber
 */
#include "backgroundTexture.h"
#include<new>

void backgroundTexture::loadTexture(const char * imgname){

	/* loading the bmp file */
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imsize;
	unsigned char *data;

    FILE *file=fopen(imgname,"rb");
    if(!file){
        cout<<"Image could not be open\n";
        exit(0);
    }
    if(fread(header,1,54,file)!=54 || header[0]!='B' || header[1]!='M'){
        cout<<"Not a correct BMP file\n";
        exit(1);
    }

    dataPos= *(int*)&(header[0xA]);
    imsize = *(int*)&(header[0x22]);
    width  = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if(imsize==0)
        imsize=width*height*3;
    if(dataPos==0)
        dataPos=54;


    data=new (nothrow)unsigned char [imsize];
    if(data==nullptr){
    	cout<<"error"<<endl;
    	exit(1);
    }

    //cout<<imsize<<endl;
    fread(data,1,imsize,file);

    fclose(file);
    char temp;

 /*   for(int i=0;i<imsize;i+=3){
        temp=data[i];
        data[i]=data[i+2];
        data[i+2]=temp;
    }

    /* ***** load and bind the texture ************** */

    glGenTextures(1, &texid);

    glBindTexture(GL_TEXTURE_2D, texid);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    delete[] data;

    cout<<imgname<<endl;

}

unsigned int backgroundTexture::drawBG(float r){
	unsigned int num;
	num=glGenLists(1);
	float z;
	z=5;
	glNewList(num,GL_COMPILE);
    	glEnable(GL_TEXTURE_2D);
    	glBindTexture(GL_TEXTURE_2D,texid);

    	glBegin(GL_QUADS);
    	    // Back Face
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f(-r, -r,  -r);  // Bottom Left Of The Texture and Quad
    	    glTexCoord2f(z, 0.0f); glVertex3f( r, -r,  -r);  // Bottom Right Of The Texture and Quad
    	    glTexCoord2f(z, z); glVertex3f( r,  r,  -r);  // Top Right Of The Texture and Quad
    	    glTexCoord2f(0.0f, z); glVertex3f(-r,  r,  -r);  // Top Left Of The Texture and Quad
    	    // Front Face
    	    glTexCoord2f(z, 0.0f); glVertex3f(-r, -r, r);  // Bottom Right Of The Texture and Quad
    	    glTexCoord2f(z, z); glVertex3f(-r,  r, r);  // Top Right Of The Texture and Quad
    	    glTexCoord2f(0.0f, z); glVertex3f( r,  r, r);  // Top Left Of The Texture and Quad
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f( r, -r, r);  // Bottom Left Of The Texture and Quad
    	    // Bottom Face
    	    glTexCoord2f(0.0f, z); glVertex3f(-r,  -r, -r);  // Top Left Of The Texture and Quad
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f(-r,  -r,  r);  // Bottom Left Of The Texture and Quad
    	    glTexCoord2f(z, 0.0f); glVertex3f( r,  -r,  r);  // Bottom Right Of The Texture and Quad
    	    glTexCoord2f(z, z); glVertex3f( r,  -r, -r);  // Top Right Of The Texture and Quad
    	    // Top Face
    	    glTexCoord2f(z, z); glVertex3f(-r, r, -r);  // Top Right Of The Texture and Quad
    	    glTexCoord2f(0.0f, z); glVertex3f( r, r, -r);  // Top Left Of The Texture and Quad
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f( r, r,  r);  // Bottom Left Of The Texture and Quad
    	    glTexCoord2f(z, 0.0f); glVertex3f(-r, r,  r);  // Bottom Right Of The Texture and Quad
    	    // left face
    	    glTexCoord2f(z, 0.0f); glVertex3f( -r, -r, -r);  // Bottom Right Of The Texture and Quad
    	    glTexCoord2f(z, z); glVertex3f( -r,  r, -r);  // Top Right Of The Texture and Quad
    	    glTexCoord2f(0.0f, z); glVertex3f( -r,  r,  r);  // Top Left Of The Texture and Quad
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f( -r, -r,  r);  // Bottom Left Of The Texture and Quad
    	    // right Face
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f(r, -r, -r);  // Bottom Left Of The Texture and Quad
    	    glTexCoord2f(z, 0.0f); glVertex3f(r, -r,  r);  // Bottom Right Of The Texture and Quad
    	    glTexCoord2f(z, z); glVertex3f(r,  r,  r);  // Top Right Of The Texture and Quad
    	    glTexCoord2f(0.0f, z); glVertex3f(r,  r, -r);  // Top Left Of The Texture and Quad
    	glEnd();

        glBindTexture(GL_TEXTURE_2D,0);
	glEndList();
	dpid=num;
	return num;
}

unsigned int backgroundTexture::drawTexCube(){
	unsigned int num;
	num=glGenLists(1);
	cout<<num<<endl;
	glNewList(num,GL_COMPILE);
    	glEnable(GL_TEXTURE_2D);
    	glBindTexture(GL_TEXTURE_2D,texid);

    	glBegin(GL_QUADS);
    	    // Front Face
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    	    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    	    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    	    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
    	    // Back Face
    	    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    	    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    	    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    	    // Top Face
    	    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    	    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    	    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    	    // Bottom Face
    	    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
    	    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    	    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    	    // Right face
    	    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
    	    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);  // Top Right Of The Texture and Quad
    	    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
    	    // Left Face
    	    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
    	    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
    	    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
    	    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);  // Top Left Of The Texture and Quad
    	glEnd();

        glBindTexture(GL_TEXTURE_2D,0);
	glEndList();
	dpid=num;
	return num;
}




backgroundTexture::~backgroundTexture(){
    glDeleteTextures(1,&texid);
    glDeleteLists(dpid,1);
}
