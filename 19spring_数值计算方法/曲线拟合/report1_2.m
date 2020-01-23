format long

f1=@(x,w)w(3)*x.^2+w(2)*x+w(1);
%Loss=@(w)sum((f1(x,w)-y).^2);
%Loss=@(w)sum(abs(f1(x,w)-y));
Loss=@(w)max(abs(f1(x,w)-y));

x=[1 2 3 4];
y=[1991 1992 2004 2009];
w0=rand(3,1);

[ans,Loss]=fmincon(Loss,w0,[],[],[],[],[],[],@limit);
%[ans,Loss]=fmincon(L2_loss,w0,[],[]);
f5=f1(5,ans);
f6=f1(6,ans);
disp(['Loss=',num2str(Loss)])
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
  fx=@(x,w)w(3)*x.^2+w(2)*x+w(1);
  c=2019-fx(5,w);
  ceq=[];
end

%ans=fmincon(loss,w0,[],[])