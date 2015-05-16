function [ outim ] = orderedDith( im,method )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
outim=zeros(size(im));
if(method==1)
    outim(:,:,1)=screen_9c(im(:,:,1)./255);
    outim(:,:,2)=screen_9c(im(:,:,2)./255);
    outim(:,:,3)=screen_9c(im(:,:,3)./255);
elseif(method==2)
    outim(:,:,1)=screen_9u(im(:,:,1)./255);
    outim(:,:,2)=screen_9u(im(:,:,2)./255);
    outim(:,:,3)=screen_9u(im(:,:,3)./255);
elseif(method==3)
    outim(:,:,1)=screen_16u(im(:,:,1)./255);
    outim(:,:,2)=screen_16u(im(:,:,2)./255);
    outim(:,:,3)=screen_16u(im(:,:,3)./255);
elseif(method==4)
    outim(:,:,1)=screen_19c(im(:,:,1)./255);
    outim(:,:,2)=screen_19c(im(:,:,2)./255);
    outim(:,:,3)=screen_19c(im(:,:,3)./255);
elseif(method==5)
    outim(:,:,1)=screen_9c(im(:,:,1)./255);
    outim(:,:,2)=screen_9c(im(:,:,2)./255);
    outim(:,:,3)=screen_9c(im(:,:,3)./255);
elseif(method==6)
    masksize=[3 3];
    mask1=[0 1 0;1 1 1;0 1 0];
    mask0=1-mask1;
    outim(:,:,1)=bed(im(:,:,1),masksize,mask1,mask0,0);
    outim(:,:,2)=bed(im(:,:,2),masksize,mask1,mask0,0);
    outim(:,:,3)=bed(im(:,:,3),masksize,mask1,mask0,0);
elseif(method==7)
    masksize=[3 3];
    mask1=[1 0 1;0 1 0;1 0 1];
    mask0=1-mask1;
    outim(:,:,1)=bed(im(:,:,1),masksize,mask1,mask0,0);
    outim(:,:,2)=bed(im(:,:,2),masksize,mask1,mask0,0);
    outim(:,:,3)=bed(im(:,:,3),masksize,mask1,mask0,0);
elseif(method==8)
    masksize=[4 4];
    mask1=[0 1 1 0;1 1 1 1;1 1 1 1;0 1 1 0];
    mask0=1-mask1;
    outim(:,:,1)=bed(im(:,:,1),masksize,mask1,mask0,0);
    outim(:,:,2)=bed(im(:,:,2),masksize,mask1,mask0,0);
    outim(:,:,3)=bed(im(:,:,3),masksize,mask1,mask0,0);
elseif(method==9)
    masksize=[4 4];
    mask1=[1 0 0 1;0 1 1 0;0 1 1 0;1 0 0 1];
    mask0=1-mask1;
    outim(:,:,1)=bed(im(:,:,1),masksize,mask1,mask0,0);
    outim(:,:,2)=bed(im(:,:,2),masksize,mask1,mask0,0);
    outim(:,:,3)=bed(im(:,:,3),masksize,mask1,mask0,0);
elseif(method==10)
    masksize=[5 5];
    mask1=[0 0 1 0 0;0 0 1 0 0;1 1 1 1 1;0 0 1 0 0;0 0 1 0 0];
    mask0=1-mask1;
    outim(:,:,1)=bed(im(:,:,1),masksize,mask1,mask0,0);
    outim(:,:,2)=bed(im(:,:,2),masksize,mask1,mask0,0);
    outim(:,:,3)=bed(im(:,:,3),masksize,mask1,mask0,0);
elseif(method==11)
    masksize=[5 5];
    mask1=[1 0 0 0 1;0 1 0 1 0;0 0 1 0 0;0 1 0 1 0;1 0 0 0 1];
    mask0=1-mask1;
    outim(:,:,1)=bed(im(:,:,1),masksize,mask1,mask0,0);
    outim(:,:,2)=bed(im(:,:,2),masksize,mask1,mask0,0);
    outim(:,:,3)=bed(im(:,:,3),masksize,mask1,mask0,0);
end
end

