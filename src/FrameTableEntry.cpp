//
// Created by gupta on 21-04-2020.
//

#include "FrameTableEntry.h"

FrameTableEntry::FrameTableEntry(Process *process, int virtualPageNumber, int phyFrameNumber){
    this->process = process;
    this->virtualPageNumber = virtualPageNumber;
    this->phyFrameNumber = phyFrameNumber;
    this->agingAlgoAge = 0;
    this->timeOfLastUse = 0;
}

void FrameTableEntry::resetAgingAlgoAge(){
    this->agingAlgoAge = 0;
}