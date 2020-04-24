//
// Created by gupta on 23-04-2020.
//

#include "Aging.h"
#include "../FrameTableEntry.h"
#include "../utils/Helper.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

Aging::Aging(vector<FrameTableEntry*> *frameTable) {
    this->hand = 0;
    this->frameTable = frameTable;
    for(int i = 0; i < frameTable->size(); i++){
        bitset<32> b;
        this->agingVector.push_back(b);
    }
}

FrameTableEntry* Aging::selectVictimFrame(){
    FrameTableEntry *victimFrame;

    return victimFrame;
}