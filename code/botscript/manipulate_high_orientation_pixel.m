clc;
clear all;

I1=imread('1269_left.jpg');

I=(imread('1269_right.jpg'));
I2=I;

H=size(I,1);
W=size(I,2);

[image, des, locs]=sift(uint8(I));
nkeys=size(locs,1);

Ig=rgb2gray(I);

[Gx, Gy] = imgradientxy(Ig);
[Gmag, Gdir] = imgradient(Gx, Gy);

nbins=8;
bin=[45.0 90.0 135.0 180.0 -45.0 -90.0 -135.0 -180.0];

wind=16;
t=wind/2;

for i=1:nkeys
    x=round(locs(i,2));
    y=round(locs(i,1));
    window=I(max(1,y-t):min(y+t-1,H),max(1,x-t):min(x+t-1,W),:);
    
    windir=Gdir(max(1,y-t):min(y+t-1,H),max(1,x-t):min(x+t-1,W));
    winmag=Gmag(max(1,y-t):min(y+t-1,H),max(1,x-t):min(x+t-1,W));
    
    h=size(window,1);
    w=size(window,2);
    
    nh=ceil(h/4);
    nw=ceil(w/4);
    
    for j=1:nh
        for k=1:nw
            
            smallwindir=windir((j-1)*4+1:min(h,j*4),(k-1)*4+1:min(w,k*4));
            smallwinmag=winmag((j-1)*4+1:min(h,j*4),(k-1)*4+1:min(w,k*4));
            smallwindow=window((j-1)*4+1:min(h,j*4),(k-1)*4+1:min(w,k*4),:);
            
            
            hh=size(smallwindir,1);
            ww=size(smallwindir,2);
%             finding the orientation histogram
            histo=zeros(1,8);
            for idx=1:nbins
                if bin(idx) > 0.0
                    lowerbound=bin(idx)-45.0;
                    upperbound=bin(idx);
                else
                    lowerbound=bin(idx);
                    upperbound=bin(idx)+45.0;
                end
                wlogical= ( smallwindir>=lowerbound & smallwindir<upperbound );
                result  = double(wlogical).*smallwinmag;
                
                histo(idx)=sum(result(:));
            end
            
            
            [val ,maxidx]=sort(histo);
            
            for p=nbins:-1:nbins-3
                
                if bin(maxidx(p)) > 0.0
                    lowerbound=bin(maxidx(p))-45.0;
                    upperbound=bin(maxidx(p));
                else
                    lowerbound=bin(maxidx(p));
                    upperbound=bin(maxidx(p))+45.0;
                end 
               
                wlogical= ( smallwindir>=lowerbound & smallwindir<upperbound );
            
                res = find(wlogical==1);
                
                yy = mod(res-1,hh)+1;
                xx = ceil(res./hh);
%                 yy
%                 xx
                for z=1:numel(res)
                    smallwindow(yy(z),xx(z),:)=uint8(randn(1,1,3)*256)+smallwindow(yy(z),xx(z),:);
                end
                  
                   
            end
             window((j-1)*4+1:min(h,j*4),(k-1)*4+1:min(w,k*4),:)=smallwindow;  
        end
    end
    I2(max(1,y-t):min(y+t-1,H),max(1,x-t):min(x+t-1,W),:)=window;
    
    
    
end

imshow(I2);
