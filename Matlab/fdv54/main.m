% Version : 5.4
% Date : 12.26.2010
% Author  : Omid Sakhi
% http://www.facedetectioncode.com

clear all;
close all;
clc;

if ~exist('gabor.mat','file')
    fprintf ('Creating Gabor Filters ...');
    create_gabor;
end
if exist('net.mat','file')
    load net;
else
    createffnn
end
if exist('imgdb.mat','file')
    load imgdb;
else
    IMGDB = loadimages;
end
while (1==1)
    choice=menu('Face Detection',...
                'Create Database',...
                'Initialize Network',...
                'Train Network',...
                'Test on Photos',...
                'Exit');
    if (choice ==1)
        IMGDB = loadimages;
    end
    if (choice == 2)
        createffnn
    end    
    if (choice == 3)
        net = trainnet(net,IMGDB);
    end
    if (choice == 4)
        pause(0.001);
        [file_name file_path] = uigetfile ('*.jpg');
        if file_path ~= 0
            im = imread ([file_path,file_name]);
            try 
                im = rgb2gray(im);
            end 
            tic
            im_out = imscan (net,im);
            toc
            figure;imshow(im_out,'InitialMagnification','fit');
        end
    end
    if (choice == 5)
        clear all;
        clc;
        close all;
        return;
    end    
end