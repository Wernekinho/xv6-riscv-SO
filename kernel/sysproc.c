#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "vm.h"
#include "utsname.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  kexit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return kfork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return kwait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int t;
  int n;

  argint(0, &n);
  argint(1, &t);
  addr = myproc()->sz;

  if (t == SBRK_EAGER || n < 0) {
    if (growproc(n) < 0) {
      return -1;
    }
  } else {
    // Lazily allocate memory for this process: increase its memory
    // size but don't allocate memory. If the processes uses the
    // memory, vmfault() will allocate it.
    if (addr + n < addr)
      return -1;
    if (addr + n > TRAPFRAME)
      return -1;
    myproc()->sz += n;
  }
  return addr;
}

uint64
sys_pause(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if (n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (killed(myproc())) {
      release(&tickslock);
      return -1;
    }
    sleep_prepare(&ticks);
    release(&tickslock);
    sleep();
    acquire(&tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kkill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64 sys_uname(void) {
  uint64 endereco; // endereço de memoria de 64 bits, esse é o endereço de memoria do espaço de usuário
  struct utsname uname; // cria um struct temporario na pilha do kernel do processo atual
  struct proc *p = myproc(); // ponteiro para a estrutura de controle do processo, precisamos disso para acessar a tabela de páginas do processo e o tamanho da memoria dele

  argaddr(0, &endereco); // No risc-v o primeiro argumento é guardado no registrador a0 da CPU esse argaddr ele puxa o ponteiro de memoria desse registrador

  // limpa o lixo de memoria alocado no espaço do struct
  memset(&uname, 0, sizeof(uname));

  // define os valores do sistema de forma segura (garante que o ultimo caracter seja \0 para evitar buffer overflow)
  safestrcpy(uname.sysname, "xv6-riscv", UTSLEN);
  safestrcpy(uname.nodename, "xv6-node", UTSLEN);
  safestrcpy(uname.release, "1.0-mit", UTSLEN);
  safestrcpy(uname.version, "2026-SO-UFRRJ", UTSLEN);
  safestrcpy(uname.machine, "riscv64", UTSLEN);

  // faz a copia dos dados da memoria do kernel para a memoria do espaço de usuario
  // o copyout ele verifica a copia não ultrapassa o limite da memoria do processo (p->sz)
  // percorre o pagetable para descobrir o espaço real da memoria na RAM e depois copia os dados de uname (kernel) para a memoria
  // se algo der errado nesse caminho ele retorna -1
  if(copyout(p->pagetable, p->sz, endereco, (char *)&uname, sizeof(uname)) < 0) {
    return -1;
  }

  return 0;
}