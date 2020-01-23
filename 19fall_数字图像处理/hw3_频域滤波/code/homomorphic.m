img = imread('office.jpg');
img = rgb2gray(img);
img = im2double(img);
[res_img, fft_img, filt_img, kernal] = homomorphic(img, 200);
% filter
res_img = transform(res_img);

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

function pad_img = padding(img)
    [width, height] = size(img);
    pad_img = zeros(width*2, height*2);
    pad_img(1:width, 1:height) = img;
end

function img = Ipadding(pad_img)
    [width, height] = size(pad_img);
    img = pad_img(1:width/2, 1:height/2);
end

function [res_img, fft_img, filt_img, kernal] = homomorphic(img, D0)
    img = padding(img);
    % padding
    [width, height] = size(img);
    img = log(img + 1);
    % log
    img = center(img);
    % centerlize
    fft_img = fft2(img);
    % DFT
    gamma_H = 2;
    gamma_L = 0.25;
    C = 1;
    kernal = zeros(width, height);
    for x = 1:width
        for y = 1:height
            D = sqrt((x-width/2)^2+(y-height/2)^2);
            kernal(x,y) = (gamma_H-gamma_L)*(1-exp(-C*((D^2)/(D0^2))))+gamma_L;
			% homomorphic
            %kernal(x,y) = 1/(1+(D0/D)^2);
			% butterworth
        end
    end
    filt_img = fft_img .* kernal;
    % homomorphic filtering
    res_img = real(ifft2(filt_img));
    % IDFT
    res_img = center(res_img);
    % Inv Centerlize
    res_img = exp(res_img)-1;
    % Exp
    res_img = Ipadding(res_img);
    % Inv padding
end
