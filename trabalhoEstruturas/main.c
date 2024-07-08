#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estoque.h"

// Função auxiliar para ler data no formato dia/mês/ano
void LerData(FILE *entrada, int *data) {
    fscanf(entrada, "%d/%d/%d", &data[0], &data[1], &data[2]);
}

int main() {
    FILE *entrada = fopen("entrada.txt", "r");
    FILE *saida = fopen("saida.txt", "w");

    if (entrada == NULL || saida == NULL) {
        printf("Erro ao abrir arquivos.\n");
        return 1;
    }

    Lista *estoque = CriaLista();
    char operacao[20];
    while (fscanf(entrada, "%s", operacao) != EOF) {
        if (strcmp(operacao, "MEDICAMENTO") == 0) {
            // Ler dados do medicamento e adicionar à lista
            char nome[20];
            int codigo;
            float valor;
            int data[3];
            fscanf(entrada, "%s %d %f", nome, &codigo, &valor);
            LerData(entrada, data);
            Medicamento *med = CriaMedicamento(nome, codigo, valor, data);
            estoque = InsereListaMedicamento(saida, estoque, med);
        } else if (strcmp(operacao, "RETIRA") == 0) {
            // Remover medicamento da lista
            int id_medicamento;
            fscanf(entrada, "%d", &id_medicamento);
            estoque = RetiraListaMedicamento(saida, estoque, id_medicamento);
        } else if (strcmp(operacao, "VERIFICA_LISTA") == 0) {
            // Verificar se um medicamento está na lista
            int id_medicamento;
            fscanf(entrada, "%d", &id_medicamento);
            VerificaListaMedicamento(saida, estoque, id_medicamento);
        } else if (strcmp(operacao, "VERIFICA_VALIDADE") == 0) {
            // Verificar se há medicamento vencido na lista
            int data[3];
            LerData(entrada, data);
            VerificaListaValidade(saida, estoque, data);
        } else if (strcmp(operacao, "IMPRIME_LISTA") == 0) {
            // Imprimir todos os medicamentos da lista
            ImprimeListaMedicamentos(saida, estoque);
        } else if (strcmp(operacao, "ORDENA_LISTA_VALOR") == 0) {
            // Ordenar lista pelo valor dos medicamentos
            estoque = OrdenaListaValor(estoque);
        } else if (strcmp(operacao, "ORDENA_LISTA_VALIDADE") == 0) {
            // Ordenar lista pela data de validade dos medicamentos
            estoque = OrdenaListaVencimento(estoque);
        } else if (strcmp(operacao, "ATUALIZA_PRECO") == 0) {
            // Atualizar preço de um medicamento na lista
            int codigo;
            float novo_preco;
            fscanf(entrada, "%d %f", &codigo, &novo_preco);
            // Implemente esta funcionalidade se necessário
        } else if (strcmp(operacao, "FIM") == 0) {
            break;
        }
    }

    fclose(entrada);
    fclose(saida);

    // Liberar memória alocada para a lista de medicamentos
    Lista *aux;
    while (estoque != NULL) {
        aux = estoque;
        estoque = estoque->prox;
        free(aux->m);
        free(aux);
    }

    return 0;
}
