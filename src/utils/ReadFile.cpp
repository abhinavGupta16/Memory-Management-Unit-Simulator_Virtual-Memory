//
// Created by gupta on 20-04-2020.
//

#include "ReadFile.h"

ReadFile::ReadFile(string filename) : input(filename) {
    this->filename = filename;
}

string Trim(string& src)
{
    // Removes all spaces from the beginning of the string
    while(src.size() && isspace(src.front()))
        src.erase(src.begin());

    // Remove all spaces from the end of the string.
    while(src.size() && isspace(src.back()))
        src.pop_back();

    return src;
}

string ReadFile::readLine() {
    string line = "";
    while(input.is_open() && getline(input, line) && Trim(line).compare("") && (line.at(0) == '#' || line.at(0) == '\n'));
    return line;
}

bool ReadFile::eof(){
    return input.eof();
}

