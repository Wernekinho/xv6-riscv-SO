# Declaração de Uso de Modelos de Linguagem (LLM)

## 1. Ferramentas e Integrantes
* **Ferramentas utilizadas:** Google Gemini e ChatGPT (ambas em suas versões Web).
* **Integrantes que utilizaram:**
  * Cauã de Novaes Wernek
  * Thiago do Carmo Dorigo dos Santos
  * Matheus da Silva Matias

---

## 2. Onde a Ferramenta foi Utilizada
As ferramentas foram utilizadas como apoio nas etapas de estudo da arquitetura do xv6-RISC-V, implementação dos comandos e chamadas ao sistema, depuração e estruturação da documentação, afetando os seguintes comandos, arquivos e funções:

* **Arquitetura geral e fluxo de chamadas ao sistema (todos os integrantes):** Estudo do percurso de uma syscall entre espaço de usuário e kernel (`ecall`, `uservec`, `kernelvec`, `trampoline.S`, registro em `kernel/syscall.h`, despacho em `kernel/syscall.c`, stubs em `user/usys.pl` e protótipos em `user/user.h`) e inclusão dos binários em `UPROGS` no `Makefile`.
* **Comandos `uname` e `hostname` (Cauã de Novaes Wernek):**
  * Estrutura global compartilhada do sistema (`kernel/utsname.h` / estrutura mantida no kernel para reutilização entre ambos os comandos).
  * Tratadores no kernel (`sys_uname` e `sys_sethostname` em `kernel/sysproc.c`).
  * Programas de usuário (`user/uname.c` e `user/hostname.c`).
* **Comando `ps` (Thiago do Carmo Dorigo dos Santos):**
  * Consulta à tabela de processos (`kernel/proc.c`, tomando como referência a estrutura de `procdump`), contagem de memória utilizada por processo e implementação do utilitário de usuário (`user/ps.c`) com verificação de argumentos.
* **Comando `free` (Matheus da Silva Matias):**
  * Estudo e extensão do alocador de memória física (`kernel/kalloc.c`), envolvendo a lista `freelist` e as funções `kinit()`, `freerange()`, `kfree()` e `kalloc()`, além do contador de páginas livres protegido pelo `spinlock` do alocador e do utilitário de usuário (`user/free.c`).

---

## 3. Como foi Utilizada (Consultas, Aceitação, Adaptação e Rejeição)

* **Uso conjunto do grupo:**
  * Utilizamos a LLM para melhor compreender o percurso de uma chamada de sistema entre o programa de usuário e o kernel na prática, além da organização dos arquivos envolvidos na implementação dos comandos e das syscalls no xv6-RISC-V.

* **Cauã de Novaes Wernek (`uname` e `hostname`):**
  * Utilizado para auxílio na implementação dos comandos `uname` e `hostname`, principalmente para o entendimento da implementação original do Linux, da estrutura de chamadas ao sistema no xv6 e na resolução de erros durante a compilação e execução.
  * *Adaptação:* As orientações de estrutura de arquivos, registro da syscall e utilitário de espaço de usuário foram adaptadas ao funcionamento do sistema do xv6.
  * *Rejeição:* Foi rejeitada uma ideia de implementação do `uname` em que originalmente a LLM manteve os dados da máquina dentro da própria chamada ao sistema do `uname`. No final, decidimos fazer algo mais estruturado e manter a estrutura de dado em geral no sistema, permitindo que ela fosse reutilizada no `hostname`.

* **Thiago do Carmo Dorigo dos Santos (`ps`):**
  * Utilizado na implementação do comando `ps`, servindo de apoio para corrigir partes do código, como a verificação de argumentos passados pelo usuário e o cálculo/contagem de memória utilizada pelos processos.
  * *Rejeição e adaptação:* Foram descartados códigos gerados em que a LLM criou uma implementação nova do zero sem seguir a estrutura existente do xv6. Ao final, optou-se por seguir uma abordagem mais próxima da documentação oficial do comando `ps` e da própria estrutura interna de `procdump` já presente no kernel do xv6.

* **Matheus da Silva Matias (`free`):**
  * Utilizado na implementação do comando `free` como ferramenta de consulta para a compreensão do fluxo de traps (`ecall`, `uservec`, `kernelvec`, `trampoline.S`) e do funcionamento interno do alocador de memória física em `kernel/kalloc.c` (estrutura `freelist` e rotinas `kinit()`, `freerange()`, `kfree()` e `kalloc()`).
  * *Decisão de projeto:* Foram discutidas com a LLM duas alternativas para obter a quantidade de páginas livres: (1) percorrer toda a lista encadeada `freelist` a cada consulta, ou (2) manter um contador atualizado dinamicamente conforme as páginas são alocadas e liberadas. Ao final, decidiu-se adotar a segunda alternativa, implementando um contador mantido pelo próprio alocador e protegido pelo seu `spinlock`.