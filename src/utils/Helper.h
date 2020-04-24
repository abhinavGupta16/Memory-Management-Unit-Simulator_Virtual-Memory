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

//void readInputFile(string filename, vector<Process*> &processes, vector<Instruction*> &instructions);

void readInputFile(vector<Process*> *processes, ReadFile *inputFile);
bool getInstruction(ReadFile *inputFile, pair<char,int> &instruction);
void printInstruction(int instCount, pair<char, int> instruction);
bool checkValidPage(int virtualPage, Process* process, VirtualMemoryAddress* &vma);
PageTableEntry* getPageTableEntry(pair<char, int> instruction, Process *process);
void initialiseFrameTable(int frameTableSize, vector<FrameTableEntry*> *frameTable, vector<FrameTableEntry*> *freePool);
void printPageTable(vector<Process*> *processes);
void printFrameTable(vector<FrameTableEntry*> *frameTable);
void printProcessStats(vector<Process*> *processes, unsigned long long instCount, bool processOption, bool sumOption);
void printPageTableForProcess(Process *process);
void exitProcess(Process *process,vector<FrameTableEntry*> *frameTable, vector<FrameTableEntry*> *freePool);
void unmapPage(FrameTableEntry* oldFTE, bool pageExit);
void readRandomFile(string filename, vector<int> *randvals);
int myrandom(vector<int> *randvals, int &randomCounter, int hand);
#endif //ASSIGNMENT3_HELPER_H