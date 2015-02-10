function [ H, N,inliers ] = homography2( des1,loc1,des2,loc2,val )
%UNTITLED2 Summary of this function goes here
%   find H in  im1=H*im2
%   Detailed explanation goes here
%  I1=imread(im1);
%  I2=imread(im2);
[N ,P2, P1]=match_1(des2,loc2,des1,loc1,val);
if (N>=4)
    found=1;
    max=0;
    for i=1:1000
        Q1=P1;
        Q2=P2;
        indx=zeros(1,4);
        j=1;
        p1=zeros(4,2);
        p2=zeros(4,2);
        while(j<=4)
            temp=randi([1 N]);
            if(~sum(indx==temp))
                p1(j,:)=Q1(temp,:);
                p2(j,:)=Q2(temp,:);
%                 Q1(temp,:)=[];
%                 Q2(temp,:)=[];
                indx(j)=temp;
                j=j+1;
            end
%         i=i+1;
        end
        
        H21=planarHomograhic(p2,p1);  
        Q2xy=ones(size(Q2,1),3);
%         Q1xy=ones(size(Q1,1),3);
        Q2xy(:,1:2)=Q2(:,2:-1:1);
%         Q1xy(:,1:2)=Q1(:,2:-1:1);
    
        Q1_est=H21*Q2xy';
        Q1_est=transpose(Q1_est);
        Q1_est(:,1)=Q1_est(:,1)./Q1_est(:,3);
        Q1_est(:,2)=Q1_est(:,2)./Q1_est(:,3);
    
        error=Q1-Q1_est(:,2:-1:1);
        error=error.^2;
        count=0;
        for j=1:size(error,1);
            if(error(j,1)<0.1 && error(j,2)<0.1)
                count=count+1;
                temp_Q1(count,:)=Q1(j,:);
                temp_Q2(count,:)=Q2(j,:);
            end
        end
        if(count>max)
            max=count;
            Q1_inliers=temp_Q1;
            Q2_inliers=temp_Q2;
            clear temp_Q1 temp_Q2;
        end
%     i        
%     size(Q1_inliers)
%     size(Q2_inliers)
%     
   
    end
    inliers=max;
    if (max>3)
        H=planarHomograhic(Q2_inliers,Q1_inliers);
    else
         H=[1 0 0;0 1 0;0 0 1];
    end
else
    inliers=0;
    found=0;
    H=[1 0 0;0 1 0;0 0 1];
end

end

