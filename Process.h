//
// Created by gupta on 19-04-2020.
//

#ifndef ASSIGNMENT3_PROCESS_H
#define ASSIGNMENT3_PROCESS_H

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "VirtualMemoryAddress.h"

using namespace std;

class Process {
public:
    vector<VirtualMemoryAddress*> vmas;
    Process();
};


#endif //ASSIGNMENT3_PROCESS_H
