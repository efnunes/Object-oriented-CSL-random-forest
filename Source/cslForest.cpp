//
//  cslForest.cpp
//  cslForest
//
//  
//  Copyright (c) 2012 Dartmouth. All rights reserved.
//

#include "../Include/cslForest.h"
#include <omp.h>

#define NUM_THREADS 4

cslForest::cslForest(int Nt, int M, int maxK, double bf, double ff)
{
    bagFrac = bf;
    featFrac = ff;
    
    for (int i = 0; i < Nt; i++)
        trees.push_back(new cslTree(M, maxK, ff));
}


cslForest::~cslForest()
{
	     FILE* fp = fopen("output.txt", "a");
    fprintf(fp, "cslforest destructor\n");
    fclose(fp);
    
	for (int i = 0; i < trees.size();i++)
		delete trees[i];
    labelSet.clear(); trees.clear();
}


vector<float> cslForest::predictLabel(float* x, int M)
{
    

	FILE* fp = fopen("output.txt", "a");
    fprintf(fp, "Inside predict label, trees size:, labelset size:\n");
    fclose(fp);
    
	
	vector<float> fullPosterior(labelSet.size(), 0);
	vector<vector<float> > treePosteriors(trees.size(), vector<float>(labelSet.size(), 0));
	
	omp_set_num_threads(NUM_THREADS);
	#pragma omp parallel for
	
	for (int i = 0; i < trees.size(); i++)
	{
		treePosteriors[i] = trees[i]->labelPosterior(x,M);
    }
	for (int i = 0; i < trees.size(); i++)
		for (int l = 0; l < labelSet.size(); l++)
			fullPosterior[l] += treePosteriors[i][l];

	//return labelSet[vec_maxIdx(fullPosterior)];
	return fullPosterior;
}


void cslForest::batchTrain(float** data, vector <int>& labels, int M)
{
    labelSet = determineLabelSet(labels);
	vector<vector<int> > indices;
    omp_set_num_threads(NUM_THREADS);
    for	(int i = 0; i < trees.size(); i++)
		indices.push_back(getBaggedData(labels));
	
	#pragma omp parallel for
	for	(int i = 0; i < trees.size(); i++)
		trees[i]->batchTrain(data, labels, labelSet, indices[i],  M);
}

/*void cslForest::batchTrain(float** data,int M)
{
	vector<vector<int> > indices;
    //omp_set_num_threads(NUM_THREADS);
    for	(int i = 0; i < trees.size(); i++)
		indices.push_back(getBaggedData(data));
	
	//#pragma omp parallel for
	for	(int i = 0; i < trees.size(); i++)
		trees[i]->batchTrain(data, indices[i],  M);
		
}*/



vector<int> cslForest::getBaggedData(vector<int>& labels)
{
    vector<int> baggedIdxs;
    for (int lidx = 0; lidx < labelSet.size();lidx++)
    {
        vector<int> idxs = vectorFindAll(labels, labelSet[lidx]);
        random_shuffle(idxs.begin(), idxs.end());
        int n = max((unsigned int)1, (unsigned int)(bagFrac*idxs.size()));
        for (int j = 0; j < n; j++)
            baggedIdxs.push_back(idxs[j]);
    }
    return baggedIdxs;
}

/*vector<int> cslForest::getBaggedData(float** data)
{
	vector<int> baggedIdxs;
	vector<int> idxs;
	int N = (sizeof(data)/sizeof(data[0]));
	for (int i = 0; i < N; i++)
		idxs.push_back(i);
	random_shuffle(idxs.begin(), idxs.end());
	int n = max((unsigned int)1, (unsigned int)(bagFrac*idxs.size()));
	for (int j = 0; j < n; j++)
            baggedIdxs.push_back(idxs[j]);
	return baggedIdxs;
}*/




