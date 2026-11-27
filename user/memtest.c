
#include "kernel/types.h"
#include "user/user.h"

#define PGSIZE 4096
#define ALLOC_SIZE (4 * PGSIZE)

int
main(void)
{
    uint64 total;
    uint64 free_before;
    uint64 free_after_alloc;
    uint64 free_after_free;

    // 1. Consultar memoria antes da alocacao
    if (meminfo(&total, &free_before) < 0) {
        printf("Erro ao consultar memoria\n");
        exit(1);
    }

    printf("Memoria livre inicial: %lu bytes\n", free_before);

    // 2. Alocar quatro paginas
    if (sbrk(ALLOC_SIZE) == (char *)-1) {
        printf("Erro ao alocar memoria\n");
        exit(1);
    }

    if (meminfo(&total, &free_after_alloc) < 0) {
        printf("Erro ao consultar memoria\n");
        exit(1);
    }

    printf("Apos alocacao: %lu bytes\n", free_after_alloc);

    // 3. Liberar as quatro paginas
    if (sbrk(-ALLOC_SIZE) == (char *)-1) {
        printf("Erro ao liberar memoria\n");
        exit(1);
    }

    if (meminfo(&total, &free_after_free) < 0) {
        printf("Erro ao consultar memoria\n");
        exit(1);
    }

    printf("Apos liberacao: %lu bytes\n", free_after_free);

    exit(0);
}
