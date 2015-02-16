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
    numModel=1;
    
    modelDir='../frame/%d/frame_000%d.';
    
    r=25; %radius of toleramce
    
    count=0;
    correct=0;
    
    
%     frame=sprintf(strcat(modelDir,'jpg'),model,0);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for model=1001:1001%1001:numModel+1000
    
    for frame1=1:1
        
        f1=sprintf(strcat(modelDir,'jpg'),model,frame1);
        
        if exist(f1,'file')==0
            continue;
        end
        
<<<<<<< HEAD
        for frame2=0:0%frame1+1:1
=======
        for frame2=5:5%frame1+1:1
>>>>>>> A101
            
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
            
            file1=importdata(fname1,' ',1);
            file2=importdata(fname2,' ',1);
            
            frameKeyPoints1=file1.data;
            frameKeyPoints2=file2.data;
            
            
            %%%%%%%%%%%%%%%%% compute the affine matrix %%%%%%%%%%%%%%%%%%%
            
            [image1, des1, locs1]=sift(I1);
            [image2, des2, locs2]=sift(I2);
                
            [Motion_est, N,inliers]=homography2(des2,locs2,des1,locs1,0.6)
            
            numpoints=size(frameKeyPoints1,1);
            
            truthPoints=size(numpoints,2);
            trackPoints=size(numpoints,2);
%             
             I3=affineWarp(I1,inv(Motion_est));
             figure, imshow(I3);
%             
            for pointI=1:numpoints
                
                x1=frameKeyPoints1(pointI,1);
                y1=frameKeyPoints1(pointI,2);
                
                x2=frameKeyPoints2(pointI,1);
                y2=frameKeyPoints2(pointI,2);
                
                
                if (x1>W||x1<0)||(y1>H||y1<0)||(x2>W||x2<0)||(y2>H||y2<0) 
                    continue;
                    
                end
                
                count=count+1;
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
                        
                        truthPoints(correct,1:2)=[x1 y1];
                        trackPoints(correct,1:2)=[x_track y_track];
                    end
                end
                
                
            end
            
        end
    end
    
end

disp(count);
disp(correct);

figure, imshow(I1);
hold on
plot(truthPoints(:,1), truthPoints(:,2),'og');

figure, imshow(I2);
hold on
plot(trackPoints(:,1), trackPoints(:,2),'og');
















