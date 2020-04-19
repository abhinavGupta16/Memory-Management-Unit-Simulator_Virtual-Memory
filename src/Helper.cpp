//
// Created by gupta on 18-04-2020.
//

#include "Helper.h"

#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include "Process.h"
#include "VirtualMemoryAddress.h"
#include "Instruction.h"

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

VirtualMemoryAddress* readVmas(ifstream &input) {
    string line;
    vector<string> tokens;
    while(getline(input, line) && line.at(0)== '#');
    convertStringToTokens(line, &tokens);
    return new VirtualMemoryAddress(stoi(tokens[0]), stoi(tokens[1]), stoi(tokens[2]), stoi(tokens[3]));
}

void readInputFile(string filename, vector<Process*> &processes, vector<Instruction*> &instructions){
    ifstream input(filename);
    string line;
    vector<string> tokens;

    int numOfProcess = 0;
    while(getline(input, line)){
        if(line.at(0) == '#')
            continue;
        else{
            numOfProcess = stoi(line);
            break;
        }
    }

    for(int i = 0; i<numOfProcess;){
        getline(input, line);
        if(line.at(0) == '#')
            continue;
        int numberOfVma = stoi(line);
        Process *process = new Process();
        while(numberOfVma > 0) {
            process->vmas.push_back(readVmas(input));
            numberOfVma--;
        }
        processes.push_back(process);
        i++;
    }

    while(getline(input, line)){
        if(line.at(0)=='#'){
            continue;
        }
        convertStringToTokens(line, &tokens);
        Instruction *instruction = new Instruction(tokens[0][0], stoi(tokens[1]));
        instructions.push_back(instruction);
        tokens.clear();
    }
}

