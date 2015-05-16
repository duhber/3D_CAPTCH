function [ idx ] =selectBestPoint(siftPoint, fastPoint)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

    n=size(fastPoint,1);

    distvec=zeros(n,1);

    for i=1:n
        x1=fastPoint(i,1);
        y1=fastPoint(i,2);
    
        dist= ((x1-siftPoint(:,1)).^2 + (y1-siftPoint(:,2)).^2).^0.5;
        temp=sort(dist);
        distvec(i,1)=temp(1);
        
        
    
    end
    
    [val, index]=sort(distvec);
    idx=index(n);
    
end

