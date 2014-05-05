#include "../Include/clusterModule.h"
#include "../Include/dataUtil.h"


clusterModule::clusterModule(int K, int M, vector<bool> weights, int clstMethod): clusterMethod(clstMethod), basePtr(NULL)
{
    if (clstMethod == 0)
        clstMethod = KMEANS;
    
    switch(clstMethod)
    {
        case KMEANS: basePtr = new kmeansModule(K, M, weights); break;
        case GMM :   basePtr = new gmmModule(K, M, weights);  break;
        default: exit(0);
    }
}


vector<vector<int> > clusterModule::clusterData(float** data, vector<int>& labels, int M, vector<int>& pts)
{
	
	return basePtr->doClustering(data, labels, M, pts);
}

int clusterModule::getClusterLabel(float* data, int M)
{
    return basePtr->getClusterLabel(data, M);
}


clusterModule::~clusterModule()
{
    if (basePtr) delete basePtr;
    basePtr = NULL;
}



