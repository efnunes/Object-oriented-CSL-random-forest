//
//  clusterBase.h
//  cslForest
//
//  Created by Ashok Chandrashekar on 12/2/12.
//  Copyright (c) 2012 Dartmouth. All rights reserved.
//

#ifndef CLUSTERBASE_H
#define CLUSTERBASE_H

#include "common.h"
#include "mathUtil.h"

class clusterBase
{

protected:
    int K;
    int maxIt;
    vector<bool> weights;
    
    virtual void initClusters(float** data, vector<int>& labels, int M, vector<int>& pts) = 0;
    virtual vector<vector<int> > getClusters(float** data, int M, vector <int>& pts) = 0;
    vector<int> simpleMeans(vector<vector<float> > data, int K);

public:
    
    clusterBase(int k, int it, vector<bool> wtVec);
    
    vector<vector<int> > doClustering(float** data, vector<int>& labels, int M, vector<int>& pts);
    virtual int getClusterLabel(float* data, int M) = 0;
    virtual vector<vector<int> > clusterData(float** data, int M, vector<int>& pts) = 0;
    
    
    int getK(){return K;}
    
	
    virtual ~clusterBase();
};



#endif
