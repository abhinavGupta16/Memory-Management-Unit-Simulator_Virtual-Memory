//
// Created by gupta on 22-04-2020.
//

#ifndef ASSIGNMENT3_PAGESTATS_H
#define ASSIGNMENT3_PAGESTATS_H

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class PageStats {
public:
    unsigned long long segvCnt;
    unsigned long long segprotCnt;
    unsigned long long unmapCnt;
    unsigned long long mapCnt;
    unsigned long long pageinCnt;
    unsigned long long pageoutCnt;
    unsigned long long pagefinCnt;
    unsigned long long pagefoutCnt;
    unsigned long long zeroOpCnt;
    PageStats();
};


#endif //ASSIGNMENT3_PAGESTATS_H
