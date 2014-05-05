#ifndef _DATAUTIL_H
#define _DATAUTIL_H

#include "mathUtil.h"


//void assert(bool val);
vector <int> determineLabelSet(vector <int>& FullLabelSet);
vector <int> determineLabelSet(vector <int>& Pts, vector <int>& FullLabelSet);
vector <int> determineMemberCounts(vector <int>& Members, vector<int>& FullLabelSet, vector <int>& LabelSet);


#endif
