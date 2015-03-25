function [  ] = plotPoints( image, fastp, siftp,id )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    filename=sprintf('../frame/%d/Image_salient_points',id);
    
    
    f1=figure;
    imshow(image);
    hold on
%     plot(fastp(1,:), fastp(2,:), 'rx');
    plot(siftp(1,:), siftp(2,:), 'gx');
    saveas(f1,filename,'jpg');
    
    close
end

