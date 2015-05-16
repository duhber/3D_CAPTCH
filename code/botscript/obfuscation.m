function [ I2 ] = obfuscation(x1,y1,I1,I2  )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

    patch=I1(max(1,y1-35):min(y1+35,400),max(1,x1-35):min(x1+35,600),:);
    
    I2(1:size(patch,1),1:size(patch,2),:)=patch;

end

