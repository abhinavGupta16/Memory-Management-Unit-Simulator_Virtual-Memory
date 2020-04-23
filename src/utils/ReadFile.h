//
// Created by gupta on 20-04-2020.
//

#ifndef ASSIGNMENT3_READFILE_H
#define ASSIGNMENT3_READFILE_H

#include <iostream>
#include <bits/stdc++.h>
#include <string>

using namespace std;

class ReadFile {
private:
    string filename;
    ifstream input;
public:
    string readLine();
    ReadFile(string filename);
    bool eof();
    void reset();
};


#endif //ASSIGNMENT3_READFILE_H
