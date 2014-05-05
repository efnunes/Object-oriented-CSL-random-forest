%% Header
% File: MyScript.m
% Author: Ashok Chandrashekar, brain engineering laboratory, Dartmouth 
% Author: Eric Nunes, brain engineering laboratory, Dartmouth 
% Description: File contains routine calls for the core components in the algorithm

%% Many model parameters defined in DefConstants.m. Please run it before any other function. 
clear;
K = 64;        
% Include your data path
DATA_PATH = 'C:\Users\Eric\Documents\GitHub\StillImageProcessing\cslForest\csl-matlab\csl_unsupervised\Data\Digit\';


load (strcat(DATA_PATH, 'TrainLabels'));
load (strcat(DATA_PATH, 'TrainDMapIndices'));
load (strcat(DATA_PATH, 'TestDMapIndices'));
load (strcat(DATA_PATH, 'TestLabels'));
% load (strcat(DATA_PATH, 'Labels'));
load (strcat(DATA_PATH, 'H'));

for run = 1:1
    fprintf('BEGIN: Run %d\n', run); 
    
    TrainDMap = H(TrainDMapIndices{run}, :);
    TestDMap = H(TestDMapIndices{run}, :);
    
    
    %% CSL
    BF = 0.6; % bagging fraction
    FF = 0.2; % Feature fraction
    num_trees = 100;
    for maxk = 10
        %Training
       
        tic;
        [PredObjLabels{run, maxk}] = batchTrain(TrainDMap, TrainLabels{run}, num_trees, maxk, BF, FF, TestDMap);
        %trainTime(run, maxk) TestTime(run, maxk)
        time = toc;
        CFMat = zeros (K, K);
        for Cnt = 1 : numel(PredObjLabels{run, maxk})
            if PredObjLabels{run, maxk}(Cnt) ~= -1
                CFMat(TestLabels{run}(Cnt), PredObjLabels{run, maxk}(Cnt)) = CFMat(TestLabels{run}(Cnt), PredObjLabels{run, maxk}(Cnt)) + 1;                    
            end
        end
        Succ(run, maxk) = 0;
        for i = 1:K
            Succ(run, maxk) = Succ(run, maxk) + CFMat(i,i);
        end
        Succ(run, maxk) = 100 * Succ(run, maxk) / numel(PredObjLabels{run, maxk});
        fprintf('CSL Tree descent: max_k = %d, Total time:%f , Accuracy:%f%%,    \n', ...
                maxk, time, Succ(run, maxk));
            
         clear mex;                       
    end
 end

save Calt256_All_Classifiers;
return;
    