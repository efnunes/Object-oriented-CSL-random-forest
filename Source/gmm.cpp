#include "../Include/gmm.h"

gmmModule::gmmModule(int k, int M, vector<bool> weights, int MaxIt, double minll):clusterBase(k,  MaxIt, weights), minLLChange(minll)
{
    means = vector<vector<float> >(K, vector<float>(M,0));
	variances = vector<vector<float> > (K, vector<float>(M,0));
	mixCoeffs = vector<float>(K);
}


int gmmModule::getClusterLabel(float* data, int M)
{
    int c = 0;
    double bestProb = -INF;
	for (int k = 0; k < means.size(); k++)
    {
        double lprob = calcLogGauss(data, means[k], variances[k]);
        if (lprob > bestProb)
        {
            bestProb = lprob;
            c = k;
        }
    }
    return c;
}


gmmModule::~gmmModule()
{
    for (int k = 0; k < means.size(); k++)
        means[k].clear();
    for (int k = 0; k < variances.size(); k++)
        variances[k].clear();
    for (int k = 0; k < gamma.size(); k++)
        gamma[k].clear();

    means.clear();
    variances.clear();
    gamma.clear();
    mixCoeffs.clear();
    
}

int gmmModule::getK()
{
    return K;
}

double gmmModule::calcLogGauss(float* x, vector<float>& mu, vector<float>& S)
{

    int M = mu.size();
    int dims = 0;
    for(int j = 0 ; j < M; j++)
        dims += (weights[j]== true);
    
    double denom = -0.5*dims*log(2*PI);
    double sum =0;
    for (int i = 0; i < M; i++)
        if (weights[i])
            sum += log (S[i]);
    denom -=  0.5 * sum;

    double numer = 0;
    for (int i = 0; i < M; i++)
        if (weights[i])
            numer +=  (pow((x[i] - mu[i]), 2)/ S[i]);
    numer *= -0.5;
    return (numer + denom);
}


void gmmModule::eStep(float** data, int M, vector<int>& pts)
{
    int N = pts.size();
    
    for (int i = 0; i < N; i++)
    {
        double bestPb = -INF;
        for (int k = 0; k < K; k++)
        {
            gamma[i][k] = log(mixCoeffs[k]) + calcLogGauss(data[pts[i]], means[k], variances[k]);
            if (gamma[i][k] > bestPb)
                bestPb = gamma[i][k];
        }
        
        double denom = 0;
        for (int k = 0; k < K; k++)
        {
            gamma[i][k] = exp(gamma[i][k] - bestPb);
            denom += gamma[i][k];
        }
        for (int k = 0; k < K; k++)
            gamma[i][k] /= denom;
    }
}



void gmmModule::mStep(float** data, int M, vector<int>& pts)
{
    int N = pts.size();
    
    for (int k = 0; k < K; k++)
    {   
        double denom = 0;
        for (int i = 0; i < N; i++)
            denom += gamma[i][k];
        
        mixCoeffs[k] = denom / N;
        
        
        //Estimate means
        vector<float> numerator(M, 0.0);
        for (int j = 0; j < M; j++)
        {
            if (weights[j])
                for (int i = 0; i < N; i++)
                    numerator[j] += (gamma[i][k] * data[pts[i]][j]);
        }
        for (int j = 0; j < M; j++)
            if (weights[j])
                means[k][j] = numerator[j] / denom;
        
        //Estimate variance
        for (int j = 0; j < M; j++)
        {
            if (weights[j])
            {
                variances[k][j] = 0;
                for (int i = 0; i < N; i++)
                    variances[k][j] +=  (gamma[i][k] * pow((data[pts[i]][j] - means[k][j]), 2));
            
                variances[k][j] = variances[k][j] / denom + EPS;
            }
        }
    }
}

double gmmModule::calcLL(float** data, int M, vector<int>& pts)
{
    double L = 0;
    int N = pts.size();
    
    for (int i = 0; i < N; i++)
       for (int k = 0; k < K; k++)
           L += (gamma[i][k] * (log(mixCoeffs[k]) + calcLogGauss(data[pts[i]], means[k], variances[k])));
    return L;
}


void gmmModule::initClusters(float **data, vector<int> &labels, int M, vector<int> &pts)
{
    initClustersLabeled(data, labels,  M,  pts);
}


