//
// Created by gupta on 23-04-2020.
//

#ifndef ASSIGNMENT3_AGING_H
#define ASSIGNMENT3_AGING_H

#include "Pager.h"
#include "../FrameTableEntry.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class Aging : public Pager{
private:
    int hand;
    vector<unsigned int> agingVector;
    vector<FrameTableEntry*> *frameTable;
    unsigned int victimFrameAge : 32;
public:
    FrameTableEntry* selectVictimFrame() override;
    Aging(vector<FrameTableEntry*> *frameTable);
};


#endif //ASSIGNMENT3_AGING_H
