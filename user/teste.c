#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
// Verifica se o usuário passou argumentos
  if(argc < 2){
    fprintf(2, "Uso: meucomando <texto>\n");
    exit(1);
  }

  // Imprime os argumentos passados no terminal
  for(int i = 1; i < argc; i++){
    printf("Argumento %d: %s\n", i, argv[i]);
  }

  // Sempre encerre com exit(0), nunca com return!
  exit(0);
}