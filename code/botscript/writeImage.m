function [ ] = writeImage( Im1,Im2, P, id,method )
%UNTITLED3 plot a points to given images
%   Detailed explanation goes here
    filename='../frame/%d/%s_%s';
    fname1=sprintf(filename,id,method,'left');
    fname2=sprintf(filename,id,method,'right');
    
    f1=figure;
    imshow(Im1);
    hold on
    plot(P(1,:), P(2,:), 'ro');
    saveas(f1,fname1,'jpg');

    close
    
    f2=figure;
    imshow(Im2);
    hold on
    plot(P(3,:), P(4,:), 'ro');
    saveas(f2,fname2,'jpg');
    
    close
end

