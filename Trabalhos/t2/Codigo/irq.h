// irq.h
// definições de constantes para interrupção
// simulador de computador
// so25b

#ifndef IRQ_H
#define IRQ_H

// os motivos para uma requisição de interrupção (irq) ser gerada
typedef enum {
  // o 0 não é usado em interrupções, é reservado para distinguir um reset
  IRQ_RESET,         // inicialização da CPU
  // interrupções geradas internamente na CPU
  IRQ_ERR_CPU,       // erro interno na CPU (ver registrador de erro)
  IRQ_SISTEMA,       // chamada de sistema
  // interrupções geradas por dispositivos de E/S
  IRQ_RELOGIO,       // interrupção causada pelo relógio
  // interrupções de E/S ainda não implementadas
  IRQ_TECLADO,       // interrupção causada pelo teclado
  IRQ_TELA,          // interrupção causada pela tela
  N_IRQ              // número de interrupções
} irq_t;

char *irq_nome(irq_t irq);

#endif // IRQ_H
