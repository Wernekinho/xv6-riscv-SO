# Trabalho Prático de Sistemas Operacionais — Extensão do xv6-RISC-V

## 1. Identificação do Grupo e Commit-Base
* **Integrantes:**
  * Cauã de Novaes Wernek
  * Thiago do Carmo
  * Matheus Matias
* **Repositório Base:** `https://github.com/mit-pdos/xv6-riscv`
* **Commit-base (antes das modificações):** `4ded343d5d9d40d4e9dcf4867ca45b14170bf364`

---

## 2. Ambiente e Versões das Ferramentas
* **Sistema Hospedeiro:** Windows 11 com WSL 2 (Debian)
* **Compilador Cruzado:** `riscv64-linux-gnu-gcc`
* **Emulador:** `qemu-system-riscv64`

### Instruções de Compilação e Execução
Para compilar o sistema do zero (garantindo a reconstrução limpa da imagem `fs.img`) e iniciar o QEMU:
```bash
make clean
make qemu
```

---

## 3. Arquivos Modificados

Makefile, 
kernel/defs.h,
kernel/file.c,
kernel/kalloc.c,
kernel/pipe.c,
kernel/proc.c,
kernel/syscall.c,
kernel/syscall.h,
kernel/sysproc.c,
kernel/trampoline.S,
kernel/uart.c,
user/sh.c,
user/user.h,
user/usertests.c,
user/usys.pl.

## 4. Comandos Implementados

### 4.1. `uname`
* **Exemplos de uso:**
  $ uname
  $ uname -s
  $ uname -n
  $ uname -r
  $ uname -v
  $ uname -m
  $ uname -a
  $ uname -sn
  $ uname -asnrvm
  $ uname -x

* **Resultados esperados:**
  * `uname` e `uname -s` exibem o nome do sistema (`xv6-riscv`).
  * `uname -n` exibe o `nodename` atual do sistema (refletindo alterações feitas via `hostname` / `sethostname`).
  * `uname -r` exibe a versão/release (`UTS_RELEASE`, via `git describe --always --dirty` ou fallback `"1.0.0"`).
  * `uname -v` exibe a versão de compilação (`UTS_VERSION`, `#1 SMP <data>`).
  * `uname -m` exibe a arquitetura (`riscv64` em 64 bits).
  * `uname -a` e `uname -asnrvm` exibem os campos selecionados separados por espaço em uma única linha; `uname -sn` exibe `sysname` e `nodename`.
  * Argumentos ou flags inválidos (como `uname -x`) exibem mensagem de erro/uso e retornam código diferente de `0`.
  * A syscall `uname(struct utsname*)` retorna `0` em sucesso e `-1` para ponteiros inválidos (sem causar `panic` no kernel).

* **Limitações conhecidas:**
  * Sem suporte a flags longas do GNU nem às opções `-p`, `-i` e `-o` do Linux.

### 4.2. `hostname` (`sethostname`)
* **Exemplos de uso:**
  $ hostname
  $ hostname novonome
  $ hostname
  $ hostname a b

* **Resultados esperados:**
  * Sem argumentos (`hostname`), exibe o `nodename` atual do sistema.
  * Com um argumento válido (`hostname novonome`), atualiza o `nodename` compartilhado no kernel via `sethostname` e passa a ser refletido em chamadas subsequentes a `hostname` e `uname -n` / `uname -a`.
  * Com argumentos em excesso (`hostname a b`), exibe mensagem de uso e encerra com erro.
  * A syscall `sethostname(const char*)` retorna `0` em sucesso e `-1` para string vazia, nome com mais de 64 caracteres úteis (`UTSLEN - 1`), ponteiro nulo (`NULL` / `0`) ou endereço inválido/fora dos limites de memória, sem causar `panic` no kernel.

* **Limitações conhecidas:**
  * Alterações ficam apenas em memória e não persistem após reiniciar o QEMU/kernel.

### 4.3. `ps`
* **Exemplos de uso:**
  $ ps

* **Resultados esperados:**
  * Lista os processos presentes na tabela de processos do kernel (`proc`), exibindo informações de identificação, estado, memória utilizada e nome do processo de forma consistente com a estrutura interna do xv6.
  * Com argumentos em excesso (`ps123`), exibe a mensagem de erro e uso e encerra.
	
### 4.4. `free`
* **Exemplos de uso:**
  $ free

* **Resultados esperados:**
  * Exibe a quantidade de memória física livre/disponível no kernel com base no contador de páginas livres mantido em `kernel/kalloc.c` e protegido pelo `spinlock` do alocador.
  * Após alocações de memória no espaço de usuário (ex.: `sbrk`/`malloc`), o valor de memória livre diminui e volta a subir após a liberação das páginas. 
	
### 4.5. `unametest` e `hostnametest`
* **Exemplos de uso:**
  $ unametest
  $ hostnametest

* **Resultados esperados:**
  * `unametest`: valida `uname()` com buffer válido (retorno `0` e campos preenchidos), ponteiros inválidos (`NULL`, `(void*)-1` e endereço acima do topo da memória retornando `-1` sem `panic`) e execução do binário `uname` com flags válidas e inválidas; encerra sem mensagens de falha.
  * `hostnametest`: valida alteração de hostname refletida em `uname()`, rejeição de string vazia e de 65 caracteres, aceitação do limite de 64 caracteres, rejeição de ponteiros inválidos (`NULL`, `(void*)-1` e acima do limite de memória) sem `panic`, execução concorrente com múltiplos processos via `fork()` e restauração do hostname original; encerra sem mensagens de falha.
