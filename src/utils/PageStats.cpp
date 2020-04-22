//
// Created by gupta on 22-04-2020.
//

#include "PageStats.h"

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

PageStats::PageStats(){
    this->segvCnt = 0;
    this->segprotCnt = 0;
    this->unmapCnt = 0;
    this->mapCnt = 0;
    this->pageinCnt = 0;
    this->pageoutCnt = 0;
    this->pagefinCnt = 0;
    this->pagefoutCnt = 0;
    this->zeroOpCnt = 0;
    this->accessCnt = 0;
    this->contextCnt = 0;
    this->processExitCnt = 0;
}