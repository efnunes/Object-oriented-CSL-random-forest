//
//  gmm.h
//  cslForest
//
//  Created by Ashok Chandrashekar on 12/2/12.
//  Copyright (c) 2012 Dartmouth. All rights reserved.
//

#ifndef _GMM_H
#define _GMM_H

#include "clusterBase.h"
#include "dataUtil.h"

class gmmModule: public clusterBase
{
    
	vector<vector<float> > means;
	vector<vector<float> > variances;
	vector<float> mixCoeffs;
	vector<vector<float> > gamma;
    
    double minLLChange;
    
	void eStep(float** data, int M, vector<int>& pts);
	void mStep(float** data, int M, vector<int>& pts);
	double calcLL(float** data, int M, vector<int>& pts);
    double calcLogGauss(float* x, vector<float>& mu, vector<float>& S);
    void initClustersLabeled(float** data, vector<int>& labels,int M, vector<int>& pts);
    void initClustersUnlabeled(float** data, int M, vector<int>& pts);
    void handleDegenerateClusters();
    
    
public:
    gmmModule(int k, int M, vector<bool> weights, int MaxIt = 100, double minll = 0.00005);
    void initClusters(float** data, vector<int>& labels, int M, vector<int>& pts);
    int getClusterLabel(float* data, int M);
    vector<vector<int> > getClusters(float** data, int M, vector <int>& pts);
    vector<vector<int> > clusterData(float** data, int M, vector<int>& pts);
	int getK();
    
    ~gmmModule();
};


#endif
