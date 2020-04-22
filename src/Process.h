//
// Created by gupta on 19-04-2020.
//

#ifndef ASSIGNMENT3_PROCESS_H
#define ASSIGNMENT3_PROCESS_H

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "VirtualMemoryAddress.h"
#include "PageTableEntry.h"
#include "utils/PageStats.h"

using namespace std;

class Process {
public:
    vector<VirtualMemoryAddress*> vmas;
    vector<PageTableEntry> pageTable;
    int pid;
    PageStats *pageStats;
    Process(int pid);
};


#endif //ASSIGNMENT3_PROCESS_H
