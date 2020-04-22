//
// Created by gupta on 21-04-2020.
//

#include "FrameTableEntry.h"

FrameTableEntry::FrameTableEntry(Process *process, int virtualPageNumber, int phyFrameNumber){
    this->process = process;
    this->virtualPageNumber = virtualPageNumber;
    this->phyFrameNumber = phyFrameNumber;
}