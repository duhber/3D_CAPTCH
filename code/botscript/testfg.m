im=imread('frame_0000.jpg');
f=figure;

p=[50 300;100 300;150 300;200 300; 250 300; 300 300;300 100;300 200;300 400];

imshow(im);
hold on
plot(p(:,1), p(:,2), 'gx');
saveas(f,'test','jpg');