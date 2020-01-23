format long
f1=@(x,w)w(3)*atan(w(1)*(x-w(2)))+w(4);
%Loss=@(w)sum((f1(x,w)-y).^2);
%Loss=@(w)sum(abs(f1(x,w)-y));
Loss=@(w)max(abs(f1(x,w)-y));

x=[1.0 2.0 3.0 4.0];
y=[1991.0 1992.0 2004.0 2009.0];
w0=[0.5 1 2 2000]

[ans,Loss]=fmincon(Loss,w0,[],[],[],[],[],[],@limit)
%[ans,Loss]=fmincon(L2_loss,w0,[],[])
f5=f1(5,ans);
f6=f1(6,ans);
disp(['L2_loss=',num2str(Loss)])
disp(['x=5,y=',num2str(f5)])
disp(['x=6,y=',num2str(f6)])

X=[0:0.01:7];
Y=f1(X,ans);
plot(X,Y)
hold on
plot(x,y,'x')
plot(5,f5,'o')
%text(5,f5,['P(5,' num2str(f5) ')'])
plot(6,f6,'o')
%text(6,f6,['P(6,' num2str(f6) ')'])

function [c,ceq]=limit(w)
  fx=@(x,w)w(3)*atan(w(1)*(x-w(2)))+w(4);
  c=2019-fx(3,w);
  ceq=[];
end

%ans=fmincon(loss,w0,[],[])