//
// Created by gupta on 21-04-2020.
//

#ifndef ASSIGNMENT3_FRAMETABLEENTRY_H
#define ASSIGNMENT3_FRAMETABLEENTRY_H


#include "Process.h"

class FrameTableEntry {
public:
    Process *process;
    int virtualPageNumber;
    int phyFrameNumber;
    FrameTableEntry(Process *process, int virtualPageNumber, int phyFrameNumber);

    // For Aging Algorithm
    unsigned int agingAlgoAge : 32;
    void resetAgingAlgoAge();

    // For Working Set Algorithm
    unsigned long long timeOfLastUse;
};


#endif //ASSIGNMENT3_FRAMETABLEENTRY_H
