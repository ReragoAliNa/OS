#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
// #include "user/ulib.c"

int main(int argc, char *argv[]) {
    // 在编译时会依据ABI接口协议为输入分配寄存器，将输入的参数的长度和内容分别存储，在xv6的riscv64架构中会分别分配给a0和a1
    if (argc != 2) {
        fprintf(2,"The command is invalid!\n");
        exit(1);
    }else {
        int num = atoi(argv[1]);
        sleep(num);
        exit(0);
    }
}