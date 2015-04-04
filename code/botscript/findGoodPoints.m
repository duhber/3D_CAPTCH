function [Q] =findGoodPoints( P1,P2,des1,locs1,des2,locs2 )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
n=size(P1,1);
count=0;
Q=[];
for i=1:n
    x1=P1(i,1);
    y1=P1(i,2);
    x2=P2(i,1);
    y2=P2(i,2);
    point=nearestSIFTmethod([x1 y1],des1,locs1,des2,locs2);
    
    if numel(point)~=0
               
        d=((x2-point(1))^2+(y2-point(2))^2)^0.5;
                
        if d>25.0
        
            count=count+1;
            Q(count)=i;
        end
                
    end
    
end

end

