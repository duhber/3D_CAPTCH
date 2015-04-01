%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                           DATE: 30 March 2015 
%
%                  selecting challenge points intelligently using distotion
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
    numModel=5;
    
    modelDir='../frame/%d/frame_000%d.';
    
    r=25; %radius of toleramce
    
    count=0;
   
    modelStat=zeros(numModel,4);
    save=0;
    NRSIFT = zeros(numModel,11);
    MOTEST = zeros(numModel,11);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for model=1001:1000+numModel
    disp(model);
    modelCount=0;
    
    for frame1=0:0
        
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
             numpoints=size(frameKeyPoints1,1); 
            numpoints
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
                goodKeyPoints1(1,modelCount)=x1;
                goodKeyPoints1(2,modelCount)=y1;
                
                goodKeyPoints2(1,modelCount)=x2;
                goodKeyPoints2(2,modelCount)=y2;
                
            end
            
            
            
            %%%%%% compute the sift descriptor of the two frame %%%%%%%%%%%
%             [image1, des1, locs1]=sift(I1);
%             [image2, des2, locs2]=sift(I2);
            
            
%             if size(des2,1)==0
%                 continue;
%             end            
%             
                            
                     
            
            p1=sprintf('../frame/%d/p1',model);
            
       
            p2=sprintf('../frame/%d/p2',model);
%             
%             pointL=importdata(p1,' ');
%             pointR=importdata(p2,' ');
%             pointL=pointL';
%             pointR=pointR';
            cpfilename=sprintf('../frame/%d/candidatePoints.cp',model);
            
            cp=importdata(cpfilename,' ');
            cp=cp';
            f1=figure;
            imshow(I1);
            hold on
%              plot(pointL(1,:), pointL(2,:), 'rx');
             plot(cp(1,:), cp(2,:),'rx');
%             plot(goodKeyPoints1(1,:), goodKeyPoints1(2,:), 'rx');
            
            saveas(f1,p1,'jpg');
            
%             close
            
            f2=figure;
            imshow(I2);
            hold on
%              plot(pointR(1,1), pointR(2,1), 'bx');
%              plot(pointR(1,2:size(pointL,2)), pointR(2,2:size(pointL,2)), 'rx');
               plot(cp(3,:), cp(4,:),'rx');
%             plot(goodKeyPoints2(1,:), goodKeyPoints2(2,:), 'rx');
            saveas(f2,p2,'jpg');                
            clear goodKeyPoints1 goodKeyPoints2
%             close
        end
    end
    
    
    
end
