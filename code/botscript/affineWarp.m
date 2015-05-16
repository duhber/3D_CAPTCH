function [ I2 ] =affineWarp( I1,M )
%UNTITLED4 Summary of this function goes here
% warp I1 in I2 using H whwere I1=H*I2
%   Detailed explanation goes here
  I2=I1;
  I1_r=I1(:,:,1);
  I1_g=I1(:,:,2);
  I1_b=I1(:,:,3);
  H=size(I1_g,1);
  W=size(I1_b,2);
  P2=ones(3,numel(I1_r));
  for i=1:H
      P2(1,(i-1)*W+1:i*W)=1:W;
      P2(2,(i-1)*W+1:i*W)=i;
  end
% size(I1)
  P1=M*P2;
  P1(1,:)=P1(1,:)./P1(3,:);
  P1(2,:)=P1(2,:)./P1(3,:);
  P1(3,:)=P1(3,:)./P1(3,:);  
  I1_r=double(I1_r);
  val_r=interp2(1:W,1:H,I1_r, P1(1,1:numel(I1_r)),P1(2,1:numel(I1_r)));
%   numel(val)
  temp_r=reshape(val_r,W,H);
  
   I1_g=double(I1_g);
  val_g=interp2(1:W,1:H,I1_g, P1(1,1:numel(I1_g)),P1(2,1:numel(I1_g)));
%   numel(val)
  temp_g=reshape(val_g,W,H);
  
   I1_b=double(I1_b);
  val_b=interp2(1:W,1:H,I1_b, P1(1,1:numel(I1_b)),P1(2,1:numel(I1_b)));
%   numel(val)
  temp_b=reshape(val_b,W,H);
  
  I2(:,:,1)=temp_r';
  I2(:,:,2)=temp_g';
  I2(:,:,3)=temp_b';
end

