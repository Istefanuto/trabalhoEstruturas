#ifndef ESTOQUE_H
#define ESTOQUE_H

#include <stdio.h>

// Estrutura para medicamento
typedef struct {
    char nome[20];
    int codigo;
    float valor;
    int data[3]; // Supondo data como um array [dia, mês, ano]
} Medicamento;

// Estrutura para nó da lista encadeada
typedef struct lista {
    Medicamento *m;
    struct lista *prox;
} Lista;

// Funções do TAD Estoque
Medicamento *CriaMedicamento(char *nome, int codigo, float valor, int *data_de_validade);
Lista *CriaLista();
Lista *InsereListaMedicamento(FILE *fp, Lista *l, Medicamento *m);
Lista *RetiraListaMedicamento(FILE *fp, Lista *l, int id_medicamento);
int VerificaListaMedicamento(FILE *fp, Lista *p, int id_medicamento);
int VerificaListaValidade(FILE *fp, Lista *p, int *data);
void ImprimeListaMedicamentos(FILE *fp, Lista *p);
Lista *OrdenaListaValor(Lista *p);
Lista *OrdenaListaVencimento(Lista *p);

#endif /* ESTOQUE_H */
