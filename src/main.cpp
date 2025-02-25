#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <getopt.h>

#include "utils/Helper.h"
#include "utils/ReadFile.h"
#include "pagers/Pager.h"
#include "pagers/FIFO.h"
#include "pagers/Clock.h"
#include "pagers/Random.h"
#include "pagers/NotRecentlyUsed.h"
#include "pagers/Aging.h"
#include "pagers/WorkingSet.h"

using namespace std;

vector<FrameTableEntry*> frameTable;
vector<FrameTableEntry*> freePool;
int frameTableSize = 16;
Pager* pager;
bool instOption = false;
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
    } else if(WorkingSet* t = dynamic_cast<WorkingSet*>(pager)){
        pager = new WorkingSet(&frameTable, &instCount);
    }

    while(getInstruction(inputFile, instruction)) {
        if(instOption) printInstruction(instCount, instruction);
        PageTableEntry *pte;
        switch(instruction.first) {
            case 'c' :
                process = processes.at(instruction.second);
                process->pageStats->contextCnt++;
                break;
            case 'e' :
                process->pageStats->processExitCnt++;
                exitProcess(process, &frameTable, &freePool, instOption);
                break;
            default:
                process->pageStats->accessCnt++;
                pte = getPageTableEntry(instruction, process);
                if(pte==nullptr){
                    //segfault
                    if(instOption) cout<<" SEGV"<<endl;
                    process->pageStats->segvCnt++;
                    break;
                }
                if(!pte->present){

                    FrameTableEntry *oldFTE = getFrame(pager);

                    if(oldFTE->process != nullptr){
                        unmapPage(oldFTE, false, instOption);
                    }
                    if (pte->fileMapped) {
                        if(instOption) cout<< " FIN"<<endl;
                        process->pageStats->pagefinCnt++;
                    } else if(pte->pagedout){
                        if(instOption) cout<< " IN"<<endl;
                        process->pageStats->pageinCnt++;
                    } else {
                        if(instOption) cout << " ZERO" << endl;
                        process->pageStats->zeroOpCnt++;
                    }

                    oldFTE->process = process;
                    oldFTE->virtualPageNumber = instruction.second;
                    oldFTE->resetAgingAlgoAge();
                    oldFTE->timeOfLastUse = instCount+1;

                    pte->reset();

                    pte->present = 1;
                    pte->phyAddr = oldFTE->phyFrameNumber;
                    process->pageTable.at(instruction.second);
                    if(instOption) cout<<" MAP " << oldFTE->phyFrameNumber<<endl;
                    process->pageStats->mapCnt++;
                }
                pte->referenced = 1;

                if (instruction.first == 'w'){
                    if(pte->writeProtect){
                        if(instOption) cout<<" SEGPROT"<< endl;
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

    if(sumOption)
        printProcessStats(&processes, instCount);

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
                        pager = new Aging(&frameTable);
                        break;
                    case 'w':
                        pager = new WorkingSet();
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
                            instOption = true;
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
