img = imread('book_cover.jpg');
vag_img = Blur(img);
vag_noisy_img = Gaussian_Noise(vag_img);

% inv_vag_img = Inverse(vag_img);
% inv_vag_noisy_img = Inverse(vag_noisy_img);
% 
% win_vag_img = Wiener(vag_img, 0);
% win_vag_noisy_img = Wiener(vag_noisy_img, 0.01);

img1 = Wiener(vag_noisy_img, 0);
img2 = Wiener(vag_noisy_img, 0.01);
img3 = Wiener(vag_noisy_img, 0.1);
img4 = Wiener(vag_noisy_img, 0.5);

subplot(2,3,1);
imshow(img);
title('image');
% subplot(2,3,2);
% imshow(vag_img);
% title('blurred img');

subplot(2,3,3);
imshow(img1);
title('Winner K=0');
subplot(2,3,4);
imshow(img2);
title('Winner K=1e-2');
subplot(2,3,5);
imshow(img3);
title('Winner K=1e-1');
subplot(2,3,6);
imshow(img4);
title('Winner K=0.5');

subplot(2,3,2);
imshow(noisy_img);
title('noisy img');

% subplot(1,3,3);
% imshow(inv_vag_img);
% title('inv vag');
% subplot(1,3,3);
% imshow(inv_vag_noisy_img);
% title('inv vag noisy');

% subplot(3,3,7);
% imshow(win_vag_img);
% title('win vag');
% subplot(3,3,8);
% imshow(win_vag_noisy_img);
% title('win vag noisy');

function result = Gaussian_Noise(img)
    [width, height] = size(img);
    H = 0 + sqrt(500) * randn(width, height);
    result = double(img) + H;
    result = uint8(result);
end
% 高斯噪声

function res_img = transform(img)
    [width, height] = size(img);
    res_img = zeros(width, height);
    max_val = max(img(:));
    min_val = min(img(:));
    range = max_val-min_val;
    for x = 1:width
        for y = 1:height
            res_img(x,y) = 255*(img(x,y)-min_val)/range;
        end
    end
    res_img = uint8(res_img);
end
% 变换输出

function cent_img = center(img)
    [width, height] = size(img);
    [X, Y] = meshgrid(1:height, 1:width);
    cent_img = double(img).*(-1).^(X+Y);
end
% 中心化

function result = Mul(img, kernal)
    cent_img = center(img);
    fft_img = fft2(cent_img);
    filt_img = fft_img .* kernal;
    result = real(ifft2(filt_img));
    result = center(result);
end
% 卷积

function result = Wiener(img, K)
    [width, height] = size(img);
    H = Move_Kernal(width, height); % 模糊
    H2 = abs(H).^2;
    kernal = (H2 ./ (H2+K)) .* Inv(H,1e-2);
    result = Mul(img, kernal);
    result = transform(result);
end
% 维纳滤波

function result = Inverse(img)
    [width, height] = size(img);
    H = Move_Kernal(width, height);
    kernal = Inv(H,1e-2);
    result = Mul(img, kernal);
    result = transform(result);
end
% 逆滤波

function kernal = Inv(kernal, eps)
    [width, height] = size(kernal);
    for x = 1:width
        for y = 1:height
            val = kernal(x,y);
            if abs(val)<eps
                kernal(x,y) = 0;
            else
                kernal(x,y) = 1/val;
            end
        end
    end
end

function result = Blur(img)
    [width, height] = size(img);
    kernal = Move_Kernal(width, height);
    result = Mul(img, kernal);
    result = transform(result);
end
% 运动模糊

function kernal = Move_Kernal(width, height)
    kernal = zeros(width, height);
    a=0.1; b=0.1; T=1;
    W = (width+1)/2; H = (height+1)/2;
    for x = 1:width
        for y = 1:height
            tmp = pi*((x-W)*a+(y-H)*b);
            if tmp==0
                kernal(x,y) = T;
            else
                kernal(x,y) = T*sin(tmp)/tmp*exp(-1i*tmp);
            end
        end
    end
end
% 运动模糊核函数