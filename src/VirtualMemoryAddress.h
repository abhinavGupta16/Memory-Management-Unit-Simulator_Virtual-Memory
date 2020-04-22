//
// Created by gupta on 19-04-2020.
//

#ifndef ASSIGNMENT3_VMA_H
#define ASSIGNMENT3_VMA_H


class VirtualMemoryAddress {
public:
    int start_vpage;
    int end_vpage;
    int write_protected;
    int file_mapped;
    VirtualMemoryAddress(int start_vpage, int end_vpage, int write_protected, int file_mapped);
    void print();
};


#endif //ASSIGNMENT3_VMA_H
