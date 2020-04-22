//
// Created by gupta on 21-04-2020.
//

#include "FIFO.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

FIFO::FIFO() {
    this->hand = 0;
}

int FIFO::selectVictimFrame(){

    int framePointer = hand;
    hand++;
    return framePointer;
}