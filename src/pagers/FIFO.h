//
// Created by gupta on 21-04-2020.
//

#ifndef ASSIGNMENT3_FIFO_H
#define ASSIGNMENT3_FIFO_H

#include "Pager.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class FIFO : public Pager{
private:
    int hand;
public:
    int selectVictimFrame() override;
    FIFO();
};


#endif //ASSIGNMENT3_FIFO_H
