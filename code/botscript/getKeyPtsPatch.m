function [ patchList ] = getKeyPtsPatch( I,points,wind )
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
    t=floor(wind/2);
    
    H=size(I,1);
    W=size(I,2);

    patchList=zeros(wind,wind,3,size(points,1));
    for i=1:size(points,1)
        x=round(points(i,1));
        y=round(points(i,2));
        patch=I(max(1,y-t):min(H,y+t-1),max(1,x-t):min(W,x+t-1),:) ;
        
        h=size(patch,1);
        w=size(patch,2);
        
        patchList(1:h,1:w,:,i)=patch;
        
    end
end

