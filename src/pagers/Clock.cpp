//
// Created by gupta on 23-04-2020.
//

#include "Clock.h"
#include "Pager.h"
#include "../FrameTableEntry.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

Clock::Clock(vector<FrameTableEntry*> *frameTable) {
    this->hand = 0;
    this->frameTable = frameTable;
}

void Clock::incrementHand(int &hand){
    hand++;
    if(hand == frameTable->size()){
        hand = 0;
    }
}

FrameTableEntry* Clock::selectVictimFrame(){
    while(frameTable->at(hand)->process->pageTable.at(frameTable->at(hand)->virtualPageNumber).referenced){
        frameTable->at(hand)->process->pageTable.at(frameTable->at(hand)->virtualPageNumber).referenced = 0;
        incrementHand(hand);
    }
    int framePointer = hand;
    incrementHand(hand);
    return frameTable->at(framePointer);
}

