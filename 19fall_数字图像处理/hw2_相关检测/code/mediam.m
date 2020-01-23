img = imread('sport car.pgm');
subplot(2,2,1);
imshow(img);
title('initial');

img = salt_noisy(img);
%img = imnoise(img,'salt & pepper');
subplot(2,2,2);
imshow(img);
title('salt noisy');

img1 = my_medfilt2(img);
subplot(2,2,3);
imshow(img1);
title('my medfilt');

img2 = medfilt2(img,[3,3]);
subplot(2,2,4);
imshow(img2);
title('matlab medfilt');

imwrite(img1,'median_filter.png');

function result = salt_noisy(img)
    [w,h] = size(img);
    T1 = rand(w,h)*255;
    T2 = rand(w,h)*255;
    for x = 1:w
        for y = 1:h
            if img(x,y)>T1(x,y)
                img(x,y) = 255;
            elseif img(x,y)<T2(x,y)
                img(x,y) = 0;
            end
        end
    end
    result = img;
end

function result = my_medfilt2(img)
    [w,h] = size(img);
    img_map = zeros(w+2, h+2);
    img_map(2:w+1, 2:h+1) = img;
    for x = 2:w+1
        for y = 2:h+1
            tmp = reshape(img_map(x-1:x+1, y-1:y+1), 9, []);
            img(x-1, y-1) = median(tmp);
        end
    end
    result = img;
end