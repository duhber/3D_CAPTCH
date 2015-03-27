%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                           DATE: 10 March 2015 
%
%                  selecting challenge points intelligently
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%       
%                               3D-CAPTCHA
%                          
%   FAST is used for selecting the salient points. The hacker used                   
% tries to estimate the points using linear combinations. The hacker runs
% sift on both the images. and used those nearby sift points to find the 
% the best matching points.
%two possible attack:
%                     1. take nearest SIFT points
%                     2. using linear combinations
% 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

clc;
close all;
clear all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                              INITIALIZE VARIABLES
    numModel=332;
    
    modelDir='../frame/%d/frame_000%d.';
    
    r=25; %radius of toleramce
    
    count=0;
   
    modelStat=zeros(numModel,4);
    save=0;
    MOTEST=0;
    NRSIFT=0;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for model=1001:1000+numModel
    disp(model);
    modelCount=0;
    
    for frame1=1:1
        
        f1=sprintf(strcat(modelDir,'jpg'),model,frame1);
        
        if exist(f1,'file')==0
            continue;
        end
        
        for frame2=2:2%frame1+1:1
            
            f2=sprintf(strcat(modelDir,'jpg'),model,frame2);
            
            if exist(f2,'file')==0
                continue;
            end
            
            % load the frames and keypoints of I1 and I2
            
            I1=imread(f1);
            
            I2=imread(f2);
            
%             figure, imshow(I1);
%             figure, imshow(I2);
            
            H=size(I1,1);% height of image
            W=size(I1,2);% width of image
            
            fname1=sprintf(strcat(modelDir,'p'),model,frame1);
            fname2=sprintf(strcat(modelDir,'p'),model,frame2);
            
            if(exist(fname1,'file')==0)
                continue;
            end
            
            if(exist(fname2,'file')==0)
                continue;
            end
            
            file1=importdata(fname1,' ',1);
            
            if isstruct(file1)==0
                continue;
            end
            file2=importdata(fname2,' ',1);
            
            if isstruct(file2)==0
                continue;
            end
            
            frameKeyPoints1=file1.data;
            frameKeyPoints2=file2.data;
            
            
            %%%%%% compute the sift descriptor of the two frame %%%%%%%%%%%
            [image1, des1, locs1]=sift(I1);
            [image2, des2, locs2]=sift(I2);
            
            
            if size(des2,1)==0
                continue;
            end            
            
                            
            numpoints=size(frameKeyPoints1,1);

            
%           Throw out points which are not visible in both frames          

            for pointI=1:numpoints
                
                x1=frameKeyPoints1(pointI,1);
                y1=frameKeyPoints1(pointI,2);
               
                x2=frameKeyPoints2(pointI,1);
                y2=frameKeyPoints2(pointI,2);
                
                
                if (x1>W||x1<0)||(y1>H||y1<0)||(x2>W||x2<0)||(y2>H||y2<0) 
                    continue;
                    
                end
%                 goodKeyPoints ::-----> points which are visible in both
%                 frames
                modelCount=modelCount+1;
                goodKeyPoints1(modelCount,1)=x1;
                goodKeyPoints1(modelCount,2)=y1;
                
                goodKeyPoints2(modelCount,1)=x2;
                goodKeyPoints2(modelCount,2)=y2;
                
            end
            count=count+1;
            
            
            %find the best challenge points            
            
            initialFrame=sprintf(strcat(modelDir,'jpg'),model,0);
            
            I=imread(initialFrame);
            
            [image, des, locs]=sift(I);
            
            fastPoints=goodKeyPoints1';
            tempPoints=locs(:,1:2)';
            siftPoints(1,:)=tempPoints(2,:);
            siftPoints(2,:)=tempPoints(1,:);
     
            idx=selectBestPoint(siftPoints',fastPoints');
           
            
            x1=goodKeyPoints1(idx,1);
            y1=goodKeyPoints1(idx,2);
            
            x2=goodKeyPoints2(idx,1);
            y2=goodKeyPoints2(idx,2);
            
            
            % using nearest sift
            
            trackPoint=nearestSIFTmethod([x1 y1],des1,locs1,des2,locs2);
            
            if numel(trackPoint)~=0
               
                d=((x2-trackPoint(1))^2+(y2-trackPoint(2))^2)^0.5;
                
                if d<=25.0
                    NRSIFT=NRSIFT+1;
                end
                
            end
            
            sift_track_point=trackPoint;
            
            
            %using motion estimation
            
            trackPoint=motionEstimation([x1 y1],des1,locs1,des2,locs2);
            
            
            if numel(trackPoint)~=0
               
                d=((x2-trackPoint(1))^2+(y2-trackPoint(2))^2)^0.5;
                
                if d<=25.0
                    MOTEST=MOTEST+1;
                end
                
            end
            
            plotPoints(I,fastPoints,siftPoints,[x1 y1],sift_track_point,trackPoint, model); 
            
            
            
            
            clear tempPoints fastPoints siftPoints goodKeyPoints1 goodKeyPoints2
            
        end
    end
    
    
    
end


disp('nearest sift');
NRSIFT_accu=(NRSIFT/count)*100;
disp(NRSIFT_accu);


disp('motion estimation');

MOTEST_accu=(MOTEST/count)*100;
disp(MOTEST_accu);
