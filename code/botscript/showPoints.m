function Im=showPoints( image,color,x,y,l,p1,p2,pointmatrix)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
% size(image)
H=size(image,1);
W=size(image,2);
Im=image;
x=round(x);
maxel=max(Im(:));
y=round(y);

patch=image(max(1,y-l):min(H,y+l),max(1,x-l):min(W,x+l),:);
patch=patch*0;
if(strcmp(color,'red'))
    patch(:,:,1)=max(image(:));
elseif(strcmp(color,'green'))
    patch(:,:,2)=max(image(:));
else
    patch(:,:,3)=max(image(:));
end
Im(max(1,y-l):min(H,y+l),max(1,x-l):min(W,x+l),:)=patch;

if numel(p1)~=0
    x2=round(p1(1));
    y2=round(p1(2));
    patch1=image(max(1,y2-l):min(H,y2+l),max(1,x2-l):min(W,x2+l),:);

    patch1=patch1*0;

    patch1(:,:,2)=max(image(:));

    Im(max(1,y2-l):min(H,y2+l),max(1,x2-l):min(W,x2+l),:)=patch1;
end

if numel(p2)~=0

    x3=round(p2(1));
    y3=round(p2(2));
    
    patch2=image(max(1,y3-l):min(H,y3+l),max(1,x3-l):min(W,x3+l),:);

    patch2=patch2*0;

    patch2(:,:,3)=max(image(:));

    Im(max(1,y3-l):min(H,y3+l),max(1,x3-l):min(W,x3+l),:)=patch2;
end


if numel(pointmatrix)~=0
   
    for i=1:size(pointmatrix,1)
        xx=round(pointmatrix(i,1));
        yy=round(pointmatrix(i,2));
        patch3=image(max(1,yy-l):min(H,yy+l),max(1,xx-l):min(W,xx+l),:);
        patch3=ones(size(patch3))*maxel;
        patch3(:,:,1)=0;
        patch3(:,:,3)=0;
        Im(max(1,yy-l):min(H,yy+l),max(1,xx-l):min(W,xx+l),:)=patch3;
    end
    
end



t=25;
w=1;
square=uint8(zeros(2*t+1,2*t+1,3));
square(:,:,2)=maxel;
square(w+1:2*t+1-w,w+1:2*t+1-w,2)=0;
unmask=square;
unmask(:,:,:)=maxel;
unmask(w+1:2*t+1-w,w+1:2*t+1-w,:)=0;
mask=uint8(logical(unmask==0));

quad_1=Im(max(1,y-t):y,x:min(W,x+t),:);
quad_2=Im(max(1,y-t):y,max(1,x-t):x,:);
quad_3=Im(y:min(H,y+t),max(1,x-t):x,:);
quad_4=Im(y:min(H,y+t),x:min(W,x+t),:);
% class(quad_1)
% class(mask)
% class(square)
Im(max(1,y-t):y,x:min(W,x+t),:)=quad_1.*mask(t+1-size(quad_1,1)+1:t+1,t+1:t+size(quad_1,2),:)+square(t+1-size(quad_1,1)+1:t+1,t+1:t+size(quad_1,2),:);
Im(max(1,y-t):y,max(1,x-t):x,:)=quad_2.*mask(t+1-size(quad_2,1)+1:t+1,t+1-size(quad_2,2)+1:t+1,:)+square(t+1-size(quad_2,1)+1:t+1,t+1-size(quad_2,2)+1:t+1,:);
Im(y:min(H,y+t),max(1,x-t):x,:)=quad_3.*mask(t+1:t+size(quad_3,1),t+1-size(quad_3,2)+1:t+1,:)+square(t+1:t+size(quad_3,1),t+1-size(quad_3,2)+1:t+1,:);
Im(y:min(H,y+t),x:min(W,x+t),:)=quad_4.*mask(t+1:t+size(quad_4,1),t+1:t+size(quad_4,2),:)+square(t+1:t+size(quad_4,1),t+1:t+size(quad_4,2),:);
end

