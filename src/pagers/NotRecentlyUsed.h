//
// Created by gupta on 23-04-2020.
//

#ifndef ASSIGNMENT3_NOTRECENTLYUSED_H
#define ASSIGNMENT3_NOTRECENTLYUSED_H


#include "Pager.h"
#include "../FrameTableEntry.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class NotRecentlyUsed : public Pager {
private:
    int hand;
    unsigned long long resetCounter;
    unsigned long long *instructionCount;
    vector<FrameTableEntry*> *frameTable;
public:
    FrameTableEntry* selectVictimFrame() override;
    NotRecentlyUsed(vector<FrameTableEntry*> *frameTable, unsigned long long *instructionCount);
    NotRecentlyUsed();
};


#endif //ASSIGNMENT3_NOTRECENTLYUSED_H
