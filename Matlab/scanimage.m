function [ out ] = scanimage( net, im )
%SCANIMAGE Summary of this function goes here
%   Detailed explanation goes here

sm = 27;
sn = 18;
d = 5;

[rm, rn] = size(im);
out = zeros(rm, rn);

for k = .2:.2:1
    scale_im = imresize(im, k);
    [m, n] = size(scale_im);
    
    for i = 1:d:m
        for j = 1:d:n
            dm = i+sm;
            dn = j+sn;

            fprintf('(%d %d) [%d %d]\n', dm, dn, m, n);

            if dm < m & dn < n
                sub_im = im(i:dm, j:dn);
                signal = sim(net,im2vec(sub_im));
                if signal(1,1) == 1
%                     figure;imshow(sub_im);
                    out(uint8(i/k):uint8(dm/k), uint8(j/k):uint8(dn/k)) = 1;
                end
            end
        end
    end
end

threshold = graythresh(out);
bw = im2bw(out,threshold);
imshow(bw);

se = strel('disk',2);
bw = imclose(bw,se);

bw = imfill(bw,'holes');

% figure;
% imshow(bw);

figure;
imshow(im);

[B,L] = bwboundaries(bw,'noholes');

% Display the label matrix and draw each boundary
% imshow(label2rgb(L, @jet, [.5 .5 .5]))
hold on
for k = 1:length(B)
  boundary = B{k};
  plot(boundary(:,2), boundary(:,1), 'g', 'LineWidth', 1)
end

end

