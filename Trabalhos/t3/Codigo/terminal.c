// terminal.c
// entrada e saída em um terminal
// simulador de computador
// so25b

#include "terminal.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

// TERMINAL

// dados para um terminal
struct terminal_t {
  // número de caracteres que cabem em uma linha
  int tam_linha;
  // texto já digitado no terminal, esperando para ser lido
  char *entrada;
  // texto sendo mostrado na saída do terminal
  char *saida;
  // estado da saída do terminal, que pode ser:
  // normal: aceitando novos caracteres na saída
  // rolando: removendo um caractere no início para gerar espaço.
  //   move um caractere por vez para a esquerda, até chegar no final
  //   da linha, então volta ao estado normal.
  //   entra neste estado quando recebe um caractere na última posição.
  //   não aceita novos caracteres
  // limpando: removendo um caractere no início da linha por vez, até
  //   ficar com a linha vazia, então volta ao estado normal.
  //   entra nesse estado quando recebe um '\n'.
  //   não aceita novos caracteres
  enum { normal, rolando, limpando } estado_saida;
  // posicao do caractere que está sendo movido durante uma rolagem
  int pos_rolagem;
};


terminal_t *terminal_cria(int tam_linha)
{
  terminal_t *self = malloc(sizeof(*self));
  assert(self != NULL);

  self->tam_linha = tam_linha;

  self->saida = calloc(1, tam_linha + 1);
  self->entrada = calloc(1, tam_linha + 1);
  assert(self->saida != NULL && self->entrada != NULL);

  self->estado_saida = normal;

  return self;
}

void terminal_destroi(terminal_t *self)
{
  free(self->entrada);
  free(self->saida);
  free(self);
}

static bool terminal_entrada_vazia(terminal_t *self)
{
  return self->entrada[0] == '\0';
}

static err_t terminal_le_char(terminal_t *self, int *pch)
{
  if (terminal_entrada_vazia(self)) return ERR_OCUP;
  char *p = self->entrada;
  *pch = *p;
  memmove(p, p + 1, strlen(p));
  return ERR_OK;
}

void terminal_insere_char(terminal_t *self, char ch)
{
  char *p = self->entrada;
  int tam = strlen(p);
  // se não cabe, ignora silenciosamente
  if (tam >= self->tam_linha - 2) return;
  p[tam] = ch;
  p[tam + 1] = '\0';
}

static bool terminal_pode_imprimir(terminal_t *self)
{
  return self->estado_saida == normal;
}

static err_t terminal_imprime(terminal_t *self, char ch)
{
  if (!terminal_pode_imprimir(self)) return ERR_OCUP;

  if (ch == '\n') {
    // se for impresso \n, inicia a limpeza da linha
    self->estado_saida = limpando;
  } else {
    // insere o caractere no final da linha
    int tam = strlen(self->saida);
    self->saida[tam] = ch;
    tam++;
    self->saida[tam] = '\0';
    // se encheu a linha, inicia a rolagem
    if (tam >= self->tam_linha - 1) {
      self->estado_saida = rolando;
      self->pos_rolagem = 0;
    }
  }
  return ERR_OK;
}

void terminal_limpa_saida(terminal_t *self)
{
  self->saida[0] = '\0';
  self->estado_saida = normal;
}

static void terminal_atualiza_rolagem(terminal_t *self)
{
  if (self->estado_saida != rolando) return;
  // rola um caractere e avança a posição de rolagem
  char *p = self->saida;
  char ch = p[self->pos_rolagem + 1];
  p[self->pos_rolagem] = ch;
  self->pos_rolagem++;
  p[self->pos_rolagem] = ' ';
  // se chegou no final da string, volta ao estado normal
  if (ch == '\0') self->estado_saida = normal;
}

static void terminal_atualiza_limpeza(terminal_t *self)
{
  if (self->estado_saida != limpando) return;
  // remove um caractere do início da string
  char *p = self->saida;
  int tam = strlen(p);
  memmove(p, p + 1, tam);
  tam--;
  // volta ao estado normal se era o último
  if (tam <= 0) self->estado_saida = normal;
}

// altera a string de saída em 1 caractere, se estiver rolando ou limpando
void terminal_tictac(terminal_t *self)
{
  terminal_atualiza_rolagem(self);
  terminal_atualiza_limpeza(self);
}

char *terminal_txt_entrada(terminal_t *self)
{
  return self->entrada;
}

char *terminal_txt_saida(terminal_t *self)
{
  return self->saida;
}

// Operações de leitura e escrita no terminal, chamadas pelo controlador de E/S
// Para o controlador, cada terminal é composto por 4 subdispositivos:
//   leitura, estado da leitura, escrita, estado da escrita
err_t terminal_leitura(void *disp, int id, int *pvalor)
{
  terminal_t *self = disp;
  int subdisp = id % 4;
  switch (subdisp) {
    case TERM_TECLADO: // leitura do teclado
      return terminal_le_char(self, pvalor);
    case TERM_TECLADO_OK: // estado do teclado
      *pvalor = !terminal_entrada_vazia(self);
      break;
    case TERM_TELA: // escrita na tela (proibido ler)
      return ERR_OP_INV;
    case TERM_TELA_OK: // estado da tela
      *pvalor = terminal_pode_imprimir(self);
      break;
    default:
      return ERR_DISP_INV;
  }
  return ERR_OK;
}

err_t terminal_escrita(void *disp, int id, int valor)
{
  terminal_t *self = disp;
  int subdisp = id % 4;
  // só pode escrever na tela
  if (subdisp != TERM_TELA) return ERR_OP_INV;
  return terminal_imprime(self, valor);
}
