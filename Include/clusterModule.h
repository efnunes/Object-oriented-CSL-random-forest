#ifndef _CLUSTERMODULE_H
#define _CLUSTERMODULE_H

#include "gmm.h"
#include "kmeans.h"

#define PLSA 1
#define LDA 2
#define KMEANS 3
#define GMM 4



class clusterModule
{
	int clusterMethod;
    clusterBase *basePtr;
	
public:
	clusterModule(int k, int M, vector<bool> weights, int clstMethod = 0);
	
    vector<vector<int> > clusterData(float** data, vector<int>& labels, int M, vector<int>& pts);
	vector<vector<int> > clusterData(float** data, int M, vector<int>& pts);
	int getClusterLabel(float* data, int M);
    int getK();
    
	~clusterModule();
	
};


#endif
