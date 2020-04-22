//
// Created by gupta on 20-04-2020.
//

#ifndef ASSIGNMENT3_PAGETABLEENTRY_H
#define ASSIGNMENT3_PAGETABLEENTRY_H

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class PageTableEntry {
public:
    unsigned int present : 1;
    unsigned int writeProtect : 1;
    unsigned int modified : 1;
    unsigned int referenced : 1;
    unsigned int pagedout : 1;
    unsigned int phyAddr : 7;
    unsigned int fileMapped : 1;
    unsigned int vmaPresent : 1;
    PageTableEntry();
    void reset();
};


#endif //ASSIGNMENT3_PAGETABLEENTRY_H
