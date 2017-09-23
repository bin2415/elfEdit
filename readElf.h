/*
@author binpang
@time 2017/08/30
@description 定义读取文件的方法及一些结构
*/

#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>
#include <elf.h>
#include <memory.h>

using namespace std;

class ReadElf{

public:
    bool readFile(const char *filename);
    void printSection();
    void printGotPlt();
    bool XorGotPlt(unsigned int key); //to be implemented
    ReadElf(const char *filename);
    ~ReadElf();

private:
    Elf64_Ehdr elf_header; //ELF File Header,在elf.h文件中有定义
    Elf64_Shdr  *elf_section; //ELF File Section,在elf.h文件中有定义
    char *mem_start;
    //char *temp_entry;
    char *strtab; //.str table section
    char *gotplt; //.got.plt table Section
    int fd; //打开文件描述符
    unsigned int file_size; //文件的大小
    int sectionNum; //Senction的数量
    int gotpltNum;
    bool getGotPlt();
};
