function [per] = analyseSiftPoints(x,y,r,locs)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
    count=0;
    
    for i=1:size(locs,1)
        dist=((x-locs(i,2))^2+(y-locs(i,1))^2)^0.5;
        if dist<=r
            count=count+1;
        end
    end
    
    per=(count/size(locs,1))*100;

end

