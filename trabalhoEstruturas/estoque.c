#include "estoque.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Função para criar um novo medicamento
Medicamento *CriaMedicamento(char *nome, int codigo, float valor, int *data_de_validade) {
    Medicamento *med = (Medicamento *)malloc(sizeof(Medicamento));
    if (med != NULL) {
        strcpy(med->nome, nome);
        med->codigo = codigo;
        med->valor = valor;
        memcpy(med->data, data_de_validade, sizeof(int) * 3);
    }
    return med;
}

// Função para criar uma nova lista vazia
Lista *CriaLista() {
    return NULL;
}

// Função para inserir um medicamento em uma lista
Lista *InsereListaMedicamento(FILE *fp, Lista *l, Medicamento *m) {
    Lista *novo = (Lista *)malloc(sizeof(Lista));
    if (novo == NULL) {
        fprintf(fp, "ERRO: Falha na alocação de memória.\n");
        return l;
    }
    novo->m = m;
    novo->prox = l;
    fprintf(fp, "MEDICAMENTO %s %d ADICIONADO\n", m->nome, m->codigo);
    return novo;
}

// Função para retirar um medicamento de uma lista
Lista *RetiraListaMedicamento(FILE *fp, Lista *l, int id_medicamento) {
    Lista *ant = NULL;
    Lista *atual = l;
    
    while (atual != NULL && atual->m->codigo != id_medicamento) {
        ant = atual;
        atual = atual->prox;
    }
    
    if (atual == NULL) {
        fprintf(fp, "MEDICAMENTO NAO ENCONTRADO NA LISTA\n");
        return l;
    }
    
    if (ant == NULL) {
        l = l->prox;
    } else {
        ant->prox = atual->prox;
    }
    
    fprintf(fp, "MEDICAMENTO %s %d RETIRADO\n", atual->m->nome, atual->m->codigo);
    free(atual->m);
    free(atual);
    
    return l;
}

// Função para verificar se um medicamento está presente em uma lista
int VerificaListaMedicamento(FILE *fp, Lista *p, int id_medicamento) {
    Lista *atual = p;
    while (atual != NULL) {
        if (atual->m->codigo == id_medicamento) {
            return 1; // Encontrou o medicamento na lista
        }
        atual = atual->prox;
    }
    fprintf(fp, "MEDICAMENTO NAO ENCONTRADO NA LISTA\n");
    return 0; // Medicamento não encontrado na lista
}

// Função para verificar se há algum medicamento vencido em uma lista
int VerificaListaValidade(FILE *fp, Lista *p, int *data) {
    Lista *atual = p;
    while (atual != NULL) {
        if (atual->m->data[2] < data[2] ||
            (atual->m->data[2] == data[2] && atual->m->data[1] < data[1]) ||
            (atual->m->data[2] == data[2] && atual->m->data[1] == data[1] && atual->m->data[0] < data[0])) {
            fprintf(fp, "MEDICAMENTO %s %d VENCIDO\n", atual->m->nome, atual->m->codigo);
            return 1; // Encontrou um medicamento vencido
        }
        atual = atual->prox;
    }
    fprintf(fp, "MEDICAMENTO VENCIDO NAO ENCONTRADO NA LISTA\n");
    return 0; // Nenhum medicamento vencido encontrado na lista
}

// Função para imprimir todos os medicamentos de uma lista
void ImprimeListaMedicamentos(FILE *fp, Lista *p) {
    Lista *atual = p;
    while (atual != NULL) {
        fprintf(fp, "%s %d %.2f %d/%d/%d\n", atual->m->nome, atual->m->codigo, atual->m->valor,
                atual->m->data[0], atual->m->data[1], atual->m->data[2]);
        atual = atual->prox;
    }
}

