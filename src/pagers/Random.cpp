//
// Created by gupta on 23-04-2020.
//

#include "Random.h"
#include "../FrameTableEntry.h"
#include "../utils/Helper.h"
#include <iostream>
#include <bits/stdc++.h>

Random::Random(){}

Random::Random(vector<FrameTableEntry*> *frameTable, vector<int> *randvals){
    this->frameTable = frameTable;
    this->randvals = randvals;
    this->hand = 0;
    this->randomCounter = 0;
}

FrameTableEntry* Random::selectVictimFrame(){
    hand = myrandom(randvals, randomCounter, frameTable->size());
    return frameTable->at(hand);
}