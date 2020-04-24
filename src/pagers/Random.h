//
// Created by gupta on 23-04-2020.
//

#ifndef ASSIGNMENT3_RANDOM_H
#define ASSIGNMENT3_RANDOM_H

#include "Pager.h"
#include "../FrameTableEntry.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class Random : public Pager{
private:
    int hand;
    int randomCounter;
    vector<FrameTableEntry*> *frameTable;
    vector<int> *randvals;
public:
    FrameTableEntry* selectVictimFrame() override;
    Random(vector<FrameTableEntry*> *frameTable, vector<int> *randvals);
    Random();
};


#endif //ASSIGNMENT3_RANDOM_H
