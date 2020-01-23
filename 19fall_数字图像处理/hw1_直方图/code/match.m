img1=imread('EightAM.png');
img2=imread('LENA.png');

hist=imhist(img2);
match1=get_match(img1,hist);
match2=histeq(img1,hist);

subplot(2,4,1);
imshow(img1);
title('ԭͼ��');
subplot(2,4,5);
imhist(img1);
title('ԭͼ��ֱ��ͼ');

subplot(2,4,2);
imshow(img2);
title('ģ��ͼ��');
subplot(2,4,6);
imhist(img2);
title('ģ��ͼ��ֱ��ͼ');

subplot(2,4,3);
imshow(match1);
title('ƥ��ͼ��');
subplot(2,4,7);
imhist(match1);
title('ƥ��ͼ��ֱ��ͼ');

subplot(2,4,4);
imshow(match2);
title('����ͼ��');
subplot(2,4,8);
imhist(match2);
title('����ͼ��ֱ��ͼ');

imwrite(match1,'EightAM_match.png');

function result=get_match(img,hist2)
    [dn,dm]=size(img);
    hist1=imhist(img);
    L=length(hist1); 
    map=zeros(1,L);
    
    f=get_map(hist1);
    g=get_map(hist2);
    
    for i = 1:L
        pos=1;
        for j = 1:L
            if abs(f(i)-g(j))<abs(f(i)-g(pos))
                pos=j;
            end
        end
        map(i)=pos;
    end
    
    img1=img;
    for x = 1:dn
        for y = 1:dm
            val=int16(img(x,y))+1;
            img1(x,y)=map(val);
        end
    end
    result=img1;
end

function result=get_map(hist)
    L=length(hist); 
    A0=sum(hist);
    f=zeros(1,L);
    for i = 1:L
        f(i)=uint8(round((L-1)/A0*sum(hist(1:i))));
    end
    result=f;
end
% ӳ�亯��