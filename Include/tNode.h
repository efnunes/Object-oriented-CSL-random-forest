//
//  tNode.h
//  cslForest
//
//  Created by Ashok Chandrashekar on 12/3/12.
//  Copyright (c) 2012 Dartmouth. All rights reserved.
//

#ifndef _TNODE_H
#define _TNODE_H

#include "common.h"
#include "clusterModule.h"

class tNode
{
public:
    vector<int> memCnt;
    vector <tNode*> children;
    tNode* parent;
    clusterModule* clst;
    
    double quality;
    int nodeLvl;
    int nId;
    
    double featFrac;
    vector<bool> ftInc;
    double seperableThresh;
    
    
    bool randProject(float** data, int M, vector<int>& pts);
    void project(int M, double ff);
    bool isDataSeperable(float** data, int M, vector<int>& pts);

    
    
    tNode(tNode* p, int nodeId, int ndLvl, int M, double ff);
    ~tNode();
};




#endif
