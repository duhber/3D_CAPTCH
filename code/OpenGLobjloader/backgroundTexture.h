/*
 * backgroundTexture.h
 *
 *  Created on: 20-Mar-2015
 *      Author: duhber
 */

#include<iostream>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<string>
#include<algorithm>
#include<vector>
#include<fstream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<ctime>
#include<map>
#include<cmath>
#ifndef BACKGROUNDTEXTURE_H_
#define BACKGROUNDTEXTURE_H_

using namespace std;

class backgroundTexture{
	private:
		unsigned int texid;
		unsigned int dpid;
	public:
		void loadTexture(const char *imgname);
		unsigned int drawBG(float);
		unsigned int drawTexCube();
		~backgroundTexture();
};




#endif /* BACKGROUNDTEXTURE_H_ */
