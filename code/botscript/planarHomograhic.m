function [ H ] = planarHomograhic(P1, P2)
%finds the planar homographic transformation matrix from P1 to P2
%   Detailed explanation goes here

N=2*size(P1,1);
A=zeros(N,9);
temp1=ones(N,3);
temp1(1:2:N,1:2)=P1(:,2:-1:1);
temp1(2:2:N,1:2)=P1(:,2:-1:1);
temp2=P2(:,2:-1:1);
temp2=temp2';
temp2=temp2(:);
A(1:2:N,1:3)=-1*temp1(1:2:N,:);
A(2:2:N,4:6)=-1*temp1(1:2:N,:);
for i=1:3
    A(:,6+i)=temp1(:,i).*temp2;
end
[U S V]=svd(A);
h=V(:,9);
H=transpose(reshape(h,3,3));
end

