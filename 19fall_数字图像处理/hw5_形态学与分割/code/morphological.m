pre=[
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1],
    [1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1],
    [1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1],
    [1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1],
    [1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1],
    [1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
];
prep=pre;
init=[
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
];

[w,h] = size(pre);
img1 = translate(pre);
subplot(1,5,1);
imshow(img1);
title('original img');

% mat=init;
% for i=3:(w-3)
%     for j=3:(h-3)
%         if pre(i, j)==1 
%             mat(i,j)=1; mat(i,j+1)=1; mat(i,j+2)=1;
%         end
%     end
% end
% img2 = translate(mat);
% subplot(2,5,2);
% imshow(img2);
% title('(a)');
% % ---------- problem a ---------- %
% 
% mat=init;
% for i=3:(w-3)
%     for j=3:(h-3)
%         if pre(i, j)==1 && pre(i, j+1)==1 && pre(i, j+2)==1 
%             mat(i,j)=1;
%         end
%     end
% end
% img2 = translate(mat);
% subplot(2,5,3);
% imshow(img2);
% title('(b)');
% % ---------- problem b ---------- %
% 
% mat=init;
% for i=3:(w-3)
%     for j=3:(h-3)
%         if pre(i, j)==1
%             mat(i,j)=1; mat(i, j-1)=1; mat(i+1, j)=1;
%         end
%     end
% end
% img3 = translate(mat);
% subplot(2,5,4);
% imshow(img3);
% title('(c)');
% % ---------- problem c ---------- %
% 
% mat=init;
% for i=3:(w-3)
%     for j=3:(h-3)
%         if pre(i,j)==1 && pre(i, j-1)==1 && pre(i+1, j)==1
%             mat(i,j)=1;
%         end
%     end
% end
% img4 = translate(mat);
% subplot(2,5,5);
% imshow(img4);
% title('(d)');
% % ---------- problem d ---------- %

pre=prep;
mat=init;
for i=3:(w-3)
    for j=3:(h-3)
        if pre(i, j)==1 && pre(i, j+1)==1 && pre(i, j+2)==1 
            mat(i,j)=1;
        end
    end
end
pre=mat;
mat=init;
for i=3:(w-3)
    for j=3:(h-3)
        if pre(i, j)==1 
            mat(i,j)=1; mat(i,j+1)=1; mat(i,j+2)=1;
        end
    end
end
img2 = translate(mat);
subplot(1,5,2);
imshow(img2);
title('(e1)');

% ---------- problem e1 ---------- %

pre=prep;
mat=init;
for i=3:(w-3)
    for j=3:(h-3)
        if pre(i,j)==1 && pre(i, j-1)==1 && pre(i+1, j)==1
            mat(i,j)=1;
        end
    end
end
pre=mat;
mat=init;
for i=3:(w-3)
    for j=3:(h-3)
        if pre(i, j)==1
            mat(i,j)=1; mat(i, j-1)=1; mat(i+1, j)=1;
        end
    end
end
img4 = translate(mat);
subplot(1,5,3);
imshow(img4);
title('(e2)');

% ---------- problem e2 ---------- %

pre=prep;
mat=init;
for i=3:(w-3)
    for j=3:(h-3)
        if pre(i, j)==1 
            mat(i,j)=1; mat(i,j+1)=1; mat(i,j+2)=1;
        end
    end
end
pre=mat;
mat=init;
for i=3:(w-3)
    for j=3:(h-3)
        if pre(i, j)==1 && pre(i, j+1)==1 && pre(i, j+2)==1 
            mat(i,j)=1;
        end
    end
end
img2 = translate(mat);
subplot(1,5,4);
imshow(img2);
title('(f1)');

% ---------- problem f1 ---------- %

pre=prep;
mat=init;
for i=3:(w-3)
    for j=3:(h-3)
        if pre(i, j)==1
            mat(i,j)=1; mat(i, j-1)=1; mat(i+1, j)=1;
        end
    end
end
pre=mat;
mat=init;
for i=3:(w-3)
    for j=3:(h-3)
        if pre(i,j)==1 && pre(i, j-1)==1 && pre(i+1, j)==1
            mat(i,j)=1;
        end
    end
end
img4 = translate(mat);
subplot(1,5,5);
imshow(img4);
title('(f2)');

% ---------- problem f2 ---------- %

function result = translate(img)
    [w,h] = size(img);
    L=40;
    result = zeros((w-4)*L, (h-4)*L);
    for i=3:(w-2)
        for j=3:(h-2)
            result((i-3)*L+1:(i-2)*L, (j-3)*L+1:(j-2)*L)=xor(img(i,j),1);
        end
    end
end