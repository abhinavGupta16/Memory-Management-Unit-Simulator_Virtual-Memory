//
// Created by gupta on 18-04-2020.
//

#include "Helper.h"

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "../Process.h"
#include "../VirtualMemoryAddress.h"
#include "../FrameTableEntry.h"

using namespace std;

void convertStringToTokens(string line, vector<string> *tokens){
    size_t prev = 0, pos;
    while ((pos = line.find_first_of(" ", prev)) != string::npos) {
        if (pos > prev)
            tokens->push_back(line.substr(prev, pos - prev));
        prev = pos + 1;
    }
    if (prev < line.length())
        tokens->push_back(line.substr(prev, string::npos));
}

VirtualMemoryAddress* readVmas(ReadFile *inputFile) {
    string line = inputFile->readLine();
    vector<string> tokens;
    convertStringToTokens(line, &tokens);
    return new VirtualMemoryAddress(stoi(tokens[0]), stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3]));
}

void readInputFile(vector<Process*> *processes, ReadFile *inputFile){
    string line;

    int numOfProcess = stoi(inputFile->readLine());

    for(int i = 0; i<numOfProcess;){
        int numberOfVma = stoi(inputFile->readLine());
        Process *process = new Process(i);
        while(numberOfVma > 0) {
            process->vmas.push_back(readVmas(inputFile));
            numberOfVma--;
        }

        processes->push_back(process);
        i++;
    }
}

bool getInstruction(ReadFile *inputFile, pair<char,int> &instruction){
    string line = inputFile->readLine();
    if(inputFile->eof()){
        return false;
    }
    vector<string> tokens;
    convertStringToTokens(line, &tokens);
    instruction = make_pair(tokens[0].at(0), stoi(tokens[1]));
    return true;
}



void printInstruction(int instCount, pair<char, int> instruction){
    printf("%d: ==> %c %d\n", instCount, instruction.first, instruction.second);
}

PageTableEntry* getPageTableEntry(pair<char, int> instruction, Process *process){
    int virtualPage = instruction.second;
    VirtualMemoryAddress *vma;
    if(!checkValidPage(virtualPage, process, vma)){
        return nullptr;
    }


    PageTableEntry *pte = &(process->pageTable.at(instruction.second));
    // Initialize PTE entry
    if(!pte->vmaPresent){
       pte->writeProtect = vma->write_protected;
       pte->fileMapped = vma->file_mapped;
    }
    return pte;
}

bool checkValidPage(int virtualPage, Process *process, VirtualMemoryAddress* &vma){

    for(int i = 0; i < process->vmas.size(); i++){
        if(process->vmas.at(i)->start_vpage <= virtualPage && process->vmas.at(i)->end_vpage >= virtualPage){
            vma = process->vmas.at(i);
            return true;
        }
    }
    return false;
}

void initialiseFrameTable(int frameTableSize, vector<FrameTableEntry*> *frameTable, vector<FrameTableEntry*> *freePool){
    for(int i = 0; i < frameTableSize; i++){
        FrameTableEntry *frameTableEntry = new FrameTableEntry(nullptr, -1, i);
        frameTable->push_back(frameTableEntry);
        freePool->push_back(frameTableEntry);
    }
}

void printPageTableForProcess(Process *process){
    PageTableEntry pte;
    printf("PT[%d]:", process->pid);
    for(int j = 0; j < process->pageTable.size(); j++){
        pte = process->pageTable.at(j);
        if(pte.present) {
            printf(" %d:", j);
            if (pte.referenced) {
                cout << "R";
            } else {
                cout << "-";
            }
            if (pte.modified) {
                cout << "M";
            } else {
                cout << "-";
            }
            if (pte.pagedout) {
                cout << "S";
            } else {
                cout << "-";
            }
        } else if(pte.pagedout){
            cout<<" #";
        } else {
            cout<< " *";
        }
    }
    cout<< " " << endl;
}

void printPageTable(vector<Process*> *processes) {
    for(int i = 0; i < processes->size(); i++){
        printPageTableForProcess(processes->at(i));
    }
}

void printFrameTable(vector<FrameTableEntry*> *frameTable){
    FrameTableEntry *fte;
    cout<<"FT:";
    for(int i = 0; i < frameTable->size(); i++){
        fte = frameTable->at(i);
        if (fte->virtualPageNumber == -1){
            cout<<" *";
        } else {
            printf(" %d:%d", fte->process->pid, fte->virtualPageNumber);
        }
    }
    cout << " " << endl;
}

