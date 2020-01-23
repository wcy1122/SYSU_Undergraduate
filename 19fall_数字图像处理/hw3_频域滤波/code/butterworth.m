img = imread('barb.png');
img = im2double(img);
[res_img, fft_img, filt_img, kernal] = img_filter(img, 40, 1);
% filter

subplot(2,2,1);
imshow(img);
title('image');
subplot(2,2,2);
imshow(res_img);
title('result');
subplot(2,2,3);
imshow(log2(1+abs(fft_img)),[]);
title('fft img');
subplot(2,2,4);
imshow(log2(1+abs(filt_img)),[]);
title('filt img');

function cent_img = center(img)
    [width, height] = size(img);
    [X, Y] = meshgrid(1:height, 1:width);
    cent_img = img.*(-1).^(X+Y);
end

function pad_img = padding(img)
    [width, height] = size(img);
    pad_img = zeros(width*2, height*2);
    pad_img(1:width, 1:height) = img;
end

function img = Ipadding(pad_img)
    [width, height] = size(pad_img);
    img = pad_img(1:width/2, 1:height/2);
end

function [res_img, fft_img, filt_img, kernal] = img_filter(img, D0, n)
    img = padding(img);
    % Padding
    img = center(img);
    % Centerlize
    [width, height] = size(img);
    fft_img = fft2(img);
    % DFT
    kernal = zeros(width, height);
    for x = 1:width
        for y = 1:height
            D = sqrt((x-width/2)^2+(y-height/2)^2);
            kernal(x,y) = 1/(1+(D/D0)^(2*n));
        end
    end
    filt_img = fft_img .* kernal;
    % Filter
    res_img = real(ifft2(filt_img));
    % IDFT
    res_img = center(res_img);
    % Inv Centerlize
    res_img = Ipadding(res_img);
    % Inv padding
end
