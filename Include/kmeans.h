//
//  kmeans.h
//  cslForest
//
//  Created by Ashok Chandrashekar on 12/2/12.
//  Copyright (c) 2012 Dartmouth. All rights reserved.
//

#ifndef _KMEANS_H
#define _KMEANS_H

#include <cstdlib>
#include <algorithm>
#include <ctime>
#include "clusterBase.h"
#include "dataUtil.h"

class kmeansModule: public clusterBase
{   
	vector<vector<float> > means;
	vector<int> clusterMemberships;
    double minEnergyChange;
    
	bool computeClustMemberships(float** data, int M, vector<int>& pts);
	void computeCentroids(float** data, int M, vector<int>& pts);
	double calcEnergy(float** data, int M, vector<int>& pts);
    void initClustersLabeled(float** data, vector<int>& labels,int M, vector<int>& pts);
    void initClustersUnlabeled(float** data, int M, vector<int>& pts);
    void handleDegenerateClusters();
    
public:
    kmeansModule(int k, int M, vector<bool> weights, int MaxIt = 100, double minEChange = 0.00005);
    void initClusters(float** data, vector<int>& labels, int M, vector<int>& pts);
	//void initClusters(float** data, int M, vector<int>& pts);
	
    int getClusterLabel(float* data, int M);
    vector<vector<int> > getClusters(float** data, int M, vector <int>& pts);
    vector<vector<int> > clusterData(float** data, int M, vector<int>& pts);
	
    ~kmeansModule();
};


#endif
