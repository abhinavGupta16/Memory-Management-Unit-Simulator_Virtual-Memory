//
// Created by gupta on 21-04-2020.
//

#include "FIFO.h"
#include "../FrameTableEntry.h"
#include <bits/stdc++.h>

using namespace std;

FIFO::FIFO(vector<FrameTableEntry*> *frameTable) {
    this->hand = 0;
    this->frameTable = frameTable;
}

FrameTableEntry* FIFO::selectVictimFrame(){
    int framePointer = hand;
    hand++;
    if(hand == frameTable->size()){
        hand = 0;
    }
    return frameTable->at(framePointer);
}