void gmmModule::initClustersLabeled(float** data, vector<int>& labels,int M, vector<int>& pts)
{
    int N = pts.size();
    vector<int> lset = determineLabelSet(pts, labels);
	int KK = lset.size();
    vector <float> counts (KK, 0);
    vector<vector<float> > classMeans(KK, vector<float> (M, 0));
    vector<vector<float> > classVars(KK, vector<float> (M, 0));
    vector<vector<float> > classGamma(N, vector<float>(KK, 0));
    
    for (int i = 0 ; i < N; i++)
    {
        int l = labels[pts[i]];
        int k = vec_find(lset, l);
        
        counts[k]++;
        classGamma[i][k] = 1;
        for (int j = 0; j < M; j++)
            if (weights[j])
                classMeans[k][j] += data[pts[i]][j];
    }

    for (int k = 0 ; k < KK; k++)
        for (int j = 0; j < M; j++)
            if (weights[j])
                classMeans[k][j] /= counts[k];
    
    for (int k = 0; k < KK; k++)
        for (int j = 0; j< M;j++)
        {
            if (weights[j])
            {
                for (int i= 0; i < N; i++)
                    classVars[k][j] += classGamma[i][k] * pow((data[pts[i]][j] - classMeans[k][j]),2) + EPS;
                classVars[k][j] = classVars[k][j] / counts[k];
            }
        }
    if (K >= KK)
    {
        K = KK;
        means = classMeans;
        variances = classVars;
		
		for (int k = 0; k < K; k++)
			mixCoeffs[k] = ((double)counts[k])/(1.0*N);
        return;
    }
    
    vector<int> clustLbls = simpleMeans(classMeans, K);
    
    counts = vector<float>(K, 0);
    gamma = vector<vector<float> >(N, vector<float>(K,0));

    for (int i = 0; i < pts.size(); i++)
    {
        int l = labels[pts[i]];
        int k = vec_find(lset, l);
        for (int j = 0; j < M; j++)
            if (weights[j])
                means[clustLbls[k]][j] += data[pts[i]][j];
        counts[clustLbls[k]]++;
        gamma[i][clustLbls[k]] = 1;
    }
    for (int k = 0; k < K; k++)
		mixCoeffs[k] = ((double)counts[k])/(1.0*N);
        

    for (int k = 0 ; k < K; k++)
        for (int j = 0; j < M; j++)
            if (weights[j])
                means[k][j] /= counts[k];
    
    for (int k = 0; k < K; k++)
        for (int j = 0; j< M;j++)
            if (weights[j])
            {
                for (int i= 0; i < N; i++)
                    variances[k][j] += gamma[i][k] * pow((data[pts[i]][j] - means[k][j]),2) + EPS;
                variances[k][j] = variances[k][j] / counts[k];
            }
}



void gmmModule::initClustersUnlabeled(float** data, int M, vector<int>& pts)
{
    // pick K pts randomly
    
}

void gmmModule::handleDegenerateClusters()
{
    vector <double> gammaSum(K, 0);
    for (int i = 0 ; i < gamma.size(); i++)
        for (int k = 0; k < K; k++)
            gammaSum[k]+=gamma[i][k];
    
    
    //find smallest nonzero gammasum
    int cand = -1;
    double min = INF;
    for (int k = 0; k < gammaSum.size(); k++)
        if (gammaSum[k] > EPS && gammaSum[k] < min)
        {
            min = gammaSum[k]; cand = k;
        }

    for (int k = 0; k < K; k++)
        if (gammaSum[k] == 0)
            variances[k] = variances[cand];
    
}

vector<vector<int> > gmmModule::clusterData(float** data, int M, vector<int>& pts)
{

    vector<double> LL(1, -INF);
    gamma = vector<vector<float> > (pts.size(), vector<float>(K,0));

    for (int i = 1; i < maxIt; i++)
    {
        eStep(data, M, pts);
        mStep(data, M, pts);
        LL.push_back(calcLL(data, M, pts));
        if (LL[i-1] > (LL[i]+EPS))
        {
            //mexPrintf("LL Error!");
            break;
        }
 
        double dLi = (LL[i] - LL[i-1]) / LL[i];
        if (abs(dLi) < minLLChange)
            break;                
    }
    
    //check and fix degenerate clusters
    handleDegenerateClusters();
    
    return getClusters(data, M, pts);

}


vector<vector<int> > gmmModule::getClusters(float** data, int M, vector <int>& pts)
{
    int N = pts.size();
    
    vector<vector<int> > clusters (K, vector<int>(0, 0));
    for (int i = 0; i < N; i++)
    	clusters[vec_maxIdx(gamma[i])].push_back(pts[i]);
    return clusters;
}

