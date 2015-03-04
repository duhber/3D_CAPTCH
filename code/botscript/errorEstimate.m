function [  ] = errorEstimate( error,str )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
    
    h=hist(error,25);
    h=h';
    filename=strcat('../result/',str,'_TEST_ERROR_',datestr(now,'dd_mmmm_yyyy'));
    dlmwrite(filename,h,'delimiter','\t');
    f=figure;
    hist(error,25);
    saveas(f,filename,'jpg');
end

