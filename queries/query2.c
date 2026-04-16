#define _GNU_SOURCE
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_aeronaves.h"
#include "queries/query2.h"
#include "entidades/voos.h"
#include "entidades/aeronaves.h"

struct gestor_voos {
    GHashTable* tabela_voos;  // chave = id do voo, valor = Voos*
};
struct gestor_aeronaves {
    GHashTable* tabela_aeronaves;  // chave = id da aeronave, valor = Aeronaves*
};


// ---------------------------------------------------------
// ESTRUTURAS E VARIÁVEIS GLOBAIS (CACHE)
// ---------------------------------------------------------

//estrutura para guardar resultados da query 2
typedef struct {
    char *aircraft_id;
    char *manufacturer;
    char *model;
    int num_voos;
} ResultadosQ2;

// Variáveis estáticas para "Cache" (persistem entre chamadas da função)
static ResultadosQ2 **cache_q2 = NULL;
static int total_aeronaves_cache = 0;

// ---------------------------------------------------------
// FUNÇÕES AUXILIARES
// ---------------------------------------------------------

// 2. Definição da função comparar_q2 
static int comparar_q2(const void* a, const void* b) {
    ResultadosQ2* res_a = *(ResultadosQ2**)a;
    ResultadosQ2* res_b = *(ResultadosQ2**)b;

    // 1. Ordenar por contagem (descendente)
    if  (res_a->num_voos > res_b->num_voos) return -1;
    if  (res_a->num_voos < res_b->num_voos) return 1;

    // 2. Desempate: Ordenar por ID (ascendente)
    return strcmp(res_a->aircraft_id, res_b->aircraft_id);
}

// Função para libertar memória da cache 
void destroy_query2() {
    if (cache_q2) {
        for (int i = 0; i < total_aeronaves_cache; i++) {
            free(cache_q2[i]->aircraft_id);
            free(cache_q2[i]->manufacturer);
            free(cache_q2[i]->model);
            free(cache_q2[i]);
        }
        free(cache_q2);
        cache_q2 = NULL;
    }
}

// -------------------------
// FUNÇÃO DE INICIALIZAÇÃO 
// -------------------------
void init_query2(GestorVoos *gv, GestorAeronaves *ga) {
    if (cache_q2 != NULL) return; // Já inicializado

    // 1. Tabela temporária para contar voos por ID de aeronave
    GHashTable *contagem = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    GHashTableIter iter;
    gpointer chave, valor;
    g_hash_table_iter_init(&iter, gv->tabela_voos);

    while (g_hash_table_iter_next(&iter, &chave, &valor)) {
        Voos *v = (Voos *)valor;
        const char *status = get_status(v);
        
        // Validação básica e filtro de cancelados
        if (status && strcasecmp(status, "Cancelled") != 0) {
            const char *air_id = get_aircraft(v);
            if (air_id) {
                // Incrementa contador
                int *count = g_hash_table_lookup(contagem, air_id);
                if (count) {
                    (*count)++;
                } else {
                    int *new_count = malloc(sizeof(int));
                    *new_count = 1;
                    g_hash_table_insert(contagem, g_strdup(air_id), new_count);
                }
            }
        }
    }

    // 2. Converter Tabela Hash para Array (Cache)
    total_aeronaves_cache = g_hash_table_size(contagem);
    if (total_aeronaves_cache == 0) {
        g_hash_table_destroy(contagem);
        return;
    }

    cache_q2 = malloc(sizeof(ResultadosQ2*) * total_aeronaves_cache);
    
    int i = 0;
    g_hash_table_iter_init(&iter, contagem);
    while (g_hash_table_iter_next(&iter, &chave, &valor)) {
        char *id = (char *)chave;
        int qtd = *(int *)valor;

        cache_q2[i] = malloc(sizeof(ResultadosQ2));
        cache_q2[i]->aircraft_id = g_strdup(id);
        cache_q2[i]->num_voos = qtd;

        // Guardamos logo strings duplicadas para acesso rápido na query
        Aeronaves *a = g_hash_table_lookup(ga->tabela_aeronaves, id);
        if (a) {
            cache_q2[i]->manufacturer = g_strdup(get_manufacturer_aeronave(a));
            cache_q2[i]->model = g_strdup(get_model_aeronave(a));
        } else {
            cache_q2[i]->manufacturer = g_strdup("Desconhecido");
            cache_q2[i]->model = g_strdup("Desconhecido");
        }
        i++;
    }

    g_hash_table_destroy(contagem);

    // 3. Ordenar a Cache uma única vez
    qsort(cache_q2, total_aeronaves_cache, sizeof(ResultadosQ2*), comparar_q2);
}

// ----------
// QUERY 2 
// ---------
void query2(int N, char* filtro, GestorVoos *gv, GestorAeronaves *ga, char separador, FILE *output) {
    (void)gv; (void)ga; // Não usados aqui porque usamos a cache, que foi iniciada na main

    // Se a cache estiver vazia ou não foi iniciada 
    if (cache_q2 == NULL || total_aeronaves_cache == 0) {
        fprintf(output, "\n");
        return;
    }

    int impressos = 0;
    int tem_filtro = (filtro != NULL && filtro[0] != '\0');

    for (int i = 0; i < total_aeronaves_cache; i++) {
        // Se já imprimimos N, paramos 
        if (impressos >= N) break;

        ResultadosQ2 *res = cache_q2[i];

        // Se houver filtro, verifica se o fabricante corresponde
        if (tem_filtro) {
            // Se o fabricante for diferente, salta para o próximo
            if (strcasecmp(res->manufacturer, filtro) != 0) {
                continue;
            }
        }

        // Imprime
        fprintf(output, "%s%c%s%c%s%c%d\n", 
                res->aircraft_id, separador,
                res->manufacturer, separador,
                res->model, separador,
                res->num_voos);
        
        impressos++;
    }

    // Escreve linha em branco
    if (impressos == 0) {
        fprintf(output, "\n");
    }
}