#include "../Include/kmeans.h"

kmeansModule::kmeansModule(int k, int M, vector<bool> weights, int MaxIt, double minEC):clusterBase(k,  MaxIt, weights), minEnergyChange(minEC)
{
    means = vector<vector<float> >(K, vector<float>(M,0));
}


int kmeansModule::getClusterLabel(float* data, int M)
{
    int c = 0;
    double bestDist = INF;
	for (int k = 0; k < means.size(); k++)
    {
        double dist = calculateDistance(data, means[k]);
        if (dist < bestDist)
        {
            bestDist = dist;
            c = k;
        }
    }
    return c;
}


kmeansModule::~kmeansModule()
{
    for (int k = 0; k < means.size(); k++)
        means[k].clear();

    clusterMemberships.clear();
    means.clear();
}


bool kmeansModule::computeClustMemberships(float** data, int M, vector<int>& pts)
{
    int N = pts.size();
    bool changed = false;
    for (int i = 0; i < N; i++)
    {
        double bestDist = INF;
		int oldLabel = clusterMemberships[i];
        for (int k = 0; k < K; k++)
        {
            double dist = calculateDistance(data[pts[i]], means[k]);
			if (dist < bestDist)
			{
				bestDist = dist;
				clusterMemberships[i] = k;
			}
        }
		if (oldLabel != clusterMemberships[i])
			changed = true;
    }
	return changed;
}



void kmeansModule::computeCentroids(float** data, int M, vector<int>& pts)
{
    int N = pts.size();
	means = vector<vector<float> >(K, vector<float>(M,0));
	vector<float> counts (K, 0);
    for (int i = 0; i < N; i++)
	{
		int k = clusterMemberships[i];
		counts[k]++;
        for (int j = 0; j < M; j++)
            if (weights[j])
                    means[k][j] += data[pts[i]][j];
	}

    for (int k = 0; k < K; k++)
        if (counts[k])
			for (int j = 0; j < M; j++)
				means[k][j] /= counts[k];
	
}




void kmeansModule::initClusters(float **data, vector<int> &labels, int M, vector<int> &pts)
{ 
   initClustersLabeled(data, labels,  M,  pts);
}

/*void kmeansModule::initClusters(float **data, int M, vector<int> &pts)
{
   initClustersUnlabeled (data ,M , pts ); 
  
}*/


void kmeansModule::initClustersLabeled(float** data, vector<int>& labels,int M, vector<int>& pts)
{
    int N = pts.size();
    vector<int> lset = determineLabelSet(pts, labels);
	int KK = lset.size();
    vector <float> counts (KK, 0);
    vector<vector<float> > classMeans(KK, vector<float> (M, 0));
    
    for (int i = 0 ; i < N; i++)
    {
        int l = labels[pts[i]];
        int k = vec_find(lset, l);
        
        counts[k]++;
        for (int j = 0; j < M; j++)
            if (weights[j])
                classMeans[k][j] += data[pts[i]][j];
    }

    for (int k = 0 ; k < KK; k++)
        for (int j = 0; j < M; j++)
            if (weights[j])
                classMeans[k][j] /= counts[k];
    
	if (K >= KK)
    {
        K = KK;
        means = classMeans;
        return;
    }
    
    vector<int> clustLbls = simpleMeans(classMeans, K);
    counts = vector<float>(K, 0);

    for (int i = 0; i < pts.size(); i++)
    {
        int l = labels[pts[i]];
        int k = vec_find(lset, l);
        for (int j = 0; j < M; j++)
            if (weights[j])
                means[clustLbls[k]][j] += data[pts[i]][j];
        counts[clustLbls[k]]++;
    }
    
    for (int k = 0 ; k < K; k++)
        for (int j = 0; j < M; j++)
            if (weights[j])
                means[k][j] /= counts[k];
} 



void kmeansModule::initClustersUnlabeled(float** data, int M, vector<int>& pts)
{
	int KK = 10;//number of clusters
	int N = pts.size();
	vector<vector<float> > means(KK , vector<float> (M, 0));

	srand(time(0));
	random_shuffle ( pts.begin(), pts.end() );

	for (int k = 0; k < KK; k++)
	   for (int j = 0; j < M; j++)
	        if (weights[j])
                means[k][j] = data[pts[k]][j];
}
    // pick K pts randomly





	

void kmeansModule::handleDegenerateClusters()
{
    
}

vector<vector<int> > kmeansModule::clusterData(float** data, int M, vector<int>& pts)
{
	clusterMemberships = vector<int>(pts.size(),-1);

    for (int i = 1; i < maxIt; i++)
    {
        bool change = computeClustMemberships(data, M, pts);
        computeCentroids(data, M, pts);
        if (!change)
            break;                
    }
    
    //check and fix degenerate clusters
    handleDegenerateClusters();
    
    return getClusters(data, M, pts);

}


vector<vector<int> > kmeansModule::getClusters(float** data, int M, vector <int>& pts)
{
    int N = pts.size();
    
    vector<vector<int> > clusters (K, vector<int>(0, 0));
    for (int i = 0; i < N; i++)
    	clusters[clusterMemberships[i]].push_back(pts[i]);
    return clusters;
}

