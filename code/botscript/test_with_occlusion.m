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
    numModel=460;
    
    modelDir='../frame/%d/frame_000%d.';
    
    r=25; %radius of toleramce
    
    count=0;
    
    save=0;
    NRSIFT = 0;
    MOTEST = 0;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for model=1001:1000+numModel
    disp(model);
    modelCount=0;
    
    for frame1=1:1
        
        f1=sprintf(strcat(modelDir,'jpg'),model,frame1);
        
        if exist(f1,'file')==0
            continue;
        end
        
        for frame2=3:3
            
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
            %% candidate points
            cpfilename=sprintf('../frame/%d/challengepoint',model);
            
            cp=importdata(cpfilename)+1;            
            
            x1=round(frameKeyPoints1(cp,1));
            y1=round(frameKeyPoints1(cp,2));
            
            x2=round(frameKeyPoints2(cp,1));
            y2=round(frameKeyPoints2(cp,2));
%             x2=x1;
%             y2=y1;
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            
            texfile='../OpenGLobjloader/octexture/eagle3.jpeg';
            teximage=imread(texfile);
            
%             I2=obfuscation(x1,y1,I1,I2);
%             
            I2=occlude(x2,y2,I2,teximage);
            
            
            %%%%%% compute the sift descriptor of the two frame %%%%%%%%%%%
            [image1, des1, locs1]=sift(I1);
            [image2, des2, locs2]=sift(I2);
            
            if size(des1,1)<60
                continue;
            end
            
            if size(des2,1)<60
                continue;
            end            
            
%             per=analyseSiftPoints(x2,y2,25,locs2);     
  
            %find the best challenge points                      
            
            [Motion_est, N,inliers]=homography2(des2,locs2,des1,locs1,0.6);
           
            count=count+1;

            
            % using nearest sift
            leftpoint=[];
            rightpoint=[];
            trackPoint=nearestSIFTmethod([x1 y1],des1,locs1,des2,locs2);
%             [leftpoint, rightpoint]=nNearestSiftPoints([x1 y1],des1,locs1,des2,locs2,10);
            
            
           
            
            if numel(trackPoint)~=0
               
                d=((x2-trackPoint(1))^2+(y2-trackPoint(2))^2)^0.5;
                
                if d<=25.0
                    NRSIFT=NRSIFT+1;
                    leftI=sprintf('../siftTracked/%d_left.jpg',model);
                    Ileft=showPoints(I1,'red',x1,y1,2,[],[],leftpoint);
                    imwrite(Ileft,leftI,'jpg');
                    
                    rightI=sprintf('../siftTracked/%d_right.jpg',model);
                    Irite=showPoints(I2,'red',x2,y2,2,trackPoint,[],rightpoint);
                    imwrite(Irite,rightI,'jpg');
                end
                
            end
            
            sift_track_point=trackPoint;
            
            
            %using motion estimation
            
%             trackPoint=motionEstimation([x1 y1],des1,locs1,des2,locs2);
            
            

             trackPoint=[];
    
             if(N>=4 && inliers>=4)
                point_track=Motion_est*[x1;y1;1];
    
                point_track=point_track./point_track(3);
    
                x_track=point_track(1);
                y_track=point_track(2);
                
                trackPoint=[x_track y_track];
            end
             

            if numel(trackPoint)~=0
               
                d=((x2-trackPoint(1))^2+(y2-trackPoint(2))^2)^0.5;
                
                if d<=25.0
                    MOTEST=MOTEST+1;
                    
                    leftI=sprintf('../motestTracked/%d_left.jpg',model);
                    Ileft=showPoints(I1,'red',x1,y1,2,[],[],[]);
                    imwrite(Ileft,leftI,'jpg');
                    
                    rightI=sprintf('../motestTracked/%d_right.jpg',model);
                    Irite=showPoints(I2,'red',x2,y2,2,[],trackPoint,[] );
                    imwrite(Irite,rightI,'jpg');
                    
                    
                end
                
            end
            
            leftI=sprintf('../image/%d_left.jpg',model);
            rightI=sprintf('../image/%d_right.jpg',model);
            
            Ileft=showPoints(I1,'red',x1,y1,2,[],[],leftpoint);
            Irite=showPoints(I2,'red',x2,y2,2,sift_track_point,trackPoint,rightpoint );
            
            imwrite(Ileft,leftI,'jpg');
            imwrite(Irite,rightI,'jpg');
          
%             plotPoints(I1,[],[],[x1 y1],[],[], model, leftI); 
%             plotPoints(I2,[],[],[x2 y2],sift_track_point,trackPoint, model, rightI); 
            
            clear tempPoints fastPoints siftPoints goodKeyPoints1 goodKeyPoints2
            
        end
    end
    
    
    
end


disp('nearest sift');

total_NRSIFT=(NRSIFT/count)*100;

disp(total_NRSIFT);

disp('motion estimation');


total_MOTEST=(MOTEST/count)*100;

disp(total_MOTEST);