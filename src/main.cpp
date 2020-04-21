#include <iostream>
#include <bits/stdc++.h>
#include <string>

#include "utils/Helper.h"
#include "Process.h"
#include "utils/ReadFile.h"
#include "Pagers/Pager.h"
#include "FrameTableEntry.h"
#include "pagers/FIFO.h"

using namespace std;

vector<FrameTableEntry*> frameTable;
int framePointer = 0;

int getFrame(Pager *pager);

int main(int argc, char *argv[]) {
    string inputFilename = argv[1];

    vector<Process*> processes;
//    vector<Instruction*> instructions;

    ReadFile *inputFile = new ReadFile(inputFilename);

    readInputFile(processes, inputFile);
//    PageTableEntry *pte = new PageTableEntry();
//
//    if(pte->modified){
//       cout<<"ooh"<<endl;
//    }
//
//    pte->modified = 1;
//
//    if(pte->modified){
//        cout<<"yay set"<<endl;
//    }
//
//    cout<<pte->phyAddr << endl;
//
//    pte->phyAddr = 45;
//
//    cout<<pte->phyAddr << endl;
//
//    if(processes.at(0)->pageTable.at(0).vmaPresent){
//        cout<< "yay found"<<endl;
//    }
//
//    if(processes.at(0)->pageTable.at(63).vmaPresent){
//        cout<< "kalesh"<<endl;
//    }


    int frameTableSize = 16;
    Pager *pager = new FIFO(frameTableSize);

    initialiseFrameTable(frameTableSize, &frameTable);

    pair<char, int> instruction;
    Process *process;
    FrameTableEntry* oldFTE = nullptr;
    int instCount = 0;
    int vno = -1;
    int fptr = -1;
    while(getInstruction(inputFile, instruction)) {
        printInstruction(instCount, instruction);
        PageTableEntry *pte = nullptr;
        switch(instruction.first) {
            case 'c' :
                process = processes.at(instruction.second);
                break;
            case 'e' :
                break;
            case 'r' || 'w' :
                pte = getPageTableEntry(instruction, process);
                vno = instruction.second;
                if(pte==nullptr){
                    continue;
                }
                if(!pte->present){
                    fptr = getFrame(pager);

                    FrameTableEntry *oldFTE = frameTable[fptr];

                    if(oldFTE != nullptr){
                        printf(" UNMAP %d:%d", fptr, oldFTE->virtualPageNumber);
                        if(oldFTE->process->pageTable.at(oldFTE->virtualPageNumber).modified){
                            printf(" OUT");
                        }
                        if(oldFTE->process->pageTable.at(oldFTE->virtualPageNumber).fileMapped){
                            printf(" FOUT");
                        }
                    } else {
                        cout<< " ZERO"<<endl;
                    }

                    frameTable[fptr] = new FrameTableEntry(process, instruction.second);
                    pte->phyAddr = fptr;
                    process->pageTable.at(vno);
                    cout<<" MAP " << fptr<<endl;
                }
                if(instruction.first == 'r'){
                    pte->referenced = 1;
                } else if (instruction.first == 'w'){
                    pte->modified = 1;
                }
            break;
        }
    }


    cout << "Hello, World!" << endl;
    return 0;
}

int getFrame(Pager *pager){
    if(framePointer < 64){
        return framePointer++;
    } else {
        return pager->selectVictimFrame();
    }
}

//void processInstruction(PageTableEntry *pte, pair<char, int> &instruction, Process *process, int fptr, Pager *pager){
//    pte = getPageTableEntry(instruction, process);
//    int vno = instruction.second;
//    if(pte==nullptr){
//        continue;
//    }
//    if(!pte->present){
//        fptr = getFrame(pager);
//
//        FrameTableEntry *oldFTE = frameTable[fptr];
//
//        if(oldFTE != nullptr){
//            printf(" UNMAP %d:%d", fptr, oldFTE->virtualPageNumber);
//            if(oldFTE->process->pageTable.at(oldFTE->virtualPageNumber).modified){
//                printf(" OUT");
//            }
//            if(oldFTE->process->pageTable.at(oldFTE->virtualPageNumber).fileMapped){
//                printf(" FOUT");
//            }
//        } else {
//            cout<< " ZERO"<<endl;
//        }
//
//        frameTable[fptr] = new FrameTableEntry(process, instruction.second);
//        cout<<" MAP " << fptr<<endl;
//        process->pageTable.at(vno);
//    }
//
//}
