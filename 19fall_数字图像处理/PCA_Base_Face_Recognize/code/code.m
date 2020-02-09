[train_x, train_y, test_x, test_y] = Load_Dataset('.\dataset');

mean_face = mean(train_x, 1);
train_x = train_x - mean_face;
[coeff, score, latent, tsquared, explained] = pca(train_x);
Show_Eigen_Face(coeff, 5, 5);

X = 1:100;
All_info = [];
All_Acc_L1 = [];
All_Acc_L2 = [];
All_Acc_Cor = [];
for K=X
    info = sum(explained(1:K));
    accuracy_L1 = Test(test_x, test_y, coeff, score, mean_face, train_y, K, 'L1');
    accuracy_L2 = Test(test_x, test_y, coeff, score, mean_face, train_y, K, 'L2');
    accuracy_Cor = Test(test_x, test_y, coeff, score, mean_face, train_y, K, 'Cor');
    All_info = [All_info, info];
    All_Acc_L1 = [All_Acc_L1, accuracy_L1];
    All_Acc_L2 = [All_Acc_L2, accuracy_L2];
    All_Acc_Cor = [All_Acc_Cor, accuracy_Cor];
end

hold on
title('Accuracy with different method');
plot(X, All_info)
plot(X, All_Acc_L1);
plot(X, All_Acc_L2);
plot(X, All_Acc_Cor);
xlabel('K');
ylabel('Accuracy');
legend('L1','L2','Cor');

function accuracy = Test(test_x, test_y, coeff, score, mean_face, train_y, K, metric)
    train_len = size(train_y,2);
    test_len = size(test_y,2);
    coeff = coeff(:,1:K);
    score = score(:,1:K);
    accuracy = 0;
    for i=1:test_len
        inputs = test_x(i,:);
        labels = test_y(i);
        feature = (inputs - mean_face) * coeff;
        similarity = zeros(1,train_len);
        for i=1:train_len
            if metric == "Cor"
                val = corrcoef(score(i,:), feature);
                if K>1
                    val = val(1,2);
                end
            elseif metric == "L1"
                val = -norm(score(i,:) - feature, 1);
            elseif metric == "L2"
                val = -norm(score(i,:) - feature, 2);
            end
            similarity(i) = val;
        end
        [best_val, best_idx] = max(similarity);
        if train_y(best_idx) == labels
            accuracy = accuracy + 1;
        end
    end
    accuracy = accuracy / test_len;
end

function Show_Eigen_Face(coeff , A, B)
    H=112; W=92; length=A*B;
    for i=1:length
        img = coeff(:,i);
        img = reshape(img,H,W);
        img = MIN_MAX(img);
        subplot(A,B,i);
        imshow(img);
        title(['eigen ',num2str(i)]);
    end
end

function result = MIN_MAX(img)
    [width, height] = size(img);
    result = zeros(width, height);
    max_val = max(img(:));
    min_val = min(img(:));
    range = max_val-min_val;
    for x = 1:width
        for y = 1:height
            result(x,y) = 255*(img(x,y)-min_val)/range;
        end
    end
    result = uint8(result);
end

function [train_x, train_y, test_x, test_y]=Load_Dataset(pth)
    train_x = [];
    train_y = [];
    test_x = [];
    test_y = [];
    for i=1:40
        for j=1:10
            folder = ['s', num2str(i)];
            fname = [num2str(j), '.pgm'];
            path = fullfile(pth, folder, fname);
            img = imread(path);
            img = im2double(img);
            img = reshape(img, 1, []);
            if j<=5
                train_x = cat(1, train_x, img);
                train_y = [train_y, i];
            else
                test_x = cat(1, test_x, img);
                test_y = [test_y, i];
            end
        end
    end
end