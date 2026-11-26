
#include "kernel/types.h"
#include "user/user.h"

#define PGSIZE 4096
#define NCHILDREN 4
#define ITERATIONS 200
#define ALLOC_PAGES 4
#define ALLOC_SIZE (ALLOC_PAGES * PGSIZE)

int
main(void)
{
    uint64 total_before, free_before;
    uint64 total, free;
    uint64 free_after;
    int i, j;
    int failed = 0;

    if (meminfo(&total_before, &free_before) < 0) {
        printf("Erro ao consultar memoria inicial\n");
        exit(1);
    }

    printf("Memoria livre inicial: %lu bytes\n", free_before);
    printf("Iniciando %d processos concorrentes...\n", NCHILDREN);

    for (i = 0; i < NCHILDREN; i++) {
        int pid = fork();

        if (pid < 0) {
            printf("Erro ao criar processo filho\n");
            failed = 1;
            break;
        }

        if (pid == 0) {
            for (j = 0; j < ITERATIONS; j++) {
                char *p = sbrk(ALLOC_SIZE);

                if (p == (char *)-1) {
                    printf("Filho %d: erro na alocacao\n", i);
                    exit(1);
                }

                // Toca em cada pagina para forcar sua alocacao fisica.
                for (int k = 0; k < ALLOC_PAGES; k++)
                    p[k * PGSIZE] = (char)k;

                if (meminfo(&total, &free) < 0 || free > total) {
                    printf("Filho %d: erro no meminfo\n", i);
                    exit(1);
                }

                if (sbrk(-ALLOC_SIZE) == (char *)-1) {
                    printf("Filho %d: erro na liberacao\n", i);
                    exit(1);
                }
            }

            exit(0);
        }
    }

    // O processo principal tambem consulta enquanto os filhos trabalham.
    for (i = 0; i < 2000; i++) {
        if (meminfo(&total, &free) < 0 || free > total) {
            printf("Processo principal: erro no meminfo\n");
            failed = 1;
            break;
        }
    }

    // Aguarda todos os filhos e verifica seus resultados.
    for (i = 0; i < NCHILDREN; i++) {
        int status;
        if (wait(&status) < 0) {
            printf("Erro ao aguardar processo filho\n");
            failed = 1;
            break;
        }
        if (status != 0)
            failed = 1;
    }

    if (meminfo(&total, &free_after) < 0) {
        printf("Erro ao consultar memoria final\n");
        exit(1);
    }

    printf("Memoria livre final: %lu bytes\n", free_after);

    if (failed) {
        printf("FALHOU: houve erro durante o teste\n");
        exit(1);
    }

    if (free_after != free_before) {
        printf("ATENCAO: memoria livre final diferente da inicial\n");
        printf("Diferenca: %ld bytes\n", (long)(free_before - free_after));
        exit(1);
    }

    printf("PASSOU: processos concorrentes finalizaram corretamente\n");
    printf("PASSOU: memoria livre retornou ao valor inicial\n");

    exit(0);
}
