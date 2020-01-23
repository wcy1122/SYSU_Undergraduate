img=imread('river.JPG');
hist=imhist(img);

img1=get_histeq(img);
% 直方图
img2=histeq(img,256);
% 调库

subplot(2,3,1);
imshow(img);
title('原图像');
subplot(2,3,4);
imhist(img);
title('原直方图');

subplot(2,3,2);
imshow(img1);
title('均衡化图像');
subplot(2,3,5);
imhist(img1);
title('均衡化直方图');

subplot(2,3,3);
imshow(img2);
title('调库均衡化图像');
subplot(2,3,6);
imhist(img2);
title('调库直方图');

imwrite(img1,'river_histeq.png');

function result=get_histeq(img)
    hist=imhist(img);
    [dn,dm]=size(img);
    L=length(hist);
    A0=sum(hist);
    
    f=zeros(1,L);
    for i = 1:L
        f(i)=uint8(round((L-1)/A0*sum(hist(1:i))));
    end
    
    img1=img;
    for x = 1:dn
        for y = 1:dm
            val=int16(img(x,y))+1;
            img1(x,y)=f(val);
        end
    end
    result=img1;
end
% 直方图均衡



