//
// Created by gupta on 18-04-2020.
//

#include "Helper.h"

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "../Process.h"
#include "../VirtualMemoryAddress.h"
#include "../Instruction.h"
#include "../FrameTableEntry.h"

using namespace std;


//void convertStringToTokens(string line, vector<string> *tokens){
//    size_t prev = 0, pos;
//    while ((pos = line.find_first_of(" ", prev)) != string::npos) {
//        if (pos > prev)
//            tokens->push_back(line.substr(prev, pos - prev));
//        prev = pos + 1;
//    }
//    if (prev < line.length())
//        tokens->push_back(line.substr(prev, string::npos));
//}
//
//VirtualMemoryAddress* readVmas(ifstream &input) {
//    string line;
//    vector<string> tokens;
//    while(getline(input, line) && line.at(0)== '#');
//    convertStringToTokens(line, &tokens);
//    return new VirtualMemoryAddress(stoi(tokens[0]), stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3]));
//}
//
//void readInputFile(string filename, vector<Process*> &processes, vector<Instruction*> &instructions){
//    ifstream input(filename);
//    string line;
//    vector<string> tokens;
//
//    int numOfProcess = 0;
//    while(getline(input, line)){
//        if(line.at(0) == '#')
//            continue;
//        else{
//            numOfProcess = stoi(line);
//            break;
//        }
//    }
//
//    for(int i = 0; i<numOfProcess;){
//        getline(input, line);
//        if(line.at(0) == '#')
//            continue;
//        int numberOfVma = stoi(line);
//        Process *process = new Process();
//        while(numberOfVma > 0) {
//            process->vmas.push_back(readVmas(input));
//            numberOfVma--;
//        }
//        processes.push_back(process);
//        i++;
//    }
//
//    while(getline(input, line)){
//        if(line.at(0)=='#'){
//            continue;
//        }
//        convertStringToTokens(line, &tokens);
//        Instruction *instruction = new Instruction(tokens[0][0], stoi(tokens[1]));
//        instructions.push_back(instruction);
//        tokens.clear();
//    }
//}

/////////////////////////////////////////////////////////////////////////////////////////

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

//void readInstructions(ReadFile *inputFile, vector<Instruction*> *instructions){
//    string line;
//    vector<string> tokens;
//    while(getline(input, line)){
//        if(line.at(0)=='#'){
//            continue;
//        }
//        convertStringToTokens(line, &tokens);
//        Instruction *instruction = new Instruction(tokens[0][0], stoi(tokens[1]));
//        instructions->push_back(instruction);
//        tokens.clear();
//    }
//}

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

//    pair<char, int> *ptr = new pair<char,int>;
//    ptr->first = tokens[0].at(0);
//    ptr->second = stoi(tokens[1]);
//    valid = true;
    instruction = make_pair(tokens[0].at(0), stoi(tokens[1]));
    return true;
}



void printInstruction(int instCount, pair<char, int> instruction){
//    printf("%d: ==> %c %d\n", instCount, instruction.first, instruction.second);
    cout<< instCount << ": ==> " << instruction.first << " " << instruction.second << endl;
//    (*instCount)++;
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
       pte->vmaPresent = vma->write_protected;
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

void printPageTable(vector<Process*> *processes) {
    PageTableEntry pte;
    for(int i = 0; i < processes->size(); i++){
        printf("PT[%d]:", i);
        for(int j = 0; j < processes->at(i)->pageTable.size(); j++){
            pte = processes->at(i)->pageTable.at(j);
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
            } else if(pte.pagedout & pte.modified & pte.referenced){
                cout<<" #";
            } else {
                cout<< " *";
            }
        }
    }
    cout<< " " << endl;
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

void printProcessStats(vector<Process*> *processes, unsigned long long instCount){
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

        printf("PROC[%d]: U=%llu M=%llu I=%llu O=%llu FI=%llu FO=%llu Z=%llu SV=%llu SP=%llu\n",
               processes->at(i)->pid,
               pageStats->unmapCnt, pageStats->mapCnt, pageStats->pageinCnt, pageStats->pageoutCnt,
               pageStats->pagefinCnt, pageStats->pagefoutCnt, pageStats->zeroOpCnt,
               pageStats->segvCnt, pageStats->segprotCnt);
    }
    printf("TOTALCOST %llu %llu %llu %llu\n", instCount, ctxSwitches, processExits, cycles);
}