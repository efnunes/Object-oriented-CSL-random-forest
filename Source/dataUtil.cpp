#include "../Include/dataUtil.h"

vector <int> determineMemberCounts(vector <int>& Members, vector<int>& FullLabelSet, vector <int>& LabelSet)
{
    vector <int> LabelCnts(LabelSet.size(), 0);
    for (int i = 0 ; i < Members.size(); i++)
	{
		int idx = vec_find(LabelSet, FullLabelSet[Members[i]] );
		LabelCnts[idx]++;
	}
    return LabelCnts;
}

vector <int> determineLabelSet(vector <int>& FullLabelSet)
{
    vector <int> LabelSet;
    for (unsigned int i = 0; i < FullLabelSet.size(); i++)
        if (vec_find(LabelSet, FullLabelSet[i]) == -1)
            LabelSet.push_back(FullLabelSet[i]);
    return LabelSet;
}


vector <int> determineLabelSet(vector <int>& Pts, vector <int>& FullLabelSet)
{
    vector <int> LabelSet;
    for (unsigned int i = 0; i < Pts.size(); i++)
        if (vec_find(LabelSet, FullLabelSet[Pts[i]]) == -1)
            LabelSet.push_back(FullLabelSet[Pts[i]]);
    return LabelSet;
}

