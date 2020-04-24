//
// Created by gupta on 23-04-2020.
//

#include "WorkingSet.h"
#include "../FrameTableEntry.h"
#include "../utils/Helper.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

WorkingSet::WorkingSet(){}

WorkingSet::WorkingSet(vector<FrameTableEntry*> *frameTable, unsigned long long *instructionCnt) {
    this->hand = 0;
    this->frameTable = frameTable;
    this->instructionCnt = instructionCnt;
    this->threshold = 49;
}

void printBits(unsigned int x) {
    if(x == 0){
        cout<<"0";
    }

    for (; x != 0; x >>= 1){
        if (x & 01)
            cout << "1";
        else
            cout << "0";
    }
    cout<<endl;
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