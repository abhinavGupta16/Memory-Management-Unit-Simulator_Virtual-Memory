//
// Created by gupta on 23-04-2020.
//

#include "NotRecentlyUsed.h"
#include "Pager.h"
#include "../FrameTableEntry.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

NotRecentlyUsed::NotRecentlyUsed(){}

NotRecentlyUsed::NotRecentlyUsed(vector<FrameTableEntry*> *frameTable, unsigned long long *instructionCount) {
    this->hand = 0;
    this->frameTable = frameTable;
    this->instructionCount = instructionCount;
    this->resetCounter = 50;
}

FrameTableEntry* NotRecentlyUsed::selectVictimFrame(){
    PageTableEntry *pte;
    FrameTableEntry *victimFrame;
    bool class0 = false;
    bool class1 = false;
    bool class2 = false;
    bool class3 = false;
    bool loopComplete = false;
    bool reset = false;
    int startPos = hand;
    int selectedHand = hand;
    if((*instructionCount)+1 >= resetCounter) {
        resetCounter = (*instructionCount)+1 + 50ULL;
        reset = true;
    }
    while(!loopComplete){
        if(((hand+1)%frameTable->size()) == startPos){
            loopComplete = true;
        }
        pte = &(frameTable->at(hand)->process->pageTable.at(frameTable->at(hand)->virtualPageNumber));
        if(!pte->referenced & !pte->modified && !class0){
            victimFrame = frameTable->at(hand);
            selectedHand = hand;
            class0 = true;
            if(!reset)
                break;
        } else if(!pte->referenced & pte->modified && !class0 && !class1){
            victimFrame = frameTable->at(hand);
            class1 = true;
            selectedHand = hand;
        } else if(pte->referenced & !pte->modified && !class0 && !class1 && !class2){
            victimFrame = frameTable->at(hand);
            class2 = true;
            selectedHand = hand;
        } else if(pte->referenced & pte->modified && !class0 && !class1 && !class2 && !class3){
            victimFrame = frameTable->at(hand);
            class3 = true;
            selectedHand = hand;
        }
        if(reset){
            pte->referenced = 0;
        }
        hand++;
        if(hand == frameTable->size())
            hand = 0;
    }


    hand = selectedHand;
    hand++;
    if(hand == frameTable->size())
        hand = 0;

    return victimFrame;
}