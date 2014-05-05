// Header
// File: cslTree.cpp
// Author: Ashok Chandrashekar, brain engineering laboratory, Dartmouth 
// Author: Eric Nunes, brain engineering laboratory, Dartmouth
// Description: Core hierarchical classifer functions


#include "../Include/cslTree.h"

cslTree::cslTree(int M, int K, double ff)
{
	nodes = vector<tNode*> (1, new tNode(NULL, 0, 0, M, ff));
	maxK = K;	
}

cslTree::~cslTree() 
{
	for (int i = 0 ; i < nodes.size(); i++)
		delete nodes[i];
	nodes.clear();
}



vector<float> cslTree::labelPosterior(float *x, int M)
{
    
	tNode* T = nodes[0];
	 FILE* fp = fopen("output.txt", "a");
    fprintf(fp, "Inside predict label\n");
    fclose(fp);
    
	while (T->children.size() > 0)
    {
       
    
		int br = T->clst->getClusterLabel(x, M);
		T = T->children[br];
    }
    
    vector<float> result(T->memCnt.size(), 0);
    double denom = Vector_Sum(T->memCnt);
    for (int i = 0; i < result.size(); i++)
        result[i] = T->memCnt[i] / denom;
	return result;
}


void cslTree::batchTrain(float** data, vector <int>& allLabels, vector<int>& lSet, vector <int>& pts, int M)
{
	vector<qNode> Q;
    int maxHt = 0;

    qNode first;
    first.members = pts;
	tNode* root = nodes[0]; 
    root->memCnt = determineMemberCounts(first.members, allLabels, lSet);
    first.tN = root;

    
    Q.push_back(first);    
    int totNodeCnt = 1;
    int spltCnt = 0;
    
    while (!Q.empty())
    {
        qNode qNd = Q[0];
        Q.erase(Q.begin());
        tNode* tN = qNd.tN;

        if (!tN->randProject(data, M, qNd.members))
        {
            // failed to project into random subspace in a linearly seperable manner.
            // make tN a leaf node.
            continue;
        }
        vector<int> ls = determineLabelSet(qNd.members, allLabels);

        int K = min((int)(ls.size()), maxK);
        tN->clst = new clusterModule(K, M, tN->ftInc);
        
        // The call below returns indices in the original labelsFullset array
        vector<vector<int> > clusters = tN->clst->clusterData(data, allLabels, M, qNd.members);
        spltCnt++;
        

        for (int k  = 0 ; k < clusters.size(); k++)
        {
			if (clusters.size() == 0)
				continue;
            tNode* child = new tNode(tN, totNodeCnt, tN->nodeLvl+1, M, root->featFrac);
			child->memCnt = determineMemberCounts(clusters[k], allLabels, lSet);
        	if (child->nodeLvl > maxHt)
				maxHt = child->nodeLvl;
			totNodeCnt++;

			nodes.push_back(child);
			tN->children.push_back(child);

        	
            vector<int> ls = determineLabelSet(clusters[k], allLabels);

			//if (clusters[k].size() > 10)
			if (ls.size() > 1 && clusters[k].size() > 5)
			{
				qNode newQNd;
				newQNd.members = clusters[k];
				newQNd.tN = child;
				Q.push_back(newQNd);
			}
		}
    }
}