// Função para comparar dois medicamentos pelo valor (usada no qsort)
int CompararValor(const void *a, const void *b) {
    Medicamento *med1 = *((Medicamento **)a);
    Medicamento *med2 = *((Medicamento **)b);
    
    if (med1->valor < med2->valor) return -1;
    if (med1->valor > med2->valor) return 1;
    return 0;
}

// Função para ordenar a lista pelo valor do medicamento
Lista *OrdenaListaValor(Lista *p) {
    int tamanho = 0;
    Lista *atual = p;
    
    // Contar o número de elementos na lista
    while (atual != NULL) {
        tamanho++;
        atual = atual->prox;
    }
    
    // Criar um array de ponteiros para Medicamento
    Medicamento **array = (Medicamento **)malloc(tamanho * sizeof(Medicamento *));
    if (array == NULL) {
        // Tratar erro de alocação de memória
        return p;
    }
    
    // Preencher o array com os medicamentos da lista
    atual = p;
    int i = 0;
    while (atual != NULL) {
        array[i] = atual->m;
        atual = atual->prox;
        i++;
    }
    
    // Ordenar o array de acordo com o valor do medicamento
    qsort(array, tamanho, sizeof(Medicamento *), CompararValor);
    
    // Reconstruir a lista ordenada
    Lista *listaOrdenada = NULL;
    for (i = tamanho - 1; i >= 0; i--) {
        Lista *novo = (Lista *)malloc(sizeof(Lista));
        if (novo == NULL) {
            // Tratar erro de alocação de memória
            return p;
        }
        novo->m = array[i];
        novo->prox = listaOrdenada;
        listaOrdenada = novo;
    }
    
    // Liberar o array de ponteiros
    free(array);
    
    return listaOrdenada;
}

// Função para comparar dois medicamentos pela data de vencimento (usada no qsort)
int CompararVencimento(const void *a, const void *b) {
    Medicamento *med1 = *((Medicamento **)a);
    Medicamento *med2 = *((Medicamento **)b);
    
    if (med1->data[2] < med2->data[2]) return -1;
    if (med1->data[2] > med2->data[2]) return 1;
    if (med1->data[1] < med2->data[1]) return -1;
    if (med1->data[1] > med2->data[1]) return 1;
    if (med1->data[0] < med2->data[0]) return -1;
    if (med1->data[0] > med2->data[0]) return 1;
    return 0;
}

// Função para ordenar a lista pela data de vencimento do medicamento
Lista *OrdenaListaVencimento(Lista *p) {
    int tamanho = 0;
    Lista *atual = p;
    
    // Contar o número de elementos na lista
    while (atual != NULL) {
        tamanho++;
        atual = atual->prox;
    }
    
    // Criar um array de ponteiros para Medicamento
    Medicamento **array = (Medicamento **)malloc(tamanho * sizeof(Medicamento *));
    if (array == NULL) {
        // Tratar erro de alocação de memória
        return p;
    }
    
    // Preencher o array com os medicamentos da lista
    atual = p;
    int i = 0;
    while (atual != NULL) {
        array[i] = atual->m;
        atual = atual->prox;
        i++;
    }
    
    // Ordenar o array de acordo com a data de vencimento do medicamento
    qsort(array, tamanho, sizeof(Medicamento *), CompararVencimento);
    
    // Reconstruir a lista ordenada
    Lista *listaOrdenada = NULL;
    for (i = tamanho - 1; i >= 0; i--) {
        Lista *novo = (Lista *)malloc(sizeof(Lista));
        if (novo == NULL) {
            // Tratar erro de alocação de memória
            return p;
        }
        novo->m = array[i];
        novo->prox = listaOrdenada;
        listaOrdenada = novo;
    }
    
    // Liberar o array de ponteiros
    free(array);
    
    return listaOrdenada;
}

// Função para liberar a memória alocada para a lista de medicamentos
void LiberaLista(Lista *l) {
    Lista *atual = l;
    while (atual != NULL) {
        Lista *prox = atual->prox;
        free(atual->m);
        free(atual);
        atual = prox;
    }
}
