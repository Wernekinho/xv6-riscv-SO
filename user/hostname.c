#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/utsname.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        struct utsname u;
        if(uname(&u) < 0) {
            fprintf(2, "hostname: erro ao ler uname\n");
            exit(1);
        }
        printf("%s\n", u.nodename);
        exit(0);
    }

    if (argc == 2) {
        if(sethostname(argv[1]) < 0) {
            fprintf(2, "hostname: nome invalido ou muito longo (maximo de %d chars)\n", UTSLEN - 1);
            exit(1);
        }
        exit(0);
    }

    fprintf(2, "Uso: hostname [novo_nome]\n");
    exit(1);
}