void printProcessStats(vector<Process*> *processes, unsigned long long instCount, bool processOption, bool sumOption){
    PageStats *pageStats;
    unsigned long long cycles = 0LL;
    unsigned long long ctxSwitches = 0LL;
    unsigned long long processExits = 0LL;
    for (int i = 0; i < processes->size(); i++) {
        pageStats = processes->at(i)->pageStats;
        cycles += ((pageStats->mapCnt * 400) + (pageStats->unmapCnt * 400) + (pageStats->pageinCnt * 3000) + (pageStats->pageoutCnt * 3000)
                + (pageStats->pagefinCnt * 2500) + (pageStats->pagefoutCnt * 2500) + (pageStats->zeroOpCnt * 150) + (pageStats->segvCnt * 240)
                + (pageStats->segprotCnt * 300) + (pageStats->accessCnt * 1) + (pageStats->contextCnt * 121) + (pageStats->processExitCnt * 175));

        ctxSwitches += pageStats->contextCnt;
        processExits += pageStats->processExitCnt;

        if(processOption)
            printf("PROC[%d]: U=%llu M=%llu I=%llu O=%llu FI=%llu FO=%llu Z=%llu SV=%llu SP=%llu\n",
                   processes->at(i)->pid,
                   pageStats->unmapCnt, pageStats->mapCnt, pageStats->pageinCnt, pageStats->pageoutCnt,
                   pageStats->pagefinCnt, pageStats->pagefoutCnt, pageStats->zeroOpCnt,
                   pageStats->segvCnt, pageStats->segprotCnt);
    }
    if(sumOption)
        printf("TOTALCOST %llu %llu %llu %llu\n", instCount, ctxSwitches, processExits, cycles);
}

void unmapPage(FrameTableEntry* oldFTE, bool pageExit){
    Process *oldProcess = oldFTE->process;
    printf(" UNMAP %d:%d\n", oldProcess->pid, oldFTE->virtualPageNumber);
    oldProcess->pageStats->unmapCnt++;
    if(!pageExit && oldProcess->pageTable.at(oldFTE->virtualPageNumber).modified &
       !oldProcess->pageTable.at(oldFTE->virtualPageNumber).fileMapped){
        printf(" OUT\n");
        oldProcess->pageStats->pageoutCnt++;
    }
    if(oldProcess->pageTable.at(oldFTE->virtualPageNumber).fileMapped &
       oldProcess->pageTable.at(oldFTE->virtualPageNumber).modified){
        printf(" FOUT\n");
        oldProcess->pageStats->pagefoutCnt++;
    }

    if(!pageExit && oldProcess->pageTable.at(oldFTE->virtualPageNumber).modified & !oldProcess->pageTable.at(oldFTE->virtualPageNumber).fileMapped ) {
        oldProcess->pageTable.at(oldFTE->virtualPageNumber).pagedout = 1;
    }
    oldProcess->pageTable.at(oldFTE->virtualPageNumber).present = 0;
    oldFTE->virtualPageNumber = -1;
    oldFTE->process = nullptr;
}

void exitProcess(Process *process,vector<FrameTableEntry*> *frameTable, vector<FrameTableEntry*> *freePool){
    PageTableEntry *pte;
    cout << "EXIT current process " << process->pid << endl;
    for(int i = 0; i < process->pageTable.size(); i++){
        pte = &(process->pageTable.at(i));
        pte->pagedout = 0;
        if(pte->present){
            int fteptr = pte->phyAddr;
            FrameTableEntry *oldFTE = frameTable->at(fteptr);
            unmapPage(oldFTE, true);
            freePool->push_back(oldFTE);
        }
    }
}

void readRandomFile(string filename, vector<int> *randvals){
    ifstream input(filename);
    string line;
    getline(input, line);
    while(getline(input, line)) {
        randvals->push_back(stoi(line));
    }
}

int myrandom(vector<int> *randvals, int &randomCounter, int hand) {
    if (randomCounter == randvals->size())
        randomCounter = 0;
    return ((randvals->at(randomCounter++)) % hand);
}

bool bitlessthan(std::bitset<32> b1, std::bitset<32> b2) {
    for (int i =31; i > -1; i--) {
        if (b1[i] == b2[i]) {
            continue;
        }
        if (b1[i] < b2[i]) {
            return true;
        }
        if (b1[i] > b2[i]) {
            return false;
        }
    }
    return false;
}