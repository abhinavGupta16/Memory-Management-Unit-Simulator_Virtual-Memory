#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <getopt.h>

#include "utils/Helper.h"
#include "Process.h"
#include "utils/ReadFile.h"
#include "Pagers/Pager.h"
#include "FrameTableEntry.h"
#include "pagers/FIFO.h"
#include "utils/PageStats.h"

using namespace std;

vector<FrameTableEntry*> frameTable;
vector<FrameTableEntry*> freePool;
int framePointer = 0;
int frameTableSize = 16;
Pager* pager;
bool O = false;
bool frameTableOption = false;
bool pageTableOption = false;
bool sum = false;

FrameTableEntry* getFrame(Pager *pager);

void parseArguments(int argc, char *argv[]);

int main(int argc, char *argv[]) {

    parseArguments(argc, argv);
    string inputFilename = argv[optind];

    cout<<inputFilename<<endl;

    vector<Process*> processes;

    ReadFile *inputFile = new ReadFile(inputFilename);

    readInputFile(&processes, inputFile);

    initialiseFrameTable(frameTableSize, &frameTable, &freePool);

    pair<char, int> instruction;
    Process *process;
    FrameTableEntry* oldFTE;
    bool valid = false;
    int instCount = 0;
    int vno = -1;

    while(getInstruction(inputFile, instruction)) {
        printInstruction(instCount, instruction);
        instCount++;
        PageTableEntry *pte;

        switch(instruction.first) {
            case 'c' :
                process = processes.at(instruction.second);
                break;
            case 'e' :
                break;
            default:
                pte = getPageTableEntry(instruction, process);
                if(pte==nullptr){
                    //segfault
                    cout<<" SEGV"<<endl;
                    process->pageStats->segvCnt++;
                    continue;
                }
                vno = instruction.second;
                if(!pte->present){

                    FrameTableEntry *oldFTE = getFrame(pager);

                    if(oldFTE->process != nullptr){
                        printf(" UNMAP %d:%d\n", oldFTE->process->pid, oldFTE->virtualPageNumber);
                        process->pageStats->unmapCnt++;
                        if(oldFTE->process->pageTable.at(oldFTE->virtualPageNumber).modified){
                            printf(" OUT\n");
                            process->pageStats->pageoutCnt++;
                        }
                        if(oldFTE->process->pageTable.at(oldFTE->virtualPageNumber).fileMapped){
                            printf(" FOUT\n");
                            process->pageStats->pagefoutCnt++;
                        }

                        oldFTE->process->pageTable.at(oldFTE->virtualPageNumber).pagedout = 1;
                        oldFTE->process->pageTable.at(oldFTE->virtualPageNumber).present = 0;
                        oldFTE->virtualPageNumber = -1;
                        oldFTE->process = nullptr;
                    }
                    process->pageStats->mapCnt++;
                    if(pte->pagedout & pte->modified){
                        cout<< " IN"<<endl;
                        process->pageStats->pageinCnt++;
                    } else if (pte->pagedout & pte->fileMapped) {
                        cout<< " FIN"<<endl;
                        process->pageStats->pagefinCnt++;
                    } else {
                        cout << " ZERO" << endl;
                        process->pageStats->zeroOpCnt++;
                    }
                    oldFTE->process = process;
                    oldFTE->virtualPageNumber = instruction.second;

                    pte->present = 1;
                    pte->phyAddr = oldFTE->phyFrameNumber;
                    process->pageTable.at(vno);
                    cout<<" MAP " << oldFTE->phyFrameNumber<<endl;
                    process->pageStats->mapCnt++;
                }
                pte->referenced = 1;
                if (instruction.first == 'w'){
                    if(pte->writeProtect){
                        cout<<" SEGPROT"<< endl;
                        process->pageStats->segprotCnt++;
                    } else {
                        pte->modified = 1;
                    }
                }
            break;
        }
    }
    if(pageTableOption)
        printPageTable(&processes);

    if(frameTableOption)
        printFrameTable(&frameTable);

    if(O) {
        PageStats *pageStats;
        for (int i = 0; i < processes.size(); i++) {
            pageStats = processes.at(i)->pageStats;
            printf("PROC[%d]: U=%lu M=%lu I=%lu O=%lu FI=%lu FO=%lu Z=%lu SV=%lu SP=%lu\n",
                   processes.at(i)->pid,
                   pageStats->unmapCnt, pageStats->mapCnt, pageStats->pageinCnt, pageStats->pageoutCnt,
                   pageStats->pagefinCnt, pageStats->pagefoutCnt, pageStats->zeroOpCnt,
                   pageStats->segvCnt, pageStats->segprotCnt);
        }
    }
    return 0;
}

FrameTableEntry* getFrame(Pager *pager){
    if(freePool.size()>0){
        FrameTableEntry *fte = freePool.front();
        freePool.erase(freePool.begin());
        return fte;
    } else {
        return frameTable[pager->selectVictimFrame() % frameTable.size()];
    }
}


void parseArguments(int argc, char *argv[]){
    int c;
    bool pagerInitialized = false;
    while ((c = getopt (argc, argv, "f:o:a:")) != -1)
        switch (c)
        {
            case 'f':
                sscanf(optarg, "%d",&frameTableSize);
                break;
            case 'a':
                pagerInitialized = true;
                switch(optarg[0]) {
                    case 'f':
                        pager = new FIFO();
                        break;
                    default:
                        fputs("Error: invalid scheduler "
                              "specified\n\n", stderr);
                        exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                for(int i = 0; i < 4; i++) {
                    switch (optarg[i]) {
                        case 'O':
                            O = true;
                            break;
                        case 'F':
                            frameTableOption = true;
                            break;
                        case 'P':
                            pageTableOption = true;
                            break;
                        case 'S':
                            sum = true;
                            break;
                        default:
                            break;
                    }
                }
                break;
            default:
                abort ();
        }

    if(!pagerInitialized)
        pager = new FIFO();
}
