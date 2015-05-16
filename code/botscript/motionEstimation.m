function [ point ] = motionEstimation(sp,des1,locs1,des2,locs2 )
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here
    point=[];
    [Motion_est, N,inliers]=homography2(des2,locs2,des1,locs1,0.6);
    x1=sp(1);
    y1=sp(2);
    
     if(N>=4 && inliers>=4)
        point_track=Motion_est*[x1;y1;1];
    
        point_track=point_track./point_track(3);
    
        x_track=point_track(1);
        y_track=point_track(2);
                
        point=[x_track y_track];
     end
     
end

