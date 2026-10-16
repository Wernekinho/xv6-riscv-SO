#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/utsname.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    struct utsname u;
    
    if(uname(&u) < 0) {
        fprintf(2, "falha na system call\n");
        exit(1);
    }

    printf("Kernel : %s\n", u.sysname);
    printf("Version : %s\n", u.version);
    printf("Release : %s\n", u.release);
    printf("Nodename : %s\n", u.nodename);
    printf("Machine : %s\n", u.machine);
    printf("\n");

    exit(0);
}