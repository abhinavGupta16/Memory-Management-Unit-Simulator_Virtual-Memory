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
}

FrameTableEntry* Aging::selectVictimFrame(){
    int leastAgeIndex = hand;
    int startPos = hand;
    bool loopComplete = false;
    FrameTableEntry *victimFrame = frameTable->at(hand);
    FrameTableEntry *currentFrame = frameTable->at(hand);
    while(!loopComplete) {
        if (((hand + 1) % frameTable->size()) == startPos) {
            loopComplete = true;
        }

        currentFrame = frameTable->at(hand);
        currentFrame->agingAlgoAge = currentFrame->agingAlgoAge >> 1;

        if(currentFrame->process->pageTable.at(currentFrame->virtualPageNumber).referenced){
            currentFrame->agingAlgoAge = (currentFrame->agingAlgoAge | 0x80000000);
            currentFrame->process->pageTable.at(currentFrame->virtualPageNumber).referenced = 0;
        }

        if(currentFrame->agingAlgoAge < victimFrame->agingAlgoAge){
            victimFrame = currentFrame;
            leastAgeIndex = hand;
        }

        hand++;
        if(hand == frameTable->size())
            hand = 0;

    }

    hand = leastAgeIndex;
    hand++;

    if(hand == frameTable->size())
        hand = 0;

    return victimFrame;
}