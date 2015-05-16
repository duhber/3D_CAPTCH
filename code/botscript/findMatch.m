function [ matchindx ] = findMatch( des1,indx, des2)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
%  x=locs1(indx,2);
%  y=locs1(indx,1);
 distRatio = 1.5;   
 des2t=des2';
 dotprods = des1(indx,:) * des2t;        % Computes vector of dot products
%  size(dotprods)
 [vals,index] = sort(acos(dotprods));  % Take inverse cosine and sort results
%  vals
   if (vals(1) < distRatio * vals(2))
      matchindx = index(1);
   else
      matchindx= 0;
   end 
%     if(numel(index)==0)
%         matchindx=0;
%     else
%         matchindx=index(1);
%     end
end

