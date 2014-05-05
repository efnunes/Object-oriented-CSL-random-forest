//
//  tNode.cpp
//  cslForest
//
//  Created by Ashok Chandrashekar on 12/3/12.
//  Copyright (c) 2012 Dartmouth. All rights reserved.
//

#include "../Include/tNode.h"

tNode::tNode(tNode* p, int nodeId, int ndLvl, int M, double ff)
{
	parent = p;
	quality = 0;
	nodeLvl = ndLvl;
	nId = nodeId;
	featFrac = ff;
	clst = NULL;
	ftInc = vector<bool>(M, false);

	memCnt = vector<int>(0,0);
    seperableThresh = 10;
}


bool tNode::randProject(float** data, int M, vector<int>& pts)
{
    int attempt;
    for(attempt = 0; attempt < 1; attempt++)
    {
        project(M, featFrac);
        //if (isDataSeperable(data, M, pts))
            break;
    }
    if (attempt == 5)
        return false;
    else
        return true;
}

void tNode::project(int M, double ff)
{
    vector<int>idxs;
    for (int i = 0; i < M;i++)
        idxs.push_back(i);
    
    srand(time(NULL));
    random_shuffle(idxs.begin(), idxs.end());
    int n = max((unsigned int)1, (unsigned int)(ff*M));
    
    int j = 0;
    for ( ;j < n; j++)
        ftInc[idxs[j]] = true;
    
    for (;j < M; j++)
        ftInc[idxs[j]] = false;
}


bool tNode::isDataSeperable(float** data, int M, vector<int>& pts)
{
    //compute pairwise distance sum
    
    double dist = 0;
    for (int i = 0; i < pts.size(); i++)
        for (int j = i+1; j < pts.size(); j++)
            for (int m =0; m < M; m++)
                if (ftInc[m])
                    dist+= abs(data[pts[i]][m] - data[pts[j]][m]);
    
    if (dist < seperableThresh)
        return false;
    else
        return true;
}




tNode::~tNode()
{
    ftInc.clear();
    memCnt.clear();
    children.clear();
	if (clst)
		delete clst;
}
