//
// Created by gupta on 21-04-2020.
//

#include "FIFO.h"
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

FIFO::FIFO(int framesize): queue(framesize) {
    this->hand = 0;
}

int FIFO::selectVictimFrame(){

    int framePointer = hand;
    hand++;
    if(hand > queue.size()){
        hand = 0;
    }
    return framePointer;
}

//void FIFO::addFrame(){
////    queue[hand] =
//}