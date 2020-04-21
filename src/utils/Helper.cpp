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

void readInputFile(vector<Process*> &processes, ReadFile *inputFile){
    string line;

    int numOfProcess = stoi(inputFile->readLine());

    for(int i = 0; i<numOfProcess;){
        int numberOfVma = stoi(inputFile->readLine());
        Process *process = new Process();
        while(numberOfVma > 0) {
//            VirtualMemoryAddress *vma = readVmas(inputFile);
            process->vmas.push_back(readVmas(inputFile));
//            for(int i = vma->start_vpage; i <= vma->end_vpage; i++){
//                process->pageTable.at(i).vmaPresent = 1;
//            }
            numberOfVma--;
        }

        processes.push_back(process);
        i++;
    }
}

bool getInstruction(ReadFile *inputFile, pair<char, int> &instruction){
    string line = inputFile->readLine();

    if(inputFile->eof())
        return false;

    vector<string> tokens;
    convertStringToTokens(line, &tokens);
    instruction = make_pair(tokens[0].at(0), stoi(tokens[1]));
    return true;
}



void printInstruction(int &instCount, pair<char, int> &instruction){
    printf("%d: ==> %c %d", instCount, instruction.first, instruction.second);
    instCount++;
}

PageTableEntry* getPageTableEntry(pair<char, int> instruction, Process *process){
    int virtualPage = instruction.second;
    VirtualMemoryAddress *vma;
    if(checkValidPage(virtualPage, &(process->vmas), vma)){
        //segfault
        cout<<" SEGV"<<endl;
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

bool checkValidPage(int virtualPage, vector<VirtualMemoryAddress*> *vmas, VirtualMemoryAddress *vma){
    for(int i = 0; i < vmas->size(); i++){
        if(vmas->at(i)->start_vpage <= virtualPage && vmas->at(i)->end_vpage >= virtualPage){
            vma = vmas->at(i);
            return true;
        }
    }
    return false;
}

void initialiseFrameTable(int frameTableSize, vector<FrameTableEntry*> *frameTable){
    for(int i = 0; i < frameTableSize; i++){
        frameTable->push_back(nullptr);
    }
}