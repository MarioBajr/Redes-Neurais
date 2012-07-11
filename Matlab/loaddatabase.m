function [ inputs, targets ] = loaddatabase( group )


%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
face_folder = ['db/',group,'/face/'];  %LOCATION OF FACE IMAGES
non_face_folder = ['db/',group,'/non-face/']; %LOCATION OF NON-FACE IMAGES
file_ext = '.png';
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

disp(face_folder);

inputs = [];
targets = [];


fprintf ('Loading Faces ');
folder_content = dir ([face_folder,'*',file_ext]);
nface = size (folder_content,1);

for k=1:nface
    string = [face_folder,folder_content(k,1).name];
    image = imread(string);    
    [m n] = size(image);
    if (m~=27 || n~=18)
        continue;
    end
    fprintf ('.');
    
    IM {1} = im2vec (image);    % ORIGINAL FACE IMAGE
    IM {2} = im2vec (fliplr(image));    % MIRROR OF THE FACE 
    IM {3} = im2vec (circshift(image,1)); 
    IM {4} = im2vec (circshift(image,-1));
    IM {5} = im2vec (circshift(image,[0 1]));
    IM {6} = im2vec (circshift(image,[0 -1]));
    IM {7} = im2vec (circshift(fliplr(image),1));
    IM {8} = im2vec (circshift(fliplr(image),-1));
    IM {9} = im2vec (circshift(fliplr(image),[0 1]));
    IM {10}= im2vec (circshift(fliplr(image),[0 -1]));
    
    for i=1:10
        inputs = cat(2, inputs, IM{i});
        targets = cat(2, targets, [1; 0]);
    end    
end
fprintf ('\nLoading non-faces ');    
folder_content = dir ([non_face_folder,'*',file_ext]);
nnface = size (folder_content,1);
for k=1:nnface
    string = [non_face_folder,folder_content(k,1).name];
    image = imread(string);
    [m n] = size(image);
    if (m~=27 || n~=18)
        continue;
    end
    fprintf ('.');
    IM {1} = im2vec (image);
    IM {2} = im2vec (fliplr(image));
    IM {3} = im2vec (flipud(image));
    IM {4} = im2vec (flipud(fliplr(image)));    
    for i=1:4
        inputs = cat(2, inputs, IM{i});
        targets = cat(2, targets, [0; 1]);
    end
end
fprintf('\n');

