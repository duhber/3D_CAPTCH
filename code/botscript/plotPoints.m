function [  ] = plotPoints( image, fastp, siftp,point,p1,p2,id )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    filename=sprintf('../frame/%d/Image_salient_points',id);
    
    
    f1=figure;
    imshow(image);
    hold on
%     plot(fastp(1,:), fastp(2,:), 'rx');
    plot(siftp(1,:), siftp(2,:), 'gx');
    plot(point(1), point(2), 'bo');
    if numel(p1) ~=0
        plot(p1(1),p1(2),'ro');
    end
    
    if numel(p2) ~=0
        plot(p2(1),p2(2),'rx');
    end   
    
    saveas(f1,filename,'jpg');
    
    close
end

