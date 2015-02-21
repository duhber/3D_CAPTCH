/*
 * readwritekeypoints.h
 *
 *  Created on: 08-Feb-2015
 *      Author: duhber
 */
#include<iostream>
#include<fstream>
#include<vector>
#include<cstdio>
#include<cmath>
#ifndef READWRITEKEYPOINTS_H_
#define READWRITEKEYPOINTS_H_

using namespace std;

struct projectedKey{
	float x,y;
	projectedKey(float , float);

};

struct unprojectedKey{
	float x,y,z;
	unprojectedKey(float, float, float);
};

class readwritekeypoints{
public:
	readwritekeypoints(float, float, float, float, float, float, float, float, float, float, float);
	~readwritekeypoints();

	vector<projectedKey*>keypoints;
	vector<unprojectedKey*>objpoints;

	float eyex, eyey, eyez, theta, phi, lx, ly, lz, upx, upy, upz;

	void readKeypoints(const char *);
	void readObjpoints(const char *);

	void writeKeypoints(const char *);
	void writeObjpoints(const char *);


};



#endif /* READWRITEKEYPOINTS_H_ */
