// dispositivos.h
// identificação dos dispositivos de entrada e saída
// simulador de computador
// so25b

#ifndef DISPOSITIVOS_H
#define DISPOSITIVOS_H

#include "terminal.h"

typedef enum {
  D_TERM_A,
  D_TERM_A_TECLADO        =  D_TERM_A + TERM_TECLADO,
  D_TERM_A_TECLADO_OK     =  D_TERM_A + TERM_TECLADO_OK,
  D_TERM_A_TELA           =  D_TERM_A + TERM_TELA,
  D_TERM_A_TELA_OK        =  D_TERM_A + TERM_TELA_OK,
  D_TERM_B,
  D_TERM_B_TECLADO        =  D_TERM_B + TERM_TECLADO,
  D_TERM_B_TECLADO_OK     =  D_TERM_B + TERM_TECLADO_OK,
  D_TERM_B_TELA           =  D_TERM_B + TERM_TELA,
  D_TERM_B_TELA_OK        =  D_TERM_B + TERM_TELA_OK,
  D_TERM_C,
  D_TERM_C_TECLADO        =  D_TERM_C + TERM_TECLADO,
  D_TERM_C_TECLADO_OK     =  D_TERM_C + TERM_TECLADO_OK,
  D_TERM_C_TELA           =  D_TERM_C + TERM_TELA,
  D_TERM_C_TELA_OK        =  D_TERM_C + TERM_TELA_OK,
  D_TERM_D,
  D_TERM_D_TECLADO        =  D_TERM_D + TERM_TECLADO,
  D_TERM_D_TECLADO_OK     =  D_TERM_D + TERM_TECLADO_OK,
  D_TERM_D_TELA           =  D_TERM_D + TERM_TELA,
  D_TERM_D_TELA_OK        =  D_TERM_D + TERM_TELA_OK,
  D_RELOGIO_INSTRUCOES,
  D_RELOGIO_REAL,
  N_DISPOSITIVOS
} dispositivo_id_t;

#endif // DISPOSITIVOS_H

