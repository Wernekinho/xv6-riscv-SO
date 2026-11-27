#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;

  printf("1. Antes do fork:\n");
  ps();

  pid = fork();
  if(pid < 0){
    fprintf(2, "pstest: fork falhou\n");
    exit(1);
  }

  if(pid == 0){
    // Processo filho
    pause(20);
    printf("Filho (pid %d) encerrando...\n", getpid());
    exit(0);
  }

  // Processo pai
  printf("2. Logo apos fork (filho pid %d criado):\n", pid);
  ps();

  // Espera o filho terminar antes de chamar wait(),
  // permitindo observar o estado de encerramento/zombie
  pause(35);
  printf("3. Apos saida do filho e antes do wait:\n");
  ps();

  wait(0);
  printf("4. Apos wait (filho recolhido):\n");
  ps();

  exit(0);
}
