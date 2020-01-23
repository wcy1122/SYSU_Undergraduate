img1 = imread('blobz1.png');
img2 = imread('blobz2.png');
res1 = segment1(img1);
res2 = segment1(img2);

subplot(2,2,1);
imshow(img1);
title('blobz1.png');
subplot(2,2,2);
imshow(res1);
title('sgmentation img');
subplot(2,2,3);
imshow(img2);
title('blobz2.png');
subplot(2,2,4);
imshow(res2);
title('sgmentation img');

function result = segment2(img)
    [w,h] = size(img);
    result = zeros(w,h);
    hist = imhist(img);
    sum_pix = sum(sum(img));
    all = w*h;
    count = 0;
    sump = 0;
    T = 0;
    maxg = 0;
    for i = 1:256
        count = count+hist(i);
        sump = sump+(i-1)*hist(i);
        if all==count || all==0
            continue
        end
        w0 = count/all;
        w1 = (all-count)/all;
        u0 = sump/count;
        u1 = (sum_pix-sump)/(all-count);
        g = w0*w1*(u0-u1)^2;
        if g>maxg
            maxg=g; T=i-1;
        end
    end
    for i=1:w
        for j=1:h
            if img(i,j)<=T
                result(i,j)=0;
            else
                result(i,j)=255;
            end
        end
    end
end


function result = segment1(img)
    [w,h] = size(img);
    result = zeros(w,h);
    hist = imhist(img);
    T=128;
    T0=0;
    sum_all = zeros(256);
    count = zeros(256);
    sum_all(1) = 0;
    count(1) = hist(1);
    for i = 2:256
        count(i) = count(i-1)+hist(i);
        sum_all(i) = sum_all(i-1)+hist(i)*(i-1);
    end
    while true
       u1 = sum_all(T) / count(T);
       u2 = (sum_all(255)-sum_all(T)) / (count(255)-count(T));
       newT = round((u1+u2)/2);
       if abs(newT-T)<=T0
           break
       end
       T=newT;
    end
    T
    for i=1:w
        for j=1:h
            if img(i,j)<T
                result(i,j)=0;
            else
                result(i,j)=255;
            end
        end
    end
end
