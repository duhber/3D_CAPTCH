#include<iostream>
#include<cstdio>
#include<fstream>
#include<cstring>
#include<algorithm>
using namespace std;

int main(int argc, char ** argv){

    if(argc!=4){
        cout<<"usage: <temp.key> <temp> <frame_0000.desc>"<<endl;
        return -1;
    }
    
    ifstream in(argv[1]);
    if(!in.is_open()){
        cout<<"file cannot open"<<endl;
        return -1;
    }
    
    int len;
    char line[256];
    int numspace;
    ofstream out(argv[2]);
    ofstream desc(argv[3]);
    
    while(!in.eof()){
        float x,y,temp1, temp2;
        in.getline(line,256);
        len=strlen(line);
        numspace=count(line,line+len,' ');

        cout<<numspace<<endl;

        if(numspace==3){
            sscanf(line,"%f %f %f %f",&y,&x,&temp1,&temp2);
            out<<x<<" "<<y<<endl;
            for(int i=0;i<7;i++){
                in.getline(line,256);
                desc<<line<<" ";
            }
            desc<<endl;

        }

    }
    
    in.close();
    out.close();
    desc.close();

}

