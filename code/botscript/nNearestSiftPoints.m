function [p1,p2] = nNearestSiftPoints( sp, des1,locs1,des2, locs2,N)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

% this function finds the N nearest sift points of a FAST point on the left 
% image and their  correspondence on the right image

    p1=[];
    p2=[];
    
    if size(des2,1)==0 
        return
    end   
    
    if size(des1,1)==0 
        return
    end   
    
    dist= ((sp(1)-locs1(:,2)).^2 + (sp(2)-locs1(:,1)).^2).^0.5;
    
    [sortDist, idx]=sort(dist);
    
    n=size(locs1,1);
    count=0;
    for i=1:n
       
        matchindx=findMatch(des1,idx(i),des2);
        
        if matchindx ~=0
            count=count+1;
            p1(count,:)=[locs1(idx(i),2) locs1(idx(i),1)];
            p2(count,:)=[locs2(matchindx,2) locs2(matchindx,1)];
        end
        
        if count==N
            break
        end
        
    end


end

