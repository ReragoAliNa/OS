// 按从小到大的顺序排列，自然数中（不考虑0和1），不是素数的数必然可以分解为小于这个数的素数之积，且分解方式是唯一的。
// 素数乘积必然得到一个大于它们的不是素数的数，且这个数唯一。
// 任何不是素数的数都可以被某个小于它们的素数整除。
// 不可以被小于它们的素数整除的数，是一个新的素数。
// - up to bottom：若要判断一个数是不是素数，只需要看比这个数小的素数是不是可以整除它。
// - bottom to up：从最小的素数2开始，向上构建数：
/* 
e.g.比3小的素数只有2，3不能被2整除，所以3是素数；比4小的素数有2和3，4 = 2 * 2； 比5小的素数有2和3，5不可以被2或者3整除，所以5是素数；
... 比11小的素数有2，3，5，7，且11不可以被2，3，5，7整除，所以11是素数 ...
*/

/*
设定一个判断是否可以整除的操作 和 后继函数（S：x->x+1） 且 这个操作从第二小的素数3开始：
Pseudocode:
Operation:
int n;
int p;
if (n % p) {
    print("n is a prime number.");
}

Generator:
start from 2;
get S(2);
get S(S(2));
...
get S^n(2);

Generator (2~35):
start from 2;
get S(2);
...
get S^{33}(2);

We can't get the numbers one by one from Generator by a c-function,
but we can just get one number in this way.
Consider the premiss, it runs on a progress.
If we take more progress to generate number and justify whether it is a prime,
then we will make it.
Assume there are 2 progresses to run the following code, one is Alice, the other one is Peter,
when they meet if-else simultaneously, and what if says that if(name == Alice), Alice and Peter will check their name,
Alice can go into it, but Peter can not. Peter will run next, if no else. Two progresses are going to be different now.
Or rather, if-else is not changed its meaning, and there are just 2 of the same sentence in different progress(even Alice and Peter are very similar, and there are no differences between them,
except for their Names. They are in fact different because of their names. If they also own the same name, they are still not one thing, but we and computer can not identity them, and we call this
case the same.), but the same if-else sentences differ them nicely by their different name. i.e. the same if-else sentences differ the similar progresses.



Progress 1:
Generate 2;
it is a prime;

Progress 2:
Generate 3;
it can not be divided by 2 in the natural numbers;
3 is a prime;
...

Progress 34:
Generate 35;
the previous primes we justified are the all primes less than 35.
justify if it can be divided by the previous primes we justified.

Progress 1(the parent progress):
create a pipe;
fork the progress 1 to produce the progress 2;
push 2 to Progress 2.

Progress 2:
get the number from Progress 1;
print("prime: 2");
create S(2) and push it to Progress 3.

Progress 3:
if (3 % 2 != 0);
print("prime: 3");

...

Progress 35;



*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void recursive(int left_terminal_fd);

void main(int argc, char *argv[]) {
    int f2s[2];

    if(pipe(f2s) < 0) {
        fprintf(2, "pip creation failed!");
        exit(1);
    }

    int pid = fork();

    if(pid < 0) {
        fprintf(2, "fork makes error!");
        exit(1);
    }else if(pid == 0){
        close(f2s[1]);
        recursive(f2s[0]); //归纳奠基
        close(f2s[0]);
    }else{
        close(f2s[0]);
        for(int i = 2; i <= 35; i++) {
            write(f2s[1], &i, sizeof(int));
        }
        close(f2s[1]);
        wait(0);
        exit(0);
    }
}

//归纳假设（假设成立通过数学证明）：假设第k-1个进程为末尾子进程（k大于2）；注意作为第k-1个进程要有归纳的所有属性，不仅要有作为末尾的属性还要考虑递推造成的影响，
//i.e. 作为中间层和末尾的属性的不同；在数学中，因为“位置”的改变不会造成“数”的性质的改变（可以证明），所以不需要考虑递推造成的影响。
/* 
作为（“末尾父”）进程的属性：1.生成管道和子进程的属性
作为末尾子进程的属性：1.接受数的属性 2.判断有没有数要接收的属性 3.必定得到素数
作为中间层子进程的属性：1.接收数的属性 2.判断有没有数要接收的属性
作为中间层父进程的属性：1.判断和传递数的属性

记得判断是否fork成功
*/
void recursive (int left_terminal_fd) {

    /* 第k-1个进程作为第k-2个进程的子进程；子进程要可以接收父进程的数（无论父进程传没传数）且要能判断父进程有没有传数 */
    int p;

    if(read(left_terminal_fd, &p, sizeof(int)) == 0) {
        exit(0);
    }

    // 末尾子进程必定得到素数
    printf("prime %d\n", p);

    /* 第k-1个进程作为第k个进程的父进程；父进程要可以创建管道和自己的子进程 且 可以判断创没创建成功 */
    int fd[2];
    int n;

    if(pipe(fd) < 0){
        fprintf(2, "pipe creation failed!\n");
        exit(1);
    }

    int pid = fork();

    if(pid < 0) {
        fprintf(2, "fork failed!\n");
        exit(1);
    }else if(pid > 0){
        close(fd[0]);
        /* 
        作为第k-2个进程的子进程；子进程要检测是否父进程是否通过管道传递数字到子进程中；
        作为第k个进程的父进程；父进程要检测是否可以传递数字到子进程，若可以传递则实现传递，若不可则不用管这个数了（仍在n中）
        */
        
        while(read(left_terminal_fd, &n, sizeof(int)) != 0){
            if(n % p != 0){
                write(fd[1], &n, sizeof(int));
            }
        }
        close(fd[1]);
        close(left_terminal_fd);
        wait(0);
        exit(0);
        

    }else {
        close(fd[1]);
        // 不需要从grandpa进程中获得数
        close(left_terminal_fd);
        // 每个进程都要保留一个数作为除数用于判断得到的数是否可以被整除，
        /* 
        i.e.假设数字a通过进程A时（此时A应被视为子进程），直接被存储（并打印）。
        数字m（大于a）通过进程A时，进程A作为父进程判断其作为子进程时得到的数m是否可以被a整除，
        若可以整除，则无操作，一直存储在A中；若不可整除，则传递传递给下一个子进程B重复这个过程。
        */
        /* 
        第k-1个进程代表了所有的进程共有属性，所以其fork生成的子进程应拥有其一样的所有属性：
        1. 作为父进程的属性；
        2. 作为子进程的属性；
        其全部包含于recursive函数中，所有下面用recursive(fd[0])实现。
        开头先写的是作为子进程的属性是因为，在实现层面，归纳假设要从第二进程（也就是第一子进程）开始实现，以保证第一进程用于产生数（与其他进程不同），
        进程2要先满足作为子进程的属性，再作为父进程产生子进程，然后作为中间子进程接收数，再作为中间父进程判断和传递数。
        若盲目先执行pipe 和 fork 再读取数据，不仅会严重阻塞上游进程的 I/O 节奏，还会导致子进程在条件未明的情况下进行无效繁衍。
        */
        recursive(fd[0]);
        close(fd[0]);
        exit(0);
        
    }


}
