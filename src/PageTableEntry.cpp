//
// Created by gupta on 20-04-2020.
//

#include "PageTableEntry.h"

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

PageTableEntry::PageTableEntry() {
    this->present = 0;
    this->writeProtect = 0;
    this->modified = 0;
    this->referenced = 0;
    this->pagedout = 0;
    this->phyAddr = 100;
    this->fileMapped = 0;
    this->vmaPresent = 0;
};