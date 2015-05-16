function [ point ] = nearestSIFTmethod(sp, des1,locs1,des2, locs2 )
%   sp-> challenge point
%   I1-> left Image
%   I2-> right Image
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
    point=[];
    

    
    if size(des2,1)==0 
        return
    end   
    
    if size(des1,1)==0 
        return
    end   
    
    dist= ((sp(1)-locs1(:,2)).^2 + (sp(2)-locs1(:,1)).^2).^0.5;
    
    [sortDist, idx]=sort(dist);
    
    n=size(locs1,1);
    for i=1:n
       
        matchindx=findMatch(des1,idx(i),des2);
        
        if matchindx ~=0
            point=[locs2(matchindx,2) locs2(matchindx,1)];
            return
        end
        
    end

end

