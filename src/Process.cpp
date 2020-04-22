//
// Created by gupta on 19-04-2020.
//

#include "Process.h"

Process::Process(int pid) : pageTable(64){
    this->pid = pid;
    this->pageStats = new PageStats();
}