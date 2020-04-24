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
#include "pagers/Clock.h"
#include "pagers/Random.h"
#include "pagers/NotRecentlyUsed.h"
#include "pagers/Aging.h"

using namespace std;

vector<FrameTableEntry*> frameTable;
vector<FrameTableEntry*> freePool;
int framePointer = 0;
int frameTableSize = 16;
Pager* pager;
bool processOption = false;
bool frameTableOption = false;
bool pageTableOption = false;
bool sumOption = false;
bool xOption = false;
bool yOption = false;

FrameTableEntry* getFrame(Pager *pager);

void parseArguments(int argc, char *argv[]);

int main(int argc, char *argv[]) {

    parseArguments(argc, argv);
    string inputFilename = argv[optind];
    string randomFilename = argv[optind+1];

    vector<Process*> processes;

    vector<int> randvals;

    ReadFile *inputFile = new ReadFile(inputFilename);

    readRandomFile(randomFilename, &randvals);

    readInputFile(&processes, inputFile);

    initialiseFrameTable(frameTableSize, &frameTable, &freePool);

    pair<char, int> instruction;
    Process *process;
    unsigned long long instCount = 0;

    if(Random* t = dynamic_cast<Random*>(pager)){
        pager = new Random(&frameTable, &randvals);
    } else if(NotRecentlyUsed* t = dynamic_cast<NotRecentlyUsed*>(pager)){
        pager = new NotRecentlyUsed(&frameTable, &instCount);
    } else if(Aging* t = dynamic_cast<Aging*>(pager)){
        pager = new Aging(&frameTable);
    }

    while(getInstruction(inputFile, instruction)) {
        printInstruction(instCount, instruction);
        PageTableEntry *pte;
//        instCount++;
        switch(instruction.first) {
            case 'c' :
                process = processes.at(instruction.second);
                process->pageStats->contextCnt++;
                break;
            case 'e' :
                process->pageStats->processExitCnt++;
                exitProcess(process, &frameTable, &freePool);
                break;
            default:
                process->pageStats->accessCnt++;
                pte = getPageTableEntry(instruction, process);
                if(pte==nullptr){
                    //segfault
                    cout<<" SEGV"<<endl;
                    process->pageStats->segvCnt++;
                    break;
                }
                if(!pte->present){

                    FrameTableEntry *oldFTE = getFrame(pager);

                    if(oldFTE->process != nullptr){
                        unmapPage(oldFTE, false);
                    }
                    if (pte->fileMapped) {
                        cout<< " FIN"<<endl;
                        process->pageStats->pagefinCnt++;
                    } else if(pte->pagedout){
                        cout<< " IN"<<endl;
                        process->pageStats->pageinCnt++;
                    } else {
                        cout << " ZERO" << endl;
                        process->pageStats->zeroOpCnt++;
                    }

                    oldFTE->process = process;
                    oldFTE->virtualPageNumber = instruction.second;

                    pte->reset();

                    pte->present = 1;
                    pte->phyAddr = oldFTE->phyFrameNumber;
                    process->pageTable.at(instruction.second);
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
                if(xOption)
                    printPageTableForProcess(process);
                if(yOption)
                    printPageTable(&processes);
            break;
        }
        instCount++;
    }
    if(pageTableOption)
        printPageTable(&processes);

    if(frameTableOption)
        printFrameTable(&frameTable);

    printProcessStats(&processes, instCount, processOption, sumOption);
    return 0;
}

FrameTableEntry* getFrame(Pager *pager){
    if(freePool.size()>0){
        FrameTableEntry *fte = freePool.front();
        freePool.erase(freePool.begin());
        return fte;
    } else {
        return pager->selectVictimFrame();
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
                        pager = new FIFO(&frameTable);
                        break;
                    case 'c':
                        pager = new Clock(&frameTable);
                        break;
                    case 'r':
                        pager = new Random();
                        break;
                    case 'e':
                        pager = new NotRecentlyUsed();
                        break;
                    case 'a':
                        pager = new NotRecentlyUsed();
                        break;
                    case 'w':
                        pager = new NotRecentlyUsed();
                        break;
                    default:
                        fputs("Error: invalid scheduler "
                              "specified\n\n", stderr);
                        exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                for(int i = 0; i < 6; i++) {
                    switch (optarg[i]) {
                        case 'O':
                            processOption = true;
                            break;
                        case 'F':
                            frameTableOption = true;
                            break;
                        case 'P':
                            pageTableOption = true;
                            break;
                        case 'S':
                            sumOption = true;
                            break;
                        case 'x':
                            xOption = true;
                            break;
                        case 'y':
                            yOption = true;
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
        pager = new FIFO(&frameTable);
}
