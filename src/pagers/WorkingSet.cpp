//
// Created by gupta on 23-04-2020.
//

#include "WorkingSet.h"
#include "../FrameTableEntry.h"
#include <bits/stdc++.h>

using namespace std;

WorkingSet::WorkingSet(){}

WorkingSet::WorkingSet(vector<FrameTableEntry*> *frameTable, unsigned long long *instructionCnt) {
    this->hand = 0;
    this->frameTable = frameTable;
    this->instructionCnt = instructionCnt;
    this->threshold = 49;
}

FrameTableEntry* WorkingSet::selectVictimFrame(){
    int leastAgeIndex = hand;
    int startPos = hand;
    bool loopComplete = false;
    unsigned long long currentAge;
    FrameTableEntry *victimFrame = frameTable->at(hand);
    FrameTableEntry *currentFrame = frameTable->at(hand);
    while(!loopComplete) {
        if (((hand + 1) % frameTable->size()) == startPos) {
            loopComplete = true;
        }

        currentFrame = frameTable->at(hand);
        currentAge = (*instructionCnt)+1 - currentFrame->timeOfLastUse;

        if(currentFrame->process->pageTable.at(currentFrame->virtualPageNumber).referenced){
            currentFrame->timeOfLastUse = (*instructionCnt)+1;
            currentFrame->process->pageTable.at(currentFrame->virtualPageNumber).referenced = 0;
        } else {
            if((currentAge <= threshold && ((*instructionCnt)+1 - victimFrame->timeOfLastUse) < currentAge)) {
                victimFrame = currentFrame;
                leastAgeIndex = hand;
            } else if (currentAge > threshold){
                victimFrame = currentFrame;
                leastAgeIndex = hand;
                break;
            }
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