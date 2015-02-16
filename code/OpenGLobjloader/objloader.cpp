/*
 * objloader.cpp
 *
 *  Created on: 26-Jan-2015
 *      Author: duhber
 *
 */

#include "objloader.h"
#include<new>
#include<stack>
coordinate::coordinate(float a, float b, float c){
	x=a;
	y=b;
	z=c;
}
coordinate::coordinate(){
	x=0.0;
	y=0.0;
	z=0.0;
}
uvcoordinate::uvcoordinate(float a, float b){
	u=a;
	v=b;
}

face::face(int ver[], int vnormal[], int vtex[], int matId){
	copy(ver,ver+3,v);
	copy(vnormal, vnormal+3,vn);
	copy(vtex,vtex+3,vt);
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

objloader::objloader(){
	includeTexture=true;
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


    data=new (nothrow)unsigned char [imsize];
    if(data==nullptr){
    	cout<<"error"<<endl;
    	exit(1);
    }

    cout<<imsize<<endl;
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

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    delete[] data;

    texIdList.push_back(texid);

    return texid;

}

void objloader::loadObj(const char* objname){

    ifstream in(objname);
    if(!in.is_open()){
        cout<<objname<<" file can not be open"<<endl;
        exit(0);
    }

    /*
     * extract the dirname
     */
    int len=strlen(objname);
    int i;

    for(i=len-1;objname[i]!='/';i--);
    strcpy(dirname,objname);

    dirname[i+1]='\0';

    char line[256];

    int mId;

    min.x=999999.9f;
    min.y=999999.9f;
    min.z=999999.9f;
    max.x=-999999.9f;
    max.y=-999999.9f;
    max.z=-999999.9f;

    while(!in.eof()){

        in.getline(line,256);

        if(line[0]=='#')
            continue;

        else if(line[0]=='m' && line[1]=='t' && line[2]=='l'){
        	char mtllib[256];
        	char temp[256];
        	sscanf(line,"mtllib %s",mtllib);
        	strcpy(temp,dirname);
        	strcat(temp,mtllib);
        	loadMaterial(temp);

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
            findMinMax(x,y,z);
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
        	int vnr[3];
        	int ver[3];
        	int vtex[]={0,0,0};

            sscanf(line,"f %d/%d/%d %d/%d/%d %d/%d/%d",&ver[0],&vtex[0],&vnr[0],&ver[1],&vtex[1],&vnr[1],&ver[2],&vtex[2],&vnr[2]);
        	//sscanf(line,"f %d//%d %d//%d %d//%d",&ver[0],&vnr[0],&ver[1],&vnr[1],&ver[2],&vnr[2]);
            f.push_back(new face(ver,vnr,vtex,mId));

        }


    }
    findCenterOfBody();

    in.close();
}

void objloader::loadMaterial(const char* mtlname){

	ifstream in(mtlname);

	if(!in.is_open()){
		cout<<"file can not be open asdfasdf"<<endl;
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
			char filename[256];
			if(line[5]=='a')
				sscanf(line,"map_Ka %s", filename);
			else if(line[5]=='d')
				sscanf(line,"map_Kd %s", filename);
			else if(line[5]=='s')
				sscanf(line,"map_Ks %s", filename);
			/*
			 *  change extension to bmp if it is jpg/png
			 */
			int len;
			len=strlen(filename);
			char temp[256];
			strcpy(temp,dirname);
			if(filename[len-1]=='g'){
				filename[len-1]='p';
				filename[len-2]='m';
				filename[len-3]='b';
			}
			strcat(temp,filename);
			cout<<temp<<endl;
			mtl[id]->texId=loadTexture(temp);
		}


	}


}

unsigned int objloader::drawModel(){

    unsigned int num;
    int prevmtl=-1;
    int material;
    num=glGenLists(1);

    glNewList(num,GL_COMPILE);

    	for(int i=0;i<f.size();i++){

    		material=f[i]->mId;

    		if(material!=prevmtl){
    			setMaterial(material);
    			prevmtl=material;
    		}
    		if(mtl[material]->texId!=-1 && includeTexture){
    			glEnable(GL_TEXTURE_2D);
    			glBindTexture(GL_TEXTURE_2D, mtl[material]->texId);
    		}
    		else
    			glDisable(GL_TEXTURE_2D);
			glBegin(GL_TRIANGLES);
    			for(int j=0;j<3;j++){
    				glNormal3f(normal[f[i]->vn[j]-1]->x,normal[f[i]->vn[j]-1]->y,normal[f[i]->vn[j]-1]->z);

    				if(mtl[material]->texId!=-1)
    					glTexCoord2f(texture[f[i]->vt[j]-1]->u,texture[f[i]->vt[j]-1]->v);

    				glVertex3f(vertex[f[i]->v[j]-1]->x,vertex[f[i]->v[j]-1]->y,vertex[f[i]->v[j]-1]->z);
    			}
            glEnd();
            if(mtl[material]->texId!=-1 && includeTexture)
            	glBindTexture(GL_TEXTURE_2D,0);
       }
    glEndList();
    clean();
    displayList.push_back(num);
    return num;
}

void objloader::setMaterial(int matId){
	GLfloat mat_amb[4], mat_dif[4],mat_spc[4];
	for(int i=0;i<3;i++){
		mat_amb[i]=mtl[matId]->Ka[i];
		mat_dif[i]=mtl[matId]->Kd[i];
		mat_spc[i]=mtl[matId]->Ks[i];
	}
	mat_amb[3]=1.0;
	mat_dif[3]=1.0;
	mat_spc[3]=1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spc);
	glMaterialf(GL_FRONT, GL_SHININESS, mtl[matId]->Ns);

}

void objloader::clean(){
	for(int i=0;i<vertex.size();i++)
		delete vertex[i];
	for(int i=0;i<normal.size();i++)
		delete normal[i];
	for(int i=0;i<texture.size();i++)
		delete texture[i];
	for(int i=0;i<f.size();i++)
		delete f[i];
	for(int i=0;i<mtl.size();i++)
		delete mtl[i];

	vertex.clear();
	normal.clear();
	texture.clear();
	f.clear();
	mtl.clear();
}

objloader::~objloader(){
	for(vector<unsigned int>::const_iterator it=texIdList.begin();it!=texIdList.end();++it)
		glDeleteTextures(1,&(*it));
	for(vector<unsigned int>::const_iterator it=displayList.begin();it!=displayList.end();++it)
		glDeleteLists(*it,1);
}

void objloader::findMinMax(float fx, float fy, float fz){
	if(fx<min.x)
		min.x=fx;
	else if(fx>max.x)
		max.x=fx;
	if(fy<min.y)
		min.y=fy;
	else if(fy>max.y)
		max.y=fy;
	if(fz<min.z)
		min.z=fz;
	else if(fz>max.z)
		max.z=fz;


}

void objloader::findCenterOfBody(){
	cout<<max.y<<" "<<min.y<<endl;

	center_of_body=new coordinate((max.x+min.x)/2,(max.y+min.y)/2,(max.z+min.z)/2);
	cout<<center_of_body->x<<" "<<center_of_body->y<<" "<<center_of_body->z<<endl;
}
