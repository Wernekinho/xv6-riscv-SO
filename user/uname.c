#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/utsname.h"
#include "user/user.h"

// Flags de exibição
#define F_SYSNAME (1 << 0)
#define F_NODENAME (1 << 1)
#define F_RELEASE (1 << 2)
#define F_VERSION (1 << 3)
#define F_MACHINE (1 << 4)
#define F_ALL (F_SYSNAME | F_NODENAME | F_RELEASE | F_VERSION | F_MACHINE)


void print(char *str, int *primeiro) {
    if(!(*primeiro))
        printf(" ");
    printf("%s", str);
    *primeiro = 0;
}

int main(int argc, char *argv[]) {
    struct utsname u;
    int flags = 0;

    for(int i = 1; i < argc; i++) {
        if(argv[i][0] != '-' || argv[i][1] == '\0') {
            fprintf(2, "uname: argumento invalido '%s'\n", argv[i]);
            exit(1);
        }
        for(int j = 1; argv[i][j]; j++) {
            switch(argv[i][j]) {
                case 'a': flags |= F_ALL; break;
                case 's': flags |= F_SYSNAME; break;
                case 'n': flags |= F_NODENAME; break;
                case 'r': flags |= F_RELEASE; break;
                case 'v': flags |= F_VERSION; break;
                case 'm': flags |= F_MACHINE; break;
                default:
                  fprintf(2, "uname: opcao invalida -- '%c'\n", argv[i][j]);
                  fprintf(2, "Uso: uname [-asnrvma]\n");
                  exit(1);
            }
        }
    }

    if(flags == 0) flags = F_SYSNAME;

    if(uname(&u) < 0){
        fprintf(2, "uname: falha na chamada ao sistema\n");
        exit(1);
    }

    int cont = 1;
    if(flags & F_SYSNAME)  print(u.sysname, &cont);
    if(flags & F_NODENAME) print(u.nodename, &cont);
    if(flags & F_RELEASE)  print(u.release, &cont);
    if(flags & F_VERSION)  print(u.version, &cont);
    if(flags & F_MACHINE)  print(u.machine, &cont);
    printf("\n");

    exit(0);
}