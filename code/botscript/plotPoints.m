function [  ] = plotPoints( image, fastp, siftp,point,p1,p2,id,str )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    filename=sprintf('../frame/%d/%s',id,str);
    
    
    f1=figure;
    imshow(image);
    hold on
%     plot(fastp(1,:), fastp(2,:), 'rx');
%     plot(siftp(1,:), siftp(2,:), 'gx');
    if numel(point)~=0
      plot(point(1), point(2), 'bx');
    
    end
    
    if numel(p1) ~=0
        plot(p1(1),p1(2),'gx');
    end
    
    if numel(p2) ~=0
        plot(p2(1),p2(2),'rx');
    end   
    
    saveas(f1,filename,'jpg');
    
    close
end

