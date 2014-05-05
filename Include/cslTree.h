#ifndef _CSLTREE_H
#define _CSLTREE_H

#include "tNode.h"
#include "qNode.h"
#include "dataUtil.h"


class cslTree
{
	vector<tNode*> nodes;
	int maxK;
    
    
public:
	cslTree(int M, int K = 2, double ff = 0.4);
    
	vector<float> labelPosterior(float *x, int M);
	void batchTrain(float** data, vector<int>& labels, vector<int>& pts, int M);
	//void batchTrain(float** data, vector<int>& pts,int M);
    void batchTrain(float** data, vector <int>& allLabels, vector<int>& lSet, vector <int>& pts, int M);
	

	void incrementalUpdate(float* data, int label, int M);
    
    
	~cslTree();
};




#endif
