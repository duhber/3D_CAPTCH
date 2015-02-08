/*
 * readwritekeypoints.cpp
 *
 *  Created on: 08-Feb-2015
 *      Author: duhber
 */
#include "readwritekeypoints.h"
#include <new>
#include<cstring>
projectedKey::projectedKey(float sx, float sy){
	x=sx;
	y=sy;
}


unprojectedKey::unprojectedKey(float sx, float sy, float sz){
	x=sx;
	y=sy;
	z=sz;
}

readwritekeypoints::readwritekeypoints(float x, float y, float z, float theta, float llx, float llz, float ux, float uy, float uz){
	eyex=x;
	eyey=y;
	eyez=z;

	angle=theta;
	lx=llx;
	lz=llz;

	upx=ux;
	upy=uy;
	upz=uz;
}


void readwritekeypoints::readKeypoints(const char* filename){
	ifstream in(filename);
	if(!in.is_open()){
		cout<<filename<<" file can not be open"<<endl;
	    exit(0);
	}
	char line[256];
	int i=1;
	while(!in.eof()){

		in.getline(line, 256);
		//cout<<i<<" "<<line<<endl;
		if(line[0]=='#'){
			sscanf(line,"# %f %f %f %f %f %f %f %f %f ",&eyex, &eyey, &eyez, &angle, &lx, &lz, &upx, &upy, &upz);
		}

		else if(strlen(line)>0){
			float fx,fy;
			sscanf(line,"%f %f",&fx,&fy);
			keypoints.push_back(new projectedKey(fx,fy));

		}
		//i++;
	}
	in.close();

}


void readwritekeypoints::readObjpoints(const char* filename){
	ifstream in(filename);
	if(!in.is_open()){
		cout<<filename<<" file can not be open"<<endl;
		exit(0);
	}
	char line[256];

	while(!in.eof()){

		in.getline(line, 256);

		if(line[0]=='#'){
			sscanf(line,"# %f %f %f %f %f %f %f %f %f ",&eyex, &eyey, &eyez, &angle, &lx, &lz, &upx, &upy, &upz);
		}

		else if(strlen(line)>0){
			float fx,fy,fz;
			sscanf(line,"%f %f %f",&fx,&fy,&fz);
			objpoints.push_back(new unprojectedKey(fx,fy,fz));

		}
	}
	in.close();

}

void readwritekeypoints::writeKeypoints(const char* filename){
	ofstream out(filename);

	if(out.is_open()){

		out<<"# "<<eyex<<" "<<eyey<<" "<<eyez<<" ";

		out<<angle<<" "<<lx<<" "<<lz<<" ";

		out<<upx<<" "<<upy<<" "<<upz<<"\n";

		for(int i=0;i<keypoints.size();i++){
			out<<keypoints[i]->x<<" "<<keypoints[i]->y<<"\n";
		}


		out.close();
		for(int i=0;i<keypoints.size();i++)
			delete keypoints[i];
		keypoints.clear();
	}


}

void readwritekeypoints::writeObjpoints(const char* filename){
	ofstream out(filename);

	if(out.is_open()){

		out<<"# "<<eyex<<" "<<eyey<<" "<<eyez<<" ";

		out<<angle<<" "<<lx<<" "<<lz<<" ";

		out<<upx<<" "<<upy<<" "<<upz<<"\n";

		for(int i=0;i<objpoints.size();i++){
			out<<objpoints[i]->x<<" "<<objpoints[i]->y<<" "<<objpoints[i]->z<<"\n";
		}


		out.close();


	}

}

readwritekeypoints::~readwritekeypoints(){
	for(int i=0;i<objpoints.size();i++)
				delete objpoints[i];
			objpoints.clear();

}
