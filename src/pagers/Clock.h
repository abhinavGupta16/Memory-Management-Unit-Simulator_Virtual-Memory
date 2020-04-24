//
// Created by gupta on 23-04-2020.
//

#ifndef ASSIGNMENT3_CLOCK_H
#define ASSIGNMENT3_CLOCK_H


#include "Pager.h"
#include "../FrameTableEntry.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class Clock  : public Pager {
private:
    int hand;
    vector<FrameTableEntry*> *frameTable;
    void incrementHand(int &hand);
public:
    FrameTableEntry* selectVictimFrame() override;
    Clock(vector<FrameTableEntry*> *frameTable);
};


#endif //ASSIGNMENT3_CLOCK_H
