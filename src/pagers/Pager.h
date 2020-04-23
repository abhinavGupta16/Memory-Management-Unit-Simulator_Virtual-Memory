//
// Created by gupta on 20-04-2020.
//

#ifndef ASSIGNMENT3_PAGER_H
#define ASSIGNMENT3_PAGER_H

#include "../FrameTableEntry.h"

class Pager {
public:
    virtual FrameTableEntry* selectVictimFrame() = 0;
};


#endif //ASSIGNMENT3_PAGER_H
