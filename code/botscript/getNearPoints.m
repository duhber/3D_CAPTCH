function [ pointList ] = getNearPoints(I,x,y,d )
%UNTITLED6 Summary of this function goes here
%   Detailed explanation goes here
    [image, des, locs]=sift(uint8(I)); 
    
    pointList(1,1:2)=[x y];
    
    dist= ((x-locs(:,2)).^2 + (y-locs(:,1)).^2).^0.5;
    
    res=dist<=d;
    idx=find(res==1);
    n=numel(idx)+1;
    
    pointList(2:n,1:2)=[locs(idx,2) locs(idx,1)];

end

