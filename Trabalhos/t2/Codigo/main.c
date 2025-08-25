// main.c
// inicializa e encerra a simulação
// simulador de computador
// so25b

#include "controle.h"
#include "programa.h"
#include "memoria.h"
#include "cpu.h"
#include "relogio.h"
#include "console.h"
#include "terminal.h"
#include "es.h"
#include "dispositivos.h"
#include "so.h"

#include <stdlib.h>
#include <stdio.h>

// constantes
#define MEM_TAM 10000        // tamanho da memória principal

// estrutura com os componentes do computador simulado
typedef struct {
  mem_t *mem;
  cpu_t *cpu;
  relogio_t *relogio;
  console_t *console;
  es_t *es;
  controle_t *controle;
} hardware_t;


// registra no controlador de es os 4 dispositivos do terminal 'id_term'
//   da console, com valores a partir de n_disp
static void registra_terminal(hardware_t *hw, int n_disp, char id_term)
{
  terminal_t *terminal;
  terminal = console_terminal(hw->console, id_term);
  // por exemplo, depois de registrado, quando o controlador de ES receber um
  //   pedido de leitura do dispositivo 'n_disp+TERM_TECLADO' (que é 4 para
  //   o terminal 'B'), vai chamar a função 'terminal_leitura', passando como
  //   argumentos o valor de 'terminal' (que é o terminal 'B' obtido acima) e
  //   o valor TERM_TECLADO
  es_registra_dispositivo(hw->es, n_disp + TERM_TECLADO,    terminal, TERM_TECLADO,    terminal_leitura, NULL);
  es_registra_dispositivo(hw->es, n_disp + TERM_TECLADO_OK, terminal, TERM_TECLADO_OK, terminal_leitura, NULL);
  es_registra_dispositivo(hw->es, n_disp + TERM_TELA,       terminal, TERM_TELA,       NULL, terminal_escrita);
  es_registra_dispositivo(hw->es, n_disp + TERM_TELA_OK,    terminal, TERM_TELA_OK,    terminal_leitura, NULL);
}

// inicializa a memória ROM com o conteúdo do programa em bios.maq
static void inicializa_rom(mem_t *mem)
{
  // programa para executar na nossa CPU
  programa_t *prog = prog_cria("bios.maq");
  if (prog == NULL) {
    fprintf(stderr, "Erro na leitura da ROM ('bios.maq')\n");
    exit(1);
  }

  int end_ini = prog_end_carga(prog);
  if (end_ini != CPU_END_RESET) {
    fprintf(stderr, "ROM não inicia no endereço %d (%d)\n", CPU_END_RESET, end_ini);
    exit(1);
  }
  int end_fim = end_ini + prog_tamanho(prog);
  if (end_fim > CPU_END_FIM_ROM) {
    fprintf(stderr, "conteúdo da ROM muito grande (%d>%d)\n", end_fim, CPU_END_FIM_ROM);
    exit(1);
  }

  for (int end = end_ini; end < end_fim; end++) {
    if (mem_escreve(mem, end, prog_dado(prog, end)) != ERR_OK) {
      printf("Erro na carga da memória ROM, endereco %d\n", end);
      exit(1);
    }
  }
  prog_destroi(prog);
}

static void cria_hardware(hardware_t *hw)
{
  // cria a memória
  hw->mem = mem_cria(MEM_TAM);
  inicializa_rom(hw->mem);

  // cria dispositivos de E/S
  hw->console = console_cria();
  hw->relogio = relogio_cria();

  // cria o controlador de E/S e registra os dispositivos
  //   por exemplo, o dispositivo 8 do controlador de E/S (e da CPU) será o
  //   dispositivo 0 do relógio (que é o contador de instruções)
  hw->es = es_cria();
  // registra os 4 dispositivos de cada terminal
  registra_terminal(hw, D_TERM_A, 'A');
  registra_terminal(hw, D_TERM_B, 'B');
  registra_terminal(hw, D_TERM_C, 'C');
  registra_terminal(hw, D_TERM_D, 'D');
  // registra os 4 dispositivos do relógio
  es_registra_dispositivo(hw->es, D_RELOGIO_INSTRUCOES, hw->relogio, 0, relogio_leitura, NULL);
  es_registra_dispositivo(hw->es, D_RELOGIO_REAL      , hw->relogio, 1, relogio_leitura, NULL);
  es_registra_dispositivo(hw->es, D_RELOGIO_TIMER     , hw->relogio, 2, relogio_leitura, relogio_escrita);
  es_registra_dispositivo(hw->es, D_RELOGIO_INTERRUPCAO,hw->relogio, 3, relogio_leitura, relogio_escrita);

  // cria a unidade de execução e inicializa com a memória e o controlador de E/S
  hw->cpu = cpu_cria(hw->mem, hw->es);

  // cria o controlador da CPU e inicializa com a unidade de execução, a console e
  //   o relógio
  hw->controle = controle_cria(hw->cpu, hw->console, hw->relogio);
}

static void destroi_hardware(hardware_t *hw)
{
  controle_destroi(hw->controle);
  cpu_destroi(hw->cpu);
  es_destroi(hw->es);
  relogio_destroi(hw->relogio);
  console_destroi(hw->console);
  mem_destroi(hw->mem);
}

int main()
{
  hardware_t hw;
  so_t *so;

  // cria o hardware
  cria_hardware(&hw);
  // cria o sistema operacional
  so = so_cria(hw.cpu, hw.mem, hw.es, hw.console);

  // executa o laço principal do controlador
  controle_laco(hw.controle);

  // destroi tudo
  so_destroi(so);
  destroi_hardware(&hw);
}

