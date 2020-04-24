//
// Created by gupta on 23-04-2020.
//

#ifndef ASSIGNMENT3_WORKINGSET_H
#define ASSIGNMENT3_WORKINGSET_H


#include "Pager.h"
#include "../FrameTableEntry.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class WorkingSet : public Pager {
private:
    int hand;
    unsigned long long threshold;
    vector<FrameTableEntry*> *frameTable;
    unsigned long long *instructionCnt;
public:
    FrameTableEntry* selectVictimFrame() override;
    WorkingSet(vector<FrameTableEntry*> *frameTable, unsigned long long *instructionCnt);
    WorkingSet();
};


#endif //ASSIGNMENT3_WORKINGSET_H
