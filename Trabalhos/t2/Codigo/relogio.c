// relogio.c
// dispositivo de E/S para contar instruções executadas
// simulador de computador
// so25b

#include "relogio.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

struct relogio_t {
  // que horas são (em tics)
  int agora;
  // quanto tempo até gerar uma interrupção
  int t_ate_interrupcao;
  // true se está gerando interrupção
  bool interrupcao_ativa;
};

relogio_t *relogio_cria(void)
{
  relogio_t *self;
  self = malloc(sizeof(relogio_t));
  assert(self != NULL);

  self->agora = 0;

  return self;
}

void relogio_destroi(relogio_t *self)
{
  free(self);
}

void relogio_tictac(relogio_t *self)
{
  self->agora++;
  // vê se tem que gerar interrupção
  if (self->t_ate_interrupcao != 0) {
    self->t_ate_interrupcao--;
    if (self->t_ate_interrupcao == 0) {
      self->interrupcao_ativa = true;
    }
  }
}

err_t relogio_leitura(void *disp, int id, int *pvalor)
{
  relogio_t *self = disp;
  err_t err = ERR_OK;
  switch (id) {
    case 0:
      *pvalor = self->agora;
      break;
    case 1:
      *pvalor = clock() / (CLOCKS_PER_SEC / 1000);
      break;
    case 2:
      *pvalor = self->t_ate_interrupcao;
      break;
    case 3:
      *pvalor = self->interrupcao_ativa;
      break;
    default: 
      err = ERR_END_INV;
  }
  return err;
}

err_t relogio_escrita(void *disp, int id, int pvalor)
{
  relogio_t *self = disp;
  err_t err = ERR_OK;
  switch (id) {
    case 2:
      self->t_ate_interrupcao = pvalor;
      break;
    case 3:
      self->interrupcao_ativa = (pvalor != 0);
      break;
    default: 
      err = ERR_END_INV;
  }
  return err;
}
