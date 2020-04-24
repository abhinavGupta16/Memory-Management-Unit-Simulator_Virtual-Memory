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

FrameTableEntry* Aging::selectVictimFrame(){
    int leastAgeIndex = hand;
    int startPos = hand;
    bool loopComplete = false;
    FrameTableEntry *victimFrame = frameTable->at(hand);
    FrameTableEntry *currentFrame = frameTable->at(hand);
    victimFrameAge = victimFrame->age;
    while(!loopComplete) {
        if (((hand + 1) % frameTable->size()) == startPos) {
            loopComplete = true;
        }

        currentFrame = frameTable->at(hand);

//        cout << "before ";
//        printBits(currentFrame->age);
        if(victimFrameAge > currentFrame->age){
            victimFrame = currentFrame;
            victimFrameAge = victimFrame->age;
            leastAgeIndex = hand;
        }

        currentFrame->age = currentFrame->age >> 1;

        if(currentFrame->process->pageTable.at(currentFrame->virtualPageNumber).referenced){
            currentFrame->age = (currentFrame->age | 0x80000000);
        }
        currentFrame->process->pageTable.at(currentFrame->virtualPageNumber).referenced = 0;
//        cout << "after ";
//        printBits(currentFrame->age);
        hand++;
        if(hand == frameTable->size())
            hand = 0;

    }

    hand = leastAgeIndex;
    hand++;

    if(hand == frameTable->size())
        hand = 0;

    victimFrame->resetAge();
    victimFrame->age = (victimFrame->age | 0x80000000);

    return victimFrame;
}