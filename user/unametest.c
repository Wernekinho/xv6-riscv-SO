#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/utsname.h"
#include "user/user.h"

// Auxiliar para executar um comando externo no xv6 e aguardar seu termino
int run_cmd(char *argv[]) {
  int pid = fork();
  if(pid < 0)
    return -1;
  if(pid == 0){
    // Substitui a imagem do processo filho pelo programa solicitado
    exec(argv[0], argv);
    exit(1);
  }
  int status = 0;
  wait(&status);
  return status;
}

int main(int argc, char *argv[]) {
  struct utsname u;
  memset(&u, 0, sizeof(u));

  // Testa chamada valida a uname(): deve retornar 0 e preencher os campos
  int r = uname(&u);
  if(r != 0)
    printf("FALHA: uname(&u) retornou %d (esperado 0)\n", r);
  if(strlen(u.sysname) == 0)
    printf("FALHA: sysname vazio\n");
  if(strlen(u.nodename) == 0)
    printf("FALHA: nodename vazio\n");
  if(strlen(u.release) == 0)
    printf("FALHA: release vazio\n");
  if(strlen(u.version) == 0)
    printf("FALHA: version vazio\n");
  if(strlen(u.machine) == 0)
    printf("FALHA: machine vazio\n");

  // Testa ponteiros invalidos para garantir que copyout rejeita com -1 sem panic
  if(uname((struct utsname *)0) != -1)
    printf("FALHA: uname(NULL) deveria retornar -1\n");
  if(uname((struct utsname *)-1) != -1)
    printf("FALHA: uname((void*)-1) deveria retornar -1\n");

  // Testa endereco alem do topo atual da memoria do processo (p->sz via sbrk(0))
  char *top = sbrk(0);
  if(uname((struct utsname *)(top + 4096)) != -1)
    printf("FALHA: uname acima do limite de memoria deveria retornar -1\n");

  // Testa invocacao do binario uname com flags validas e flag invalida
  char *cmd_default[] = { "uname", 0 };
  char *cmd_all[]     = { "uname", "-a", 0 };
  char *cmd_comb[]    = { "uname", "-sn", 0 };
  char *cmd_invalid[] = { "uname", "-x", 0 };

  if(run_cmd(cmd_default) != 0)
    printf("FALHA: exec 'uname' retornou erro\n");
  if(run_cmd(cmd_all) != 0)
    printf("FALHA: exec 'uname -a' retornou erro\n");
  if(run_cmd(cmd_comb) != 0)
    printf("FALHA: exec 'uname -sn' retornou erro\n");
  if(run_cmd(cmd_invalid) == 0)
    printf("FALHA: exec 'uname -x' deveria falhar\n");

  // No xv6, processos de usuario devem encerrar com exit()
  exit(0);
}