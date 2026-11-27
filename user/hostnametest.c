#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/utsname.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  struct utsname orig, u;
  char exact64[UTSLEN];
  char over65[UTSLEN + 1];

  // Captura o estado inicial para restaurar o hostname ao final dos testes
  if(uname(&orig) < 0){
    printf("FALHA: nao foi possivel ler hostname inicial\n");
    exit(1);
  }

  // Testa alteracao valida e verifica se uname() enxerga o estado atualizado
  if(sethostname("ufrrj-xv6") != 0)
    printf("FALHA: sethostname('ufrrj-xv6') deveria retornar 0\n");
  if(uname(&u) != 0 || strcmp(u.nodename, "ufrrj-xv6") != 0)
    printf("FALHA: uname() nao refletiu 'ufrrj-xv6'\n");

  // Rejeita string vazia (sem caracteres uteis)
  if(sethostname("") != -1)
    printf("FALHA: sethostname('') deveria retornar -1\n");

  // Limite maximo: 64 caracteres uteis + '\0' cabe em UTSLEN (65 bytes)
  memset(exact64, 'a', UTSLEN - 1);
  exact64[UTSLEN - 1] = '\0';
  if(sethostname(exact64) != 0)
    printf("FALHA: sethostname com 64 chars deveria ser aceito\n");
  if(uname(&u) != 0 || strcmp(u.nodename, exact64) != 0)
    printf("FALHA: uname() alterou ou truncou nome de 64 chars\n");

  // Acima do limite: 65 caracteres uteis excede UTSLEN - 1 e deve falhar
  memset(over65, 'b', UTSLEN);
  over65[UTSLEN] = '\0';
  if(sethostname(over65) != -1)
    printf("FALHA: sethostname com 65 chars deveria retornar -1\n");

  // Testa ponteiros invalidos para garantir que copyinstr rejeita sem panic
  if(sethostname((char *)0) != -1)
    printf("FALHA: sethostname(NULL) deveria retornar -1\n");
  if(sethostname((char *)-1) != -1)
    printf("FALHA: sethostname((void*)-1) deveria retornar -1\n");

  char *top = sbrk(0);
  if(sethostname(top + 4096) != -1)
    printf("FALHA: sethostname acima do limite de memoria deveria retornar -1\n");

  // Teste de concorrencia: multiplos processos alternando escrita e leitura
  // Verifica ausencia de panic e consistencia do tamanho da string lida
  char *nomes[4] = { "worker-0", "worker-1-longo", "w2", "node-3-so" };
  int nprocs = 4;
  int iters = 30;

  for(int i = 0; i < nprocs; i++){
    int pid = fork();
    if(pid == 0){
      struct utsname local;
      for(int k = 0; k < iters; k++){
        if(sethostname(nomes[i]) < 0)
          exit(1);
        if(uname(&local) < 0)
          exit(1);
        int len = strlen(local.nodename);
        if(len <= 0 || len >= UTSLEN)
          exit(1);
      }
      exit(0);
    }
  }

  for(int i = 0; i < nprocs; i++){
    int st = 0;
    wait(&st);
    if(st != 0)
      printf("FALHA: processo filho %d falhou durante teste de concorrencia\n", i);
  }

  // Restaura o nome original do sistema antes de encerrar
  if(sethostname(orig.nodename) != 0)
    printf("FALHA: nao foi possivel restaurar o hostname original\n");

  exit(0);
}