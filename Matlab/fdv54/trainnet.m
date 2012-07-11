% Version : 5.4
% Date : 12.26.2010
% Author  : Omid Sakhi
% http://www.facedetectioncode.com

function NET = trainnet(net,IMGDB)

%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
net.trainFcn = 'trainscg';
net.trainParam.lr = 0.4;
net.trainParam.epochs = 400;
net.trainParam.show = 10;
net.trainParam.goal = 1e-3;
%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

T{1,1} = cell2mat(IMGDB(2,:));
P{1,1} = cell2mat(IMGDB(3,:));
net = train(net,P,T);
%save net net
NET = net;