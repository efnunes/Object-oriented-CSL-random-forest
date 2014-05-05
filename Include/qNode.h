//
//  qNode.h
//  cslForest
//
//  Created by Ashok Chandrashekar on 12/3/12.
//  Copyright (c) 2012 Dartmouth. All rights reserved.
//

#ifndef _QNODE_H
#define _QNODE_H

#include "tNode.h"

class qNode
{
public:
    qNode();
    ~qNode();
    
    vector<int> members;
    tNode* tN;
    double fragRatio;
};




#endif
