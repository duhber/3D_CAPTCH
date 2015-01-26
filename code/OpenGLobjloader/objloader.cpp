/*
 * objloader.cpp
 *
 *  Created on: 26-Jan-2015
 *      Author: duhber
 */

#include "objloader.h"

coordinate::coordinate(float a, float b, float c){
	x=a;
	y=b;
	z=c;
}

uvcoordinate::uvcoordinate(float a, float b){
	u=a;
	v=b;
}

face::face(int ver[], int vnormal[], int vtex[], int matId){
	v=ver;
	vn=vnormal;
	vt=vtex;
	mId=matId;
}

material::material(){
	for(int i=0;i<3;i++){
		Ka[i]=0.0;
		Kd[i]=0.0;
		Ks[i]=0.0;
	}
	illum=1;
	texId=-1;
	Ns=0.0;
}

unsigned int objloader::loadTexture(const char * imgname){
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
    data=new unsigned char [imsize];

    fread(data,1,imsize,file);

    fclose(file);
    char temp;

    for(int i=0;i<imsize;i+=3){
        temp=data[i];
        data[i]=data[i+2];
        data[i+2]=temp;
    }

    /* ***** load and bind the texture ************** */
    unsigned int texid;

    glGenTextures(1, &texid);

    glBindTexture(GL_TEXTURE_2D, texid);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    delete data;

    texIdList.push_back(texid);

    return texid;

}

void objloader::loadObj(const char* objname){

    ifstream in(objname);
    if(!in.is_open()){
        cout<<"file can not be open"<<endl;
        exit(0);
    }

    char line[256];

    int mId;

    while(!in.eof()){

        in.getline(line,256);

        if(line[0]=='#')
            continue;

        else if(line[0]=='m' && line[1]=='t' && line[2]=='l'){
        	char mtllib[128];
        	sscanf(line,"mtllib %s",mtllib);
        	loadMaterial(mtllib);

        }

        else if(line[0]=='u' && line[1]=='s' && line[2]=='e'){
        	char temp[128];
        	sscanf(line,"usemtl %s",temp);
        	string usemtl(temp);
        	mId=mtlmap[usemtl];
        }

        else if(line[0]=='v' && line[1]==' '){

            float x, y, z;
            sscanf(line,"v %f %f %f",&x, &y, &z);
            vertex.push_back(new coordinate(x,y,z));

        }

	    else if(line[0]=='v' && line[1]=='n'){

            float x, y, z;
            sscanf(line,"vn %f %f %f",&x, &y, &z);
            normal.push_back(new coordinate(x, y, z));
        }

	    else if(line[0]=='v' && line[1]=='t'){

            float u, v;
            sscanf(line,"vt %f %f",&u, &v);
            texture.push_back(new uvcoordinate(u, v));
        }

        else if(line[0]=='f'){
        	int vn[3];
        	int v[3];
        	int vt[3];

            sscanf(line,"f %d/%d/%d %d/%d/%d %d/%d/%d",&v[0],&vt[0],&vn[0],&v[1],&vt[1],&vn[1],&v[2],&vt[2],&vn[2]);
            f.push_back(new face(v,vn,vt,mId));

        }


    }

    in.close();
}

void objloader::loadMaterial(const char* mtlname){

	ifstream in(mtlname);

	if(!in.is_open()){
		cout<<"file can not be open"<<endl;
	    exit(0);
	}

	char line[256];

	int id;

	while(!in.eof()){

		in.getline(line,256);

		if(line[0]=='#')
			continue;
		else if(line[0]=='n' && line[1]=='e' && line[2]=='w'){
			char temp[128];
			sscanf(line,"newmtl %s",temp);
			string newmtl(temp);

			mtl.push_back(new material());
			id=mtl.size()-1;

			mtlmap[newmtl]=id;
		}

		else if(line[0]=='K' && line[1]=='a'){
			float a[3];
			sscanf(line,"Ka %f %f %f",&a[0], &a[1], &a[2]);
			copy(a,a+3,mtl[id]->Ka);
		}

		else if(line[0]=='K' && line[1]=='d'){
			float d[3];
			sscanf(line,"Kd %f %f %f",&d[0], &d[1], &d[2]);
			copy(d,d+3,mtl[id]->Kd);
		}
		else if(line[0]=='K' && line[1]=='s'){
			float s[3];
			sscanf(line,"Ks %f %f %f",&s[0], &s[1], &s[2]);
			copy(s,s+3,mtl[id]->Ks);
		}

		else if(line[0]=='i'){
			int i;
			sscanf(line,"illum %d",&i);
			mtl[id]->illum=i;
		}

		else if(line[0]=='N' && line[1]=='s'){
			float ns;
			sscanf(line,"Ns %f",&ns);
			mtl[id]->Ns=ns;
		}

		else if(line[0]=='m' && line[1]=='a' && line[2]=='p'){
			char filename[128];
			if(line[5]=='a')
				sscanf(line,"map_Ka %s", filename);
			else if(line[5]=='d')
				sscanf(line,"map_Kd %s", filename);
			else if(line[5]=='s')
				sscanf(line,"map_Ks %s", filename);
			mtl[id]->texId=loadTexture(filename);
		}


	}

}

unsigned int objloader::drawModel(){

    unsigned int num;

    num=glGenLists(1);

    glNewList(num,GL_COMPILE);
    	for(int i=0;i<f.size();i++){

    		if(mtl[2]->texId)

			glBegin(GL_TRIANGLES);
    			for(int j=0;j<3;j++){
    				glNormal3f(normal[f[i]->vn[j]-1]->x,normal[f[i]->vn[j]-1]->y,normal[f[i]->vn[j]-1]->z);


    				glVertex3f(vertex[f[i]->v[j]-1]->x,vertex[f[i]->v[j]-1]->y,vertex[f[i]->v[j]-1]->z);
    			}
            glEnd();
       }
    glEndList();

    return num;
}
