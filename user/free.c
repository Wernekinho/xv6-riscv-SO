#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
    uint64 total;
    uint64 free;

    if (meminfo(&total, &free) < 0) {
        printf("free: erro ao obter informacoes de memoria\n");
        exit(1);
    }

    printf("Memoria total: %lu bytes\n", total);
    printf("Memoria livre: %lu bytes\n", free);

    exit(0);
}