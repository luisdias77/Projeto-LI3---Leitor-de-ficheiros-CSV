#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_aeroportos.h"
#include "queries/query3.h"

// ----------------------
// 1. ESTRUTURAS DE DADOS
// -----------------------
typedef struct {
    int data_int;    // Data em número (ex: 20231005)
    char *origem;    // Código do aeroporto (ex: "OPO")
} VooQ3;

// Lista global de voos
static VooQ3 *lista_voos = NULL;
static int total_voos = 0;

// -----------------------
// 2. FUNÇÕES AUXILIARES 
// -----------------------

// Transforma "2023-10-05" em 20231005
static int data_para_int(const char *data_str) {
    int ano = (data_str[0] - '0') * 1000 + (data_str[1] - '0') * 100 + (data_str[2] - '0') * 10 + (data_str[3] - '0');
    int mes = (data_str[5] - '0') * 10 + (data_str[6] - '0');
    int dia = (data_str[8] - '0') * 10 + (data_str[9] - '0');
    return (ano * 10000) + (mes * 100) + dia;
}

// Função para ordenar a lista por data (usada pelo qsort)
static int comparar_por_data(const void* a, const void* b) {
    VooQ3* v1 = (VooQ3*)a;
    VooQ3* v2 = (VooQ3*)b;
    return (v1->data_int - v2->data_int);
}

// Limpar memória no fim
void destroy_query3() {
    if (lista_voos) {
        for (int i = 0; i < total_voos; i++) {
            free(lista_voos[i].origem);
        }
        free(lista_voos);
        lista_voos = NULL;
    }
}
// -----------------
// 3. INICIALIZAÇÃO 
// -----------------
void init_query3(GestorVoos *gv) {
    if (lista_voos != NULL) return; 

    GHashTable* tabela = get_tabela_voos(gv);
    int tamanho_estimado = g_hash_table_size(tabela);
    
    // Alocamos espaço para a lista
    lista_voos = malloc(sizeof(VooQ3) * tamanho_estimado);
    
    GHashTableIter iter;
    gpointer chave, valor;
    g_hash_table_iter_init(&iter, tabela);

    int i = 0;
    while (g_hash_table_iter_next(&iter, &chave, &valor)) {
        Voos* v = (Voos*)valor;
        
        // Se o voo é Cancelado ou N/A, nem sequer entra na lista.
        const char* status = get_status(v);
        const char* data_str = get_actual_departure(v);
        const char* origem = get_origin(v);

        if (v && status && data_str && origem &&
            strcasecmp(status, "Cancelled") != 0 &&
            strcasecmp(data_str, "N/A") != 0) {
            
            // Guardamos na lista
            lista_voos[i].data_int = data_para_int(data_str);
            lista_voos[i].origem = g_strdup(origem);
            i++;
        }
    }

    total_voos = i;
    
    // Ordenamos a lista por data.
    qsort(lista_voos, total_voos, sizeof(VooQ3), comparar_por_data);
}

// ------------
// 4. QUERY 3 
// ------------
void query3(GestorVoos* g, GestorAeroportos* a, char* data_inicio_str, char* data_fim_str, char separador, FILE* output) {
    (void)g; // Evitar warning se g não for usado

    if (total_voos == 0) {
        fprintf(output, "\n");
        return;
    }

    // Converter as datas do input para números (int)
    int inicio = data_para_int(data_inicio_str);
    int fim = data_para_int(data_fim_str);

    // Tabela para contar partidas
    GHashTable* contagem = g_hash_table_new(g_str_hash, g_str_equal);
    
    char *melhor_aeroporto = NULL;
    int max_partidas = 0;

    // Percorrer a lista ordenada
    for (int i = 0; i < total_voos; i++) {
        
        int data_atual = lista_voos[i].data_int;

        // 1. Se a data for menor que o inicio, passa à frente
        if (data_atual < inicio) {
            continue; 
        }

        // 2. Se a data for maior que o fim, para o loop
        // Como a lista está ordenada, sabemos que não há mais nada interessante para a frente.
        if (data_atual > fim) {
            break; 
        }

        // 3. Se chegou aqui, está no intervalo certo
        char *origem = lista_voos[i].origem;
        
        int qtd = GPOINTER_TO_INT(g_hash_table_lookup(contagem, origem));
        qtd++;
        g_hash_table_insert(contagem, origem, GINT_TO_POINTER(qtd));

        // Verificar vencedor
        if (qtd > max_partidas) {
            max_partidas = qtd;
            melhor_aeroporto = origem;
        } 
        else if (qtd == max_partidas) {
            // Desempate por nome
            if (melhor_aeroporto == NULL || strcmp(origem, melhor_aeroporto) < 0) {
                melhor_aeroporto = origem;
            }
        }
    }

    // Imprimir resultado
    if (max_partidas == 0 || melhor_aeroporto == NULL) {
        fprintf(output, "\n");
    } else {
        Aeroportos* ap = get_aeroporto(a, melhor_aeroporto);
        if (ap) {
            fprintf(output, "%s%c%s%c%s%c%s%c%d\n", 
                get_code_aeroporto(ap), separador,
                get_name_aeroporto(ap), separador,
                get_city_aeroporto(ap), separador,
                get_country_aeroporto(ap), separador,
                max_partidas);
        } else {
            fprintf(output, "\n");
        }
    }

    g_hash_table_destroy(contagem);
}