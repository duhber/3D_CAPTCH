%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                     Created on:   20 february 2015
%                     
%                     Finished on:   
%
%                SIFT descriptor matching test on 3d-CAPTCHA (SD)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%       
%                               3D-CAPTCHA
%   The Captcha consists of 2 frames taken from same 3D model but with
%   different viewpoints. 
%   Here, we assume that the hacker have the skills to estimate the sift   
%   descriptor accurately for each points selected on the left image.
%
%   We do this by parsing the sift keypoints and save in frame_0000.desc
%   file. each lines in .desc file is the sift descriptor of its
%   corresponding lines in .p file. 
%   
%   THIS SIFT DESCRIPTOR NEED TO BE NORMALIZE------------------
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

for model=1001:1006%1001:numModel+1000
    
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
            
            
            %%%%%% compute the sift descriptor of right frame %%%%%%%%%%%%%
            
            [image2, des2, locs2]=sift(I2);
                            
            numpoints=size(frameKeyPoints1,1);
          
            descFilename=sprintf(strcat(modelDir,'desc'),model,frame1);
                
            if exist(descFilename,'file')==0
                continue;
            end
                
            des1=importdata(descFilename,' ');
            
            
            
            %%%%%%%%%%%% normalize the sift descriptor %%%%%%%%%%%%%%%%%%%%
            
            sumSqrdDes1=(sum(des1.^2,2)).^0.5;
            
            ndes1=des1./(repmat(sumSqrdDes1,1,128));
            
            for pointI=1:numpoints
                
                x1=frameKeyPoints1(pointI,1);
                y1=frameKeyPoints1(pointI,2);
               
                x2=frameKeyPoints2(pointI,1);
                y2=frameKeyPoints2(pointI,2);
                
                
                if (x1>W||x1<0)||(y1>H||y1<0)||(x2>W||x2<0)||(y2>H||y2<0) 
                    continue;
                    
                end
                
                count=count+1;
                %%%%%%%%%%%% sift descriptor matching test %%%%%%%%%%%%%%%%
                
                
                matchindx=findMatch(ndes1,pointI,des2);
                
                x_truth=x2;
                y_truth=y2;
                
                if(matchindx~=0)
                    x_track=locs2(matchindx,2);
                    y_track=locs2(matchindx,1);
                    
                    if ((x_truth-r<=x_track && x_track<=x_truth+r) && (y_truth-r<=y_track && y_track<=y_truth+r))
                        %disp('motion estimation');
                        correct=correct+1;
                        errorstat(count)=0;
                    else
                        errorstat(count)=sqrt((x_track-x_truth)^2 + (y_track-y_truth)^2)-r;
                        if(errorstat(count) >=100)
                            errorstat(count)=100;
                        end
                    end
                else
                    errorstat(count)=100;
                end
                
                
            end
            
        end
    end
    
end

disp(count);
disp(correct);

percentage=(correct/count)*100;

disp(percentage);











