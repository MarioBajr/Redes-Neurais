% Version : 5.4
% Date : 12.26.2010
% Author  : Omid Sakhi
% http://www.facedetectioncode.com

function im_out = imscan (net,im)

close all

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% PARAMETERS
SCAN_FOLDER = 'imscan/';
UT_FOLDER = 'imscan/under-thresh/';
TEMPLATE1 = 'template1.png';      
TEMPLATE2 = 'template2.png';      
Threshold = 0.5;
DEBUG = 0;
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


warning off;
delete ([UT_FOLDER,'*.*']);
delete ([SCAN_FOLDER,'*.*']);
if (DEBUG == 1)
    mkdir (UT_FOLDER);
    mkdir (SCAN_FOLDER);
end

[m n]=size(im);

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% First Section
C1 = mminmax(double(im));
C2 = mminmax(double(imread (TEMPLATE1)));
C3 = mminmax(double(imread (TEMPLATE2)));
Corr_1 = double(conv2 (C1,C2,'same'));
Corr_2 = double(conv2 (C1,C3,'same'));
Cell.state = int8(imregionalmax(Corr_1) | imregionalmax(Corr_2));
Cell.state(1:13,:)=-1;
Cell.state(end-13:end,:)=-1;
Cell.state(:,1:9)=-1;
Cell.state(:,end-9:end)=-1;
Cell.net = ones(m,n)*-1;
[LUTm LUTn]= find(Cell.state == 1);
imshow(im);
hold on
plot(LUTn,LUTm,'.y');pause(0.01);

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% Second Section
while (1==1)
    [i j] = find(Cell.state==1,1);
    if isempty(i)
        break;
    end
    imcut = im(i-13:i+13,j-9:j+8);
    Cell.state(i,j) = -1;
    Cell.net(i,j) = sim(net,im2vec(imcut));   
    if Cell.net(i,j) < -0.95
        for u_=i-3:i+3
            for v_=j-3:j+3
                try
                    Cell.state(u_,v_)=-1;
                end
            end
        end        
        plot(j,i,'.k');pause(0.01);
        continue;
    elseif Cell.net(i,j) < -1*Threshold
        plot(j,i,'.m');pause(0.01);
        continue;
    elseif Cell.net(i,j) > 0.95
        plot(j,i,'.b');pause(0.01);
        for u_=i-13:i+13
            for v_=j-9:j+9
                try
                    Cell.state(u_,v_)=-1;
                end
            end
        end
    elseif Cell.net(i,j) > Threshold
        plot(j,i,'.g');pause(0.01);
    elseif Cell.net(i,j) < Threshold        
        plot(j,i,'.r');pause(0.01);                        
    end        
    for i_=-1:1
        for j_=-1:1
            m_=i+i_;                    
            n_=j+j_;            
            if (Cell.state(m_,n_) == -1 || Cell.net(m_,n_)~=-1)
                continue;
            end            
            imcut = im(m_-13:m_+13,n_-9:n_+8);
            Cell.net(m_,n_) = sim(net,im2vec(imcut));
            if Cell.net(m_,n_) > 0.95
                plot(n_,m_,'.b');pause(0.01);
                for u_=m_-13:m_+13
                    for v_=n_-9:n_+9
                        try
                            Cell.state(u_,v_)=-1;
                        end
                    end
                end
                continue;
            end           
            if Cell.net(m_,n_) > Threshold
                Cell.state(m_,n_) = 1;
                plot(n_,m_,'.g');pause(0.01);
                if (DEBUG == 1)
                    imwrite(imcut,[SCAN_FOLDER,'@',int2str(m_),',',int2str(n_),' (',int2str(fix(Cell.net(m_,n_)*100)),'%).png']);                       
                end
            else
                Cell.state(m_,n_) = -1;
                plot(n_,m_,'.r');pause(0.01);
                if (DEBUG == 1)
                    imwrite(imcut,[UT_FOLDER,'@',int2str(m_),',',int2str(n_),' (',int2str(fix(Cell.net(m_,n_)*100)),'%).png']);                       
                end
            end  
        end
    end
end

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% Third Section
hold off
figure;imshow (Cell.net,[]);
xy_ = Cell.net > Threshold;
xy_ = imregionalmax(xy_);
xy_ = imdilate (xy_,strel('disk',2,4));
[LabelMatrix,nLabel] = bwlabeln(xy_,4);
CentroidMatrix = regionprops(LabelMatrix,'centroid');
xy_ = zeros(m,n);
for i = 1:nLabel
    xy_(fix(CentroidMatrix(i).Centroid(2)),...
           fix(CentroidMatrix(i).Centroid(1))) = 1;
end
xy_ = drawrec(xy_,[27 18]);
im_out (:,:,1) = im;
im_out (:,:,2) = im;
im_out (:,:,3) = im;
for i = 1:m
    for j=1:n
        if xy_(i,j)==1
            im_out (i,j,1)=0;
            im_out (i,j,2)=255;
            im_out (i,j,3)=0;            
        end
    end
end

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~