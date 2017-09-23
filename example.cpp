#include "readElf.h"

int main(int argc, char **argv){
    ReadElf e("hello");
    e.printSection();
    e.printGotPlt();
}
