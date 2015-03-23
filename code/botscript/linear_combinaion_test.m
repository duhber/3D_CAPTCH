%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                           DATE: 10 March 2015 
%
%                  estimate point using Linear combinations
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
    numModel=165;
    
    modelDir='../frame/%d/frame_000%d.';
    
    r=25; %radius of toleramce
    
    count=0;
    correct=0;
    
    errorcount=0;
    
    modelCount=0;
    modelCorrect=0;
    modelAccuracy=0;
   
    modelStat=zeros(numModel,4);
    save=1;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for model=1001:1000+numModel
    disp(model);
    modelCount=0;
    modelCorrect=0;
    
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
            count=modelCount+count;
            for pointI=1:modelCount   
                
                
                %%%%%%%%%%%% take n nearest sift keypoints %%%%%%%%%%%%%%%%
                
                x1=goodKeyPoints1(pointI,1);
                y1=goodKeyPoints1(pointI,2);
               
                x2=goodKeyPoints2(pointI,1);
                y2=goodKeyPoints2(pointI,2);

                
                
                distPointI= ((x1-locs1(:,2)).^2 + (y1-locs1(:,1)).^2).^0.5;
                
                [sortDist, idx]=sort(distPointI);
                n=min(5,size(idx,1))+1;% number of vectors in vspace
                
                vspace1=zeros(n,2);
                
                vspace1(1,:)=[x1 y1];
                
                for i=2:n
                    vspace1(i,:)=[locs1(idx(i-1),2) locs1(idx(i-1),1)];
                end
                
                V=vspace1';
                
%               solve for Va=0

                [u,s,v]=svd(V);
                
                a=v(:,size(v,2));
                
                sumV=[0 0];
                vspace2=zeros(size(vspace1));
               
                matchSiftPoint=0;
                for i=1:n-1
                    matchindx=findMatch(des1,idx(i),des2);
                    if matchindx ~=0
                        vspace2(i+1,:)=[locs2(matchindx,2) locs2(matchindx,1)];
                        matchSiftPoint=i+1;
                        break;
                    end
                end
                
%                 
%                 for i=2:n
%                     sumV=sumV+a(i)*vspace2(i,:);
%                 end
                
%                 sumV=sumV*(-1);
%                 vspace2(1,:)=sumV/a(1);
%                 
%                 
%                 if size(vspace2,1)<2
%                     continue
%                 end
                if matchSiftPoint ~=0
                    x_track=vspace2(matchSiftPoint,1);
                    y_track=vspace2(matchSiftPoint,2);
                else
                    continue
                end
                
                
                x_truth=x2;
                y_truth=y2;
                                    
                if ((x_truth-r<=x_track && x_track<=x_truth+r) && (y_truth-r<=y_track && y_track<=y_truth+r))
                    
                    correct=correct+1;
                    modelCorrect=modelCorrect+1;

                else
                    errorcount=errorcount+1;
                    errorstat(errorcount)=sqrt((x_track-x_truth)^2 + (y_track-y_truth)^2)-r;
                    if(errorstat(errorcount) >=100)
                        errorstat(errorcount)=100;
                    end
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

filename=strcat('../result/NEAREST_SIFT_test_result_',datestr(now,'dd_mmmm_yyyy'));

if save==1
    dlmwrite(filename,modelStat,'delimiter','\t');
    
%     errorEstimate(errorstat,'SIFT_LC');
end