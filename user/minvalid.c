
#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
    uint64 total;
    uint64 free;

    printf("Teste 1: enderecos validos\n");

    if (meminfo(&total, &free) == 0)
        printf("PASSOU: syscall retornou 0\n");
    else
        printf("FALHOU: syscall retornou erro\n");

    printf("\nTeste 2: primeiro endereco invalido\n");

    if (meminfo((uint64 *)-1, &free) == -1)
        printf("PASSOU: syscall rejeitou o endereco\n");
    else
        printf("FALHOU: syscall aceitou o endereco\n");

    printf("\nTeste 3: segundo endereco invalido\n");

    if (meminfo(&total, (uint64 *)-1) == -1)
        printf("PASSOU: syscall rejeitou o endereco\n");
    else
        printf("FALHOU: syscall aceitou o endereco\n");

    printf("\nTestes finalizados.\n");

    exit(0);
}
