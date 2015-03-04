%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                     Created on:   9 february 2015
%                     
%                     Finished on:   
%
%                     Motion-Estimation test on 3d-CAPTCHA
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%       
%                               3D-CAPTCHA
%   The Captcha consists of 2 frames taken from same 3D model but with
%   different viewpoints. 
%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  

%clc;
close all;
clear all;

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                              INITIALIZE VARIABLES
    numModel=150;
    
    modelDir='../frame/%s/%d/frame_000%d.';
    
    r=25; %radius of toleramce
    
    count=0;
    correct=0;
    
    errorcount=0;
    
    modelCount=0;
    modelCorrect=0;
    modelAccuracy=0;
   
    modelStat=zeros(numModel,4);
    save=1;
    
%     frame=sprintf(strcat(modelDir,'jpg'),model,0);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for model=1001:numModel+1000
    modelCount=0;
    modelCorrect=0;    
    for frame1=0:0
        
        f1=sprintf(strcat(modelDir,'jpg'),model,frame1);
        
        if exist(f1,'file')==0
            continue;
        end
        
        for frame2=1:1%frame1+1:1
            
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
            
            
            %%%%%%%%%%%%%%%%% compute the affine matrix %%%%%%%%%%%%%%%%%%%
            
            [image1, des1, locs1]=sift(I1);
            [image2, des2, locs2]=sift(I2);
            
            if size(des2,1)==0
                continue;
            end
            
            [Motion_est, N,inliers]=homography2(des2,locs2,des1,locs1,0.6)
            
            numpoints=size(frameKeyPoints1,1);
            
%             I3=affineWarp(I1,inv(Motion_est));
%             figure, imshow(I3);
            
            for pointI=1:numpoints
                
                x1=frameKeyPoints1(pointI,1);
                y1=frameKeyPoints1(pointI,2);
                
                x2=frameKeyPoints2(pointI,1);
                y2=frameKeyPoints2(pointI,2);
                
                
                if (x1>W||x1<0)||(y1>H||y1<0)||(x2>W||x2<0)||(y2>H||y2<0) 
                    continue;
                    
                end
                
                count=count+1;
                modelCount=modelCount+1;
                %%%%%%%%%%%% motion estimation test %%%%%%%%%%%%%%%%%%%%%%
              
                point_track=Motion_est*[x1;y1;1];
                point_track=point_track./point_track(3);
                
                x_track=point_track(1);
                y_track=point_track(2);
                
                x_truth=x2;
                y_truth=y2;
                
                if(N>=4 && inliers>=4)
                    if ((x_truth-r<=x_track && x_track<=x_truth+r) && (y_truth-r<=y_track && y_track<=y_truth+r))
                        %disp('motion estimation');
                        correct=correct+1;
                        modelCorrect=modelCorrect+1;
                        %errorstat(count)=0;
                    else
                        errorcount=errorcount+1;
                        errorstat(errorcount)=sqrt((x_track-x_truth)^2 + (y_track-y_truth)^2)-r;
                        if(errorstat(errorcount) >=100)
                            errorstat(errorcount)=100;
                        end
                    end
                else
                    errorcount=errorcount+1;
                    errorstat(errorcount)=100;
                end
                
                
            end
            
        end
    end
    
    modelAccuracy=(modelCorrect/modelCount)*100;
    
    modelStat(model-1000,:)=[model modelCount modelCorrect modelAccuracy];
    
end

disp(count);
disp(correct);

percentage=(correct/count)*100;

disp(percentage);


modelStat(model-1000+1,:)=[0 count correct percentage];

filename=strcat('../result/MOTEST_test_result_',datestr(now,'dd_mmmm_yyyy'));

if save==1
    dlmwrite(filename,modelStat,'delimiter','\t');
    errorEstimate(errorstat,'MOTEST');
    
end













