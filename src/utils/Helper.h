//
// Created by gupta on 18-04-2020.
//

#ifndef ASSIGNMENT3_HELPER_H
#define ASSIGNMENT3_HELPER_H

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "../Process.h"
#include "../Instruction.h"
#include "ReadFile.h"
#include "../FrameTableEntry.h"

using namespace std;

void readInputFile(vector<Process*> &processes, ReadFile *inputFile);
bool getInstruction(ReadFile *inputFile, pair<char, int> &instruction);
void printInstruction(int &instCount, pair<char, int> &instruction);
bool checkValidPage(int virtualPage, vector<VirtualMemoryAddress*> *vmas, VirtualMemoryAddress *vma);
PageTableEntry* getPageTableEntry(pair<char, int> instruction, Process *process);
void initialiseFrameTable(int frameTableSize, vector<FrameTableEntry*> *frameTable);
#endif //ASSIGNMENT3_HELPER_H