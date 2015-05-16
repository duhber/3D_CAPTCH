function [ I1 ] = occlude( x,y,I1,I2 )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
            h=size(I2,1);
            w=size(I2,2);
            y2=round(h/2);
            x2=round(w/2);
%             x2
%             y2
%             max(1,y2-25)
%             min(y2+25,h)
%             max(1,x2-25)
%             min(x2+25,w)
            patch=I2(max(1,y2-25):min(y2+25,h),max(1,x2-25):min(x2+25,w),:);
            
            wind=I1(max(1,y-25):min(y+25,400),max(1,x-25):min(x+25,600),:);
            size(wind)
            I1(max(1,y-25):min(y+25,400),max(1,x-25):min(x+25,600),:)=patch(1:size(wind,1),1:size(wind,2),:);%uint8(zeros(size(wind)));%
            
            

end

