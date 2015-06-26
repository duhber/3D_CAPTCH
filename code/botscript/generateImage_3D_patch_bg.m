%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                           DATE: 23 May 2015 
%
%        Using near keypts patches and background texture in 3D
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
    numModel=495;
    
    modelDir='../frame/%d/frame_000%d.';
    
    r=25; %radius of toleramce
    
    Xdata=zeros(4, numModel);
    
    count=0;
    
    NRSIFT = 0;
    MOTEST = 0;
    
    leftTable=fopen('../image/left.sql','w');
    rightTable=fopen('../image/right.sql','w');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

for model = 1001:1000+numModel
    disp(model);
    
    for frame1=0:0
        
        f1=sprintf(strcat(modelDir,'jpg'),model,frame1);
        
        if exist(f1,'file')==0
            continue;
        end
        frame3=1;
        for frame2=2:2
            
            f2=sprintf(strcat(modelDir,'jpg'),model,frame2);
            f3=sprintf(strcat(modelDir,'jpg'),model,frame3);
            if exist(f2,'file')==0
                continue;
            end
      
            if exist(f3,'file')==0
                continue;
            end
            
            % load the frames and keypoints of I1 and I2
            
            I1=double(imread(f1));
            
            I2int=imread(f3);
            I2=double(imread(f2));
            
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
%             candidate points
            cpfilename=sprintf('../frame/%d/challengepoint',model);
            
            cp=importdata(cpfilename)+1;            
            
            x1=round(frameKeyPoints1(cp,1));
            y1=round(frameKeyPoints1(cp,2));
            
            x2=round(frameKeyPoints2(cp,1));
            y2=round(frameKeyPoints2(cp,2));
            
            if x2>600
                continue;
            end
            if y2>400
                continue;
            end
%             x2=x1;
%             y2=y1;
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            [image, des, locs]=sift(uint8(I2));
            nkeys=size(locs,1);
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                     generate an image with patch as background
            Izeros=(~I2int(:,:,1)) & (~I2int(:,:,2)) & (~I2int(:,:,3)) ;
            
            Imask(:,:,1)=double(Izeros(:,:));
            Imask(:,:,2)=double(Izeros(:,:));
            Imask(:,:,3)=double(Izeros(:,:));
            
            wind=32;    
            
            t=floor(wind/2);
            
            pointList=getNearPoints(I1,x1,y1,30);
            
            patchList=getKeyPtsPatch(I1,pointList,wind);
            
            size(patchList,4)
            size(pointList,1)
            
            Ip=zeros(H,W,3);
            
            contin=0;
            
            for i=1:size(patchList,4)
                j=1;
                moveon=0;
                while j<=1
                    qx=randi([t+1 W-t+1]); 
                    qy=randi([t+1 H-t+1]);
                    
                    patchTemp=Izeros(qy-t:qy+t-1,qx-t:qx+t-1);
                    bpixel=sum(patchTemp(:));
                    bper=(bpixel/(wind*wind))*100;
                    if bper>95
                        I2(qy-t:qy+t-1,qx-t:qx+t-1,:)=patchList(:,:,:,i);
                        Izeros(qy-t:qy+t-1,qx-t:qx+t-1)=zeros(size(patchTemp));
                        j=j+1;
                    else
                        moveon=moveon+1;
                    end
                    
                    if moveon>=100
                        break;
                    end
                    
                end
                
                if moveon>=100
                    contin=1;
                    break;
                end
            end 
            
            if contin==1
                continue;
            end
            
            I2=(I2./max(I2(:)));
            I1hack=I1;
            I1=I1./max(I1(:));
            %I2hack=I2;
           
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%             
            %%%%% compute the sift descriptor of the two frame %%%%%%%%%%%
%             hacking part
            
            I2hack=((I2-min(I2(:)))/(max(I2(:))-min(I2(:)))*255);
%             mask=fspecial('average',5);
%             I2=imfilter(I2,mask);
            [image1, des1, locs1]=sift(uint8(I1hack));
            
            
            [image2, des2, locs2]=sift(uint8(I2hack));
            
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

             
            Xdata(1:4,count)=[count;model;round(x2);round(y2)];
            % using nearest sift
            leftpoint=[];
            rightpoint=[];
            trackPoint=nearestSIFTmethod([x1 y1],des1,locs1,des2,locs2);
%             [leftpoint, rightpoint]=nNearestSiftPoints([x1 y1],des1,locs1,des2,locs2,10);
            
            
           
            
            if numel(trackPoint)~=0
               
                d=((x2-trackPoint(1))^2+(y2-trackPoint(2))^2)^0.5;
                
                if d<=r
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
                
                if d<=r
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
            
%             Ileft=showPoints(I1,'red',x1,y1,2,[],[],leftpoint);
%             Irite=showPoints(I2,'red',x2,y2,2,sift_track_point,trackPoint,rightpoint );

             Ileft=showPoints(I1,'red',x1,y1,2,[],[],[]);
%             Irite=showPoints(I2,'red',x2,y2,2,[],[],[]);
            Irite=I2;
            
            imwrite(Ileft,leftI,'jpg');
            imwrite(Irite,rightI,'jpg');
          
%             plotPoints(I1,[],[],[x1 y1],[],[], model, leftI); 
%             plotPoints(I2,[],[],[x2 y2],sift_track_point,trackPoint, model, rightI); 
            
            clear tempPoints fastPoints siftPoints goodKeyPoints1 goodKeyPoints2
            
            
            leftname=sprintf('''/tmp/%d_left.jpg''',model);
            rightname=sprintf('''/tmp/%d_right.jpg''',model);
            
            fprintf(leftTable,'INSERT INTO leftImageRepo VALUES (%d, LOAD_FILE(%s));\n',model,leftname);
            fprintf(rightTable,'INSERT INTO rightImageRepo VALUES (%d, LOAD_FILE(%s));\n',model,rightname);
            
        end
    end
    
    
    
end


disp('nearest sift');

total_NRSIFT=(NRSIFT/count)*100;

disp(total_NRSIFT);

disp('motion estimation');


total_MOTEST=(MOTEST/count)*100;

disp(total_MOTEST);
%save the point to tab separated valuestr


fileID = fopen('../image/groundTruth.sql','w');
fprintf(fileID,'INSERT INTO ground_truth VALUES (%d,%d,%d,%d);\n',Xdata(:,1:count));

fclose(fileID);
fclose(leftTable);
fclose(rightTable);
