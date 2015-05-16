function [ patchList ] = getPatch(I, x, y,wind)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
    H=size(I,1);
    W=size(I,2);
    t=floor(wind/2);
    h=2*t+1;
    w=2*t+1;
    patch=zeros(h,w,3);
    temp=I(max(1,y-t):min(H,y+t),max(1,x-t):min(W,x+t),:) ;
    patch(1:size(temp,1),1:size(temp,2),:)=temp;
    
    patchList=zeros(21,21,3,9);
    
    pindx=1;
            
    for yy=1:21:63
        for xx=1:21:63
            patchList(:,:,:,pindx)=patch(yy:yy+20,xx:xx+20,:);
            pindx=pindx+1;
        end
    end

end

