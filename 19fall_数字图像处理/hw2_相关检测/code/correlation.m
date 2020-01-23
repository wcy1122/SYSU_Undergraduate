img1 = imread('car.png');
img2 = imread('wheel.png');
score_map = filt(img1, img2);

subplot(2,2,1);
imshow(img1);
title('car');
subplot(2,2,2);
imshow(img2);
title('wheel');
subplot(2,2,3);
imshow(score_map);
title('score map');
imwrite(score_map,'score_map.png');

pos = get_detect_pos(score_map, img2);
% ªÒ»°Œª÷√
subplot(2,2,4);
img1 = img1(:,:,[1 1 1]);
imshow(img1);
hold on;

x = pos(1,:); y = pos(2,:);
[w,h] = size(img2);
[n,m] = size(img1);
W = floor(w/2); H = floor(h/2);
for i = 1:length(x)
    x1 = max(x(i)-W,1); x2 = min(x(i)+W,n);
    y1 = max(y(i)-H,1); y2 = min(y(i)+H,m);
    rectx = [x1 x2 x2 x1 x1];
    recty = [y1 y1 y2 y2 y1];
    plot(recty, rectx, 'linewidth',2);
end
title('bounding box');
% ª≠øÚ

function result = get_detect_pos(score_map, mask)
    [w,h] = size(mask);
    [n,m] = size(score_map);
    W = floor(w/2); H = floor(h/2);
    max_val = max(score_map(:));
    res_x = []; res_y=[];
    while 1
        val=max(score_map(:));
        if val<max_val*0.85
            break
        end
        [x,y] = find(score_map==val);
        x=x(1); y=y(1);
        res_x = [res_x, x]; res_y = [res_y, y];
        score_map(max(x-W,1):min(x+W,n), max(y-H,1):min(y+H,m)) = 0;
    end
    result = [res_x;res_y];
end

function result = filt(img, mask)
    [w1,h1] = size(img);
    [w2,h2] = size(mask);
    W = floor(w2/2); H = floor(h2/2);
    img_map = zeros(w1+w2, h1+h2);
    img_map(W+1:W+w1, H+1:H+h1) = img;
    G = zeros(w1,h1);
    for x = W+1:W+w1
        for y = H+1:H+h1
            sum1 = 0; sum2 = 0; sum3=0;
            for u = -W:W
                for v = -H:H
                    sum1 = sum1+img_map(x+u,y+v)*double(mask(u+W+1,v+H+1));
                    sum2 = sum2+img_map(x+u,y+v)^2;
                    sum3 = sum3+double(mask(u+W+1,v+H+1))^2;
                end
            end
            G(x-W,y-H) = sum1/(sqrt(sum2*sum3));
        end
    end
    
    result = im2uint8(mat2gray(G));
end