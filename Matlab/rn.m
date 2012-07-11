% clear all;
% close all;
clc;  

test = [test_inputs', test_targets(2, :)'];

[m, n] = size(test);
idx = randperm(m)';

out = test(idx, :);