// cpu.h
// executor de instruções da CPU
// simulador de computador
// so25b

#ifndef CPU_H
#define CPU_H

typedef struct cpu_t cpu_t; // tipo opaco

// os modos de execução da CPU
typedef enum { supervisor, usuario } cpu_modo_t;

// endereços na memória onde a CPU salva os valores dos registradores
//   quando aceita uma interrupção, e de onde recupera esses valores
//   quando retorna de uma interrupção
#define CPU_END_PC          50
#define CPU_END_A           51
#define CPU_END_erro        52
#define CPU_END_complemento 53

// endereço inicial do PC quando o processador é inicializado
#define CPU_END_RESET        0

// endereço para onde desviar quando aceita uma interrupção
#define CPU_END_TRATADOR    60

// endereço limite da memória só de leitura
#define CPU_END_FIM_ROM     49
// endereço limite da memória protegida (não acessável em modo usuário)
#define CPU_END_FIM_PROT    99

#include "memoria.h"
#include "es.h"

// cria uma unidade de execução com acesso à memória e ao
//   controlador de E/S fornecidos
cpu_t *cpu_cria(mem_t *mem, es_t *es);

// destrói a unidade de execução
void cpu_destroi(cpu_t *self);

// executa a instrução apontada pelo PC
//   se a CPU estiver em erro, não executa
//   se a execução causar algum erro, altera o estado da CPU
//     e causa uma interrupção
void cpu_executa_1(cpu_t *self);

// implementa uma interrupção
// passa para modo supervisor, salva o estado da CPU no início da memória,
//   altera A para identificar a requisição de interrupção, altera PC para
//   o endereço do tratador de interrupção
// retorna true se interrupção foi aceita ou false caso contrário
bool cpu_interrompe(cpu_t *self, irq_t irq);

// concatena a descrição do estado da CPU no final de str
void cpu_concatena_descricao(cpu_t *self, char *str);

#endif // CPU_H
