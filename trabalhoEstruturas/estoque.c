#include "estoque.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para comparar medicamentos por valor
int compara_valor(const void* a, const void* b) {
    Medicamento* m1 = *((Medicamento**)a);
    Medicamento* m2 = *((Medicamento**)b);
    
    if (m1->valor < m2->valor) return -1;
    if (m1->valor > m2->valor) return 1;
    return 0;
}

// Função auxiliar para comparar medicamentos por data de vencimento
int compara_data(const void* a, const void* b) {
    Medicamento* m1 = *((Medicamento**)a);
    Medicamento* m2 = *((Medicamento**)b);
    
    if (m1->data[2] != m2->data[2]) return m1->data[2] - m2->data[2];
    if (m1->data[1] != m2->data[1]) return m1->data[1] - m2->data[1];
    return m1->data[0] - m2->data[0];
}

// Cria um novo medicamento
Medicamento* CriaMedicamento(char* nome, int codigo, float valor, int* data_de_validade) {
    Medicamento* m = (Medicamento*)malloc(sizeof(Medicamento));
    if (m == NULL) return NULL;
    strncpy(m->nome, nome, 20);
    m->codigo = codigo;
    m->valor = valor;
    memcpy(m->data, data_de_validade, 3 * sizeof(int));
    return m;
}

// Cria uma nova lista
Lista* CriaLista() {
    return NULL;
}

// Insere um medicamento em uma lista
Lista* InsereListaMedicamento(FILE* fp, Lista* l, Medicamento* m) {
    Lista* novo = (Lista*)malloc(sizeof(Lista));
    if (novo == NULL) return l;
    novo->m = m;
    novo->prox = l;
    fprintf(fp, "MEDICAMENTO %s %d ADICIONADO\n", m->nome, m->codigo);
    return novo;
}

// Retira um medicamento de uma lista
Lista* RetiraListaMedicamento(FILE* fp, Lista* l, int id_medicamento) {
    Lista* ant = NULL;
    Lista* p = l;
    while (p != NULL && p->m->codigo != id_medicamento) {
        ant = p;
        p = p->prox;
    }
    if (p == NULL) {
        fprintf(fp, "MEDICAMENTO NAO ENCONTRADO NA LISTA\n");
        return l;
    }
    if (ant == NULL) {
        l = p->prox;
    } else {
        ant->prox = p->prox;
    }
    fprintf(fp, "MEDICAMENTO %s %d RETIRADO\n", p->m->nome, p->m->codigo);
    free(p->m);
    free(p);
    return l;
}

// Verifica se um medicamento está presente em uma lista
int VerificaListaMedicamento(FILE* fp, Lista* l, int id_medicamento) {
    Lista* p = l;
    while (p != NULL) {
        if (p->m->codigo == id_medicamento) {
            fprintf(fp, "MEDICAMENTO ENCONTRADO NA LISTA\n");
            return 1;
        }
        p = p->prox;
    }
    fprintf(fp, "MEDICAMENTO NAO ENCONTRADO NA LISTA\n");
    return 0; 
}

// Verifica se existe um medicamento vencido em uma lista
int VerificaListaValidade(FILE* fp, Lista* l, int* data) {
    Lista* p = l;
    while (p != NULL) {
        if (p->m->data[2] < data[2] || (p->m->data[2] == data[2] && p->m->data[1] < data[1]) || 
            (p->m->data[2] == data[2] && p->m->data[1] == data[1] && p->m->data[0] < data[0])) {
            fprintf(fp, "MEDICAMENTO %s %d VENCIDO\n", p->m->nome, p->m->codigo);
            return 1;
        }
        p = p->prox;
    }
    fprintf(fp, "MEDICAMENTO VENCIDO NAO ENCONTRADO NA LISTA\n");
    return 0;
}

// Imprime todos os medicamentos de uma lista
void ImprimeListaMedicamentos(FILE* fp, Lista* l) {
    Lista* p = l;
    while (p != NULL) {
        fprintf(fp, "%s %d %.1f %02d %02d %04d\n", p->m->nome, p->m->codigo, p->m->valor, p->m->data[0], p->m->data[1], p->m->data[2]);
        p = p->prox;
    }
}

// Ordena a lista pelo valor do medicamento
Lista* OrdenaListaValor(Lista* l) {
    if (l == NULL) return l;
    
    // Contar o número de elementos
    int n = 0;
    Lista* p = l;
    while (p != NULL) {
        n++;
        p = p->prox;
    }
    
    // Alocar memória para um array de ponteiros para Medicamento
    Medicamento** arr = (Medicamento**)malloc(n * sizeof(Medicamento*));
    if (arr == NULL) return l; // Verificar se a alocação foi bem-sucedida
    
    p = l;
    for (int i = 0; i < n; i++) {
        arr[i] = p->m;
        p = p->prox;
    }
    
    // Ordenar o array de ponteiros
    qsort(arr, n, sizeof(Medicamento*), compara_valor);
    
    // Reconstruir a lista ordenada
    p = l;
    for (int i = 0; i < n; i++) {
        p->m = arr[i];
        p = p->prox;
    }
    
    free(arr);
    return l;
}

// Ordena a lista pela data de vencimento do medicamento
Lista* OrdenaListaVencimento(Lista* l) {
    if (l == NULL) return l;
    
    // Contar o número de elementos
    int n = 0;
    Lista* p = l;
    while (p != NULL) {
        n++;
        p = p->prox;
    }
    
    // Alocar memória para um array de ponteiros para Medicamento
    Medicamento** arr = (Medicamento**)malloc(n * sizeof(Medicamento*));
    if (arr == NULL) return l; // Verificar se a alocação foi bem-sucedida
    
    p = l;
    for (int i = 0; i < n; i++) {
        arr[i] = p->m;
        p = p->prox;
    }
    
    // Ordenar o array de ponteiros
    qsort(arr, n, sizeof(Medicamento*), compara_data);
    
    // Reconstruir a lista ordenada
    p = l;
    for (int i = 0; i < n; i++) {
        p->m = arr[i];
        p = p->prox;
    }
    
    free(arr);
    return l;
}

// Libera toda a memória alocada para a lista
void LiberaLista(Lista* l) {
    Lista* p = l;
    while (p != NULL) {
        Lista* t = p->prox;
        free(p->m);
        free(p);
        p = t;
    }
}
