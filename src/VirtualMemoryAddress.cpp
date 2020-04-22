//
// Created by gupta on 19-04-2020.
//

#include "VirtualMemoryAddress.h"

#include "utils/Helper.h"

#include <iostream>
#include <bits/stdc++.h>
#include <string>

using namespace std;

VirtualMemoryAddress::VirtualMemoryAddress(int start_vpage, int end_vpage, int write_protected, int file_mapped){
    this->start_vpage = start_vpage;
    this->end_vpage = end_vpage;
    this->write_protected = write_protected;
    this->file_mapped = file_mapped;
}

void VirtualMemoryAddress::print(){
    cout<<this->start_vpage << " " << this->end_vpage << " " << this->write_protected << this->file_mapped << endl;
}