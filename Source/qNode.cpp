//
//  qNode.cpp
//  cslForest
//
//  Created by Ashok Chandrashekar on 12/3/12.
//  Copyright (c) 2012 Dartmouth. All rights reserved.
//

#include "../Include/qNode.h"

qNode::qNode():tN(NULL),fragRatio(0){}
qNode::~qNode()
{
    members.clear();
    tN = NULL;
}