//
//  cslForest.h
//  cslForest
//
//  Created by Ashok Chandrashekar on 12/2/12.
//  Copyright (c) 2012 Dartmouth. All rights reserved.
//

#ifndef _CSLFOREST_H
#define _CSLFOREST_H
#include "cslTree.h"
#include "qNode.h"

class cslForest
{
	double bagFrac;
    double featFrac;
    
	vector<cslTree*> trees;
	vector<int> labelSet;
    
	vector<int> getBaggedData(vector<int>& labels);
	//vector<int> getBaggedData(float** data);
    
public:
    
	cslForest(int N, int M, int maxK=2, double bf = 0.6, double ff = 0.6);
    
	vector<float> predictLabel(float* x, int M);
	void batchTrain(float** data, vector <int>& labels, int M);
	//void batchTrain(float** data, int M);
	void incrementalUpdate(float* data, int label, int M);
    
	~cslForest();
    
};



union addrConverter
{
    cslForest* fptr;
    unsigned int addr;
};


#endif
