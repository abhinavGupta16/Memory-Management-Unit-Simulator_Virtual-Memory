#include <iostream>
#include <bits/stdc++.h>
#include <string>

#include "Helper.h"
#include "Process.h"
#include "Instruction.h"

using namespace std;

int main(int argc, char *argv[]) {
    string inputFilename = argv[1];

    vector<Process*> processes;
    vector<Instruction*> instructions;

    readInputFile(inputFilename, processes, instructions);

    cout << "Hello, World!" << endl;
    return 0;
}
