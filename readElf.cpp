/*
@author binpang
@time 2017/08/30
@description 定义读取文件的部分Section，并将.got.plt部分进行修改
*/

#include "readElf.h"

bool ReadElf::readFile(const char *filename){
    struct stat sb;
    fd = open(filename, O_RDWR);
    fstat(fd, &sb); //获取文件信息
    file_size = sb.st_size;
    mem_start = (char*)mmap(NULL, sb.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	
    close(fd);
    if(mem_start == MAP_FAILED){ //判断文件是否映射成功
        return false;
    }
    return true;

}

bool ReadElf::getGotPlt(){
    char* tempName;
    for(int i = 1; i < sectionNum; i++){ //Section从1开始索引，0为保留的Section
        tempName = strtab + elf_section[i].sh_name;
	printf("tempName: %s\n", tempName);
        if(!strcmp(tempName, ".got.plt")){
            gotplt = mem_start + elf_section[i].sh_offset;
	    //gotpltOffset = elf_section[i].sh_offset;
	    printf("GotPlt size is %d, entry size is %d\n", elf_section[i].sh_size, elf_section[i].sh_entsize);
            //gotpltNum = elf_section[i].sh_size / elf_section[i].sh_entsize;
	    gotpltNum = elf_section[i].sh_size / 4;
            return true;
        }
    }
    return false;
}
ReadElf::ReadElf(const char *filename){
    if(!readFile(filename)){
        printf("mmap %s failed!!!\n", filename);
        exit(0);
    }
    memcpy(&elf_header, this->mem_start, sizeof(Elf64_Ehdr)); //获得ELF Header
    sectionNum = elf_header.e_shnum;
    //获得所有的Section
    try{
        elf_section = (Elf64_Shdr*)(mem_start + elf_header.e_shoff);

    } catch(exception &e){

        cout << e.what() << "\n";
    }

    /*
    获得String Table Section的起始地址
    */
    int str_index = elf_header.e_shstrndx;
    strtab = mem_start + elf_section[str_index].sh_offset;
    if(!getGotPlt()){
	printf("Don't find Gotplt\n");
    }
}

void ReadElf::printSection(){
    printf("Num %d\n", elf_header.e_shnum);
    for(int i = 1; i < sectionNum; i++){
        printf("Section Name %s\n", strtab + elf_section[i].sh_name);
    }
}

/*
    将.got.plt表项的内容打印出来
*/
void ReadElf::printGotPlt(){
    unsigned int gotEntry;
    unsigned int gotEntry1;
    //char tempChar[5];
    printf("GotPltNum : %d\n", gotpltNum);
    for(int i = 0; i < gotpltNum; i++){
	    memcpy(&gotEntry, this->gotplt +i*4, sizeof(unsigned int));
	    //tempChar[4] = '\0';
	    printf("copy done!\n");
	    //printf("%s\n", tempChar);
	    //gotEntry=atoi(tempChar);
	    printf("%08x ", gotEntry);
	    gotEntry1 = gotEntry;
 	    memcpy(this->gotplt +i*4, &gotEntry1, sizeof(unsigned int));
	    memcpy(&gotEntry1, this->gotplt + i*4, sizeof(unsigned int));
	    printf("\n");
	    printf("%08x ", gotEntry1);
        //gotEntry = (unsigned int)(gotplt + i * 4);
        //printf("%02x", *(gotplt+i*4));
	    //printf("%02x", *(gotplt+i*4+1));
	    //printf("%02x", *(gotplt+i*4+2));
	    //printf("%02x ", *(gotplt+i*4+3));
    }

    if ((msync((void *)mem_start, file_size, MS_SYNC)) == -1) {  
        printf("Modify Error!");
    }  


}
ReadElf::~ReadElf(){  
    munmap(mem_start, file_size); /* 解除映射 */
}
