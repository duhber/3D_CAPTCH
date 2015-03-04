%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                           DATE: day month 2015 
%
%                               Analyze the results
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%       
%                               3D-CAPTCHA
%                          
%                              Explanation         
%           feb 27 experiment result:
%               sift test: 15.233
%               motest test: 12.399 GLOBAL
%  Since the attacking accuracy is unexpectedly high. 
% I'm trying to find out the reason
% and this code analyze the variations 
% of viewpoints parameters (x,y,z,theta, phi). 
% 
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

clc;
close all;
clear all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                             INITIALIZE VARIABLES
modelDir='../frame/%s/%d/frame_000%d.';
renderMethod='without_texture';
resltDir='../result/%s/%s_test_result_27_February_2015';

frame1=0;
frame2=1;

outDir='../result/%s/%s_accuracy_based_view_point_diff_data';
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                                LOAD DATA

mottest=dlmread(sprintf(resltDir,renderMethod,'MOTEST'));
siftest=dlmread(sprintf(resltDir,renderMethod,'SIFT'));

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

modelnum=size(mottest,1)-1;

outfile=zeros(modelnum,8);


for model=1001:1000+modelnum
    
    if mottest(model-1000,2)==0
        continue;
    end
    
    
    fname1=sprintf(strcat(modelDir,'p'),renderMethod,model,frame1);
    fname2=sprintf(strcat(modelDir,'p'),renderMethod,model,frame2);    
    
    file1=importdata(fname1,' ',1);
    
    file2=importdata(fname2,' ',1);
    
    str1=char(file1.textdata);
    str2=char(file2.textdata);
    
    view1=sscanf(str1,'# %f %f %f %f %f %f %f %f %f %f %f',[1 5]);
    view2=sscanf(str2,'# %f %f %f %f %f %f %f %f %f %f %f',[1 5]);
        
    
    viewdiff=abs(view1-view2);
    
    %     if the initial camera is on the x axis
    
    if view1(4)==-90
        temp=viewdiff(1);
        viewdiff(1)=viewdiff(3);
        viewdiff(3)=temp;
    end
    
    outfile(model-1000,:)=[model mottest(model-1000,4) siftest(model-1000,4) viewdiff];
    
    
end

% arrange rows in ascending order of motion estimation accuracy 

n=size(outfile,1);

[val,index]=sort(outfile(:,2));

motestViewPointDiff=zeros(size(outfile));

for i=1:n
    motestViewPointDiff(i,:)=outfile(index(i),:);
end

filename=sprintf(outDir,renderMethod,'MOTEST');

dlmwrite(filename,motestViewPointDiff,'delimiter','\t');

[val,index]=sort(outfile(:,3));

siftViewPointDiff=zeros(size(outfile));

for i=1:n
    siftViewPointDiff(i,:)=outfile(index(i),:);
end

filename=sprintf(outDir,renderMethod,'SIFT');

dlmwrite(filename,siftViewPointDiff,'delimiter','\t');
















