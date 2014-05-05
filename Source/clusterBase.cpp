//
//  clusterBase.cpp
//  cslForest
//
//  Created by Ashok Chandrashekar on 12/2/12.
//  Copyright (c) 2012 Dartmouth. All rights reserved.
//

#include "../Include/clusterBase.h"

clusterBase::clusterBase(int k, int it, vector<bool> wtVec):K(k), maxIt(it), weights(wtVec){}


vector<vector<int> > clusterBase::doClustering(float** data, vector<int>& labels, int M, vector<int>& pts)
{
    this->initClusters(data, labels, M, pts);
    return this->clusterData(data, M, pts);
}

/*vector<vector<int> > clusterBase::doClustering(float** data, int M, vector<int>& pts)
{
    this->initClusters(data, M, pts);
    return this->clusterData(data, M, pts);
}*/


vector<int> clusterBase::simpleMeans(vector<vector<float> > data, int K)
{
    int N = data.size();
    int M = data[0].size();
    
    vector<int> means (K, - 1);
    vector<double> bestDist (N, INF);
    vector<int> labels(N, 0);
    means[0] = 0;
    
    for (int k = 1; k < K; k++)
    {
        int bestCentroidCand = -1;
        double curBestDistOverall = 0;
            
        for (int i = 0; i < N; i++)
        {
            double dist = calculateDistance( data[i], data[means[k-1]]);
            if (dist < bestDist[i] + EPS)
                bestDist[i] = dist;
                
            if (bestDist[i] > curBestDistOverall)
            {
                curBestDistOverall = bestDist[i];
                bestCentroidCand = i;
            }
        }
            
        if (bestCentroidCand >= 0)
            means[k] = bestCentroidCand;
    }
    
    for (int i  = 0 ; i < N; i++)
    {
        double bestDist = INF;
        for (int k = 0 ; k < K; k++)
        {
            double dist = calculateDistance(data[i], data[means[k]]);
            if (dist < bestDist)
            {
                bestDist = dist;
                labels[i] = k;
            }
        }
    }
    return labels;
}


clusterBase::~clusterBase()
{
    weights.clear();
}
