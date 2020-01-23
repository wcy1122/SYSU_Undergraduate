%f=@(x)x.^2-2.*x+3;
%f=@(x)x.^3-3.*x.^2+2.*x+4;
%f=@(x)sin(x);
f=@(x)exp(x);
%title('f(x)=sin(x)');
n=6;
X=[-3:1:3];
%X=[-3,-1,0,0.5,2,2.1,3];
Y=f(X);
A=zeros(n,n);
B=zeros(n,1);
h=zeros(n,1);
%defination

for i=[1:n-1]
    A(i,i)=1;
    A(i,i+1)=1;
end
A(n,n)=3;
A(n,n-1)=-1;
%get A

for i=[1:n]
    h(i)=X(i+1)-X(i);
    B(i)=2.*(Y(i+1)-Y(i))./h(i);
end
M=inv(A)*B;
M=[M' 2*M(n)-M(n-1)];
%get B,M

for i=[1:n]
    a(i)=(M(i+1)-M(i))/2/h(i);
    b(i)=M(i);
    c(i)=Y(i);
end
%get a,b,c

hold on;
plot(X,Y,'x');
x=[-4:0.01:4];
y=f(x);
plot(x,y,'b');
Loss=0;sz=0;
for i=[1:n]
    x=[X(i):0.01:X(i+1)];sz=sz+length(x);
    y=a(i)*(x-X(i)).^2+b(i)*(x-X(i))+c(i);
    Loss=Loss+sum((y-f(x)).^2);
    plot(x,y,'m');
end
disp(['Inter_Loss=',num2str(Loss/sz)])
%inter

Loss=0;sz=0;
x=[X(1)-1:0.01:X(1)];sz=sz+length(x);
y=a(1)*(x-X(1)).^2+b(1)*(x-X(1))+c(1);
Loss=Loss+sum((y-f(x)).^2);
plot(x,y,'m');
x=[X(n+1):0.01:X(n+1)+1];sz=sz+length(x);
y=a(n)*(x-X(n)).^2+b(n)*(x-X(n))+c(n);
Loss=Loss+sum((y-f(x)).^2);
plot(x,y,'m');
disp(['Extra_Loss=',num2str(Loss/sz)])
%extra
legend('数据点','目标曲线','插值结果');