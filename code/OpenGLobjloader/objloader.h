/*
 * objloader.h
 *
 *  Created on: 26-Jan-2015
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
#include<map>

#ifndef OBJLOADER_H_
#define OBJLOADER_H_

using namespace std;

struct coordinate{
    float x,y,z;
    coordinate(float a, float b, float c);
    coordinate();
};

struct uvcoordinate{
	float u,v;
	uvcoordinate(float a, float b);
};

struct face{
    int vn[3];
    int v[3];
    int vt[3];
    int mId;// material id
    face(int ver[], int vnormal[], int vtex[], int matId);
};

struct material{
	//string name;
	float Ka[3], Kd[3], Ks[3];
	int illum, texId;//illum = 2 denotes the presence of specular highlights
	float Ns;// Shininess of material

	material();
};

class objloader{
	private:
		vector<coordinate*> vertex;
		vector<coordinate*>normal;
		vector<uvcoordinate*>texture;
		vector<face*> f;
		vector<material*>mtl;
		map<string,int>mtlmap;
		vector<unsigned int>texIdList;
		vector<unsigned int>displayList;

		coordinate min,max;

		char dirname[256];

		unsigned int loadTexture(const char *imgname);
		void loadMaterial(const char* mtlname);
		void setMaterial(int matId);
		void clean();
		void findCenterOfBody();
		void findMinMax(float x, float y, float z);


	public:
		coordinate *center_of_body;
		~objloader();
	 	void loadObj(const char* objname);
	 	unsigned int drawModel();
};


#endif /* OBJLOADER_H_ */
