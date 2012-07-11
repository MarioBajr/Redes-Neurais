clear all;
close all;
clc;  

addpath ./fdv54
addpath ./somtoolbox

%Create Gabor
if ~exist('gabor.mat','file')
    fprintf ('Creating Gabor Filters ...');
    create_gabor;
end

%Load / Process Train Images
if exist('train_inputsDB.mat','file')
    load train_inputsDB;
    load train_targetsDB;
else
    [train_inputs, train_targets] = loaddatabase('train');
%     train_inputs = cca(train_inputs', 500, 10)';
%     train_inputs = cca(train_inputs',pcaproj(train_inputs',100),10)';
    
    save train_inputsDB train_inputs;
    save train_targetsDB train_targets;
end

%Load / Process Test Images
if exist('test_inputsDB.mat','file')
    load test_inputsDB;
    load test_targetsDB;
else
    [test_inputs, test_targets] = loaddatabase('test');
%     test_inputs = cca(test_inputs', 500, 10)';
%     test_inputs = cca(test_inputs',pcaproj(test_inputs',100),10)';
    
    save test_inputsDB test_inputs;
    save test_targetsDB test_targets;
end

[m, n] = size(train_inputs);
idx = randperm(n);

rand_inputs = train_inputs(:, idx);
rand_targets = train_targets(:, idx);

if exist('trained_net.mat', 'file')
    load trained_net;
else
    net = newlvq(train_inputs,40,[.6 .4]);
    net.trainParam.epochs = 300;
    [net, tr] = train(net, train_inputs, train_targets);
    save trained_net net;
end

%--------------------------------
% load simpleclass_dataset
% net = newsom(simpleclassInputs,[8 8]);
% net = train(net,simpleclassInputs);
% plotsomtop(net);

% net = newsom(inputs,[5 5]);
% [net, tr] = train(net,inputs);
% plotsomtop(net);

outputs = sim(net,test_inputs);
plotconfusion(test_targets,outputs);

[file_name file_path] = uigetfile ('*.jpg');
if file_path ~= 0
    im = imread ([file_path,file_name]);
    try
        im = rgb2gray(im);
    end
    
    out = scanimage(net, im);
end

