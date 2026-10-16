# Trabalho Prático de Sistemas Operacionais — Extensão do xv6-RISC-V

## 1. Identificação do Grupo e Commit-Base
* **Integrantes:**
  * Cauã de Novaes Wernek
  * Thiago do Carmo
  * Matheus Matias
* **Repositório Base:** `https://github.com/mit-pdos/xv6-riscv`
* **Commit-base original (antes das modificações):** `4ded343d5d9d40d4e9dcf4867ca45b14170bf364`

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