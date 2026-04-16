#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <glib.h>

#include "queries/query4.h"
#include "entidades/reservas.h"
#include "entidades/voos.h"
#include "entidades/passageiros.h"

// =========================================================
// ESTRUTURAS DE DADOS
// =========================================================

// Guarda o resultado pré-calculado de UMA semana
typedef struct {
    char *data_domingo;  // Data de início da semana (YYYY-MM-DD)
    char *top10_ids[10]; // IDs dos 10 passageiros que mais gastaram nesta semana
    int count;           // Quantos IDs existem (máximo 10)
} SemanaCache;

// Array global ordenado com todas as semanas
static SemanaCache *cache_semanas = NULL;
static int total_semanas_cache = 0;

// Auxiliar para ordenar passageiros durante o init
typedef struct {
    char *passageiro_id;
    double gasto_total;
} PassageiroGasto;

// =========================================================
// FUNÇÕES AUXILIARES
// =========================================================

// Calcula o Domingo da semana (YYYY-MM-DD)
static char* get_sunday_of_week(const char* date_str) {
    struct tm tm = {0};
    int year, month, day;
    if (sscanf(date_str, "%d-%d-%d", &year, &month, &day) != 3) return NULL;
    
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_isdst = -1;

    time_t raw_time = mktime(&tm);
    if (raw_time == -1) return NULL;

    // Recuar para Domingo
    raw_time -= tm.tm_wday * 24 * 3600;

    struct tm *sunday_tm = localtime(&raw_time);
    char *buffer = malloc(20);
    strftime(buffer, 20, "%Y-%m-%d", sunday_tm);
    return buffer;
}

// Comparar gastos (Decrescente valor, Crescente ID)
static gint compare_gastos(gconstpointer a, gconstpointer b) {
    const PassageiroGasto *p1 = (const PassageiroGasto *)a;
    const PassageiroGasto *p2 = (const PassageiroGasto *)b;

    if (p1->gasto_total > p2->gasto_total) return -1;
    if (p1->gasto_total < p2->gasto_total) return 1;
    return strcmp(p1->passageiro_id, p2->passageiro_id);
}

// Comparar Semanas por Data (para qsort)
static int compare_semanas_cache(const void *a, const void *b) {
    const SemanaCache *s1 = (const SemanaCache *)a;
    const SemanaCache *s2 = (const SemanaCache *)b;
    return strcmp(s1->data_domingo, s2->data_domingo);
}

// Limpar memória
void destroy_query4() {
    if (cache_semanas) {
        for (int i = 0; i < total_semanas_cache; i++) {
            free(cache_semanas[i].data_domingo);
            for (int j = 0; j < cache_semanas[i].count; j++) {
                free(cache_semanas[i].top10_ids[j]);
            }
        }
        free(cache_semanas);
        cache_semanas = NULL;
    }
}

// Extrai o primeiro ID de voo da lista raw (formato: "['ID1','ID2',...]")
static char* extrair_primeiro_voo(const char *raw_list) {
    if (!raw_list) return NULL;
    char *copia = strdup(raw_list);
    char *cursor = copia;
    
    // Avança '[' se existir
    if (*cursor == '[') cursor++;
    
    char *token = strsep(&cursor, ",");
    char *resultado = NULL;

    if (token) {
        // Limpar aspas e espaços
        while (*token && (*token == '\'' || *token == '"' || *token == ' ')) token++;
        size_t len = strlen(token);
        while (len > 0 && (token[len-1] == '\'' || token[len-1] == '"' || token[len-1] == ' ' || token[len-1] == ']')) {
            token[len-1] = '\0';
            len--;
        }
        if (len > 0) resultado = strdup(token);
    }
    free(copia);
    return resultado;
}

// Verifica se uma semana (definida a começar em domingo_str) está dentro de um intervalo de datas
static int semana_interseta_datas(const char *domingo_str, const char *inicio, const char *fim) {
    // Se não há filtro, interseta sempre
    if (!inicio && !fim) return 1;

    struct tm tm = {0};
    int y, m, d;

    // Parse do Domingo da semana
    if (sscanf(domingo_str, "%d-%d-%d", &y, &m, &d) != 3) return 0;
    tm.tm_year = y - 1900; tm.tm_mon = m - 1; tm.tm_mday = d; tm.tm_isdst = -1;
    time_t t_start = mktime(&tm); // Início da semana (Domingo)
    time_t t_end = t_start + (6 * 24 * 3600); // Fim da semana (Sábado)

    // Parse do Inicio do filtro
    time_t filtro_inicio = 0;
    if (inicio) {
        memset(&tm, 0, sizeof(struct tm));
        if (sscanf(inicio, "%d-%d-%d", &y, &m, &d) == 3) {
            tm.tm_year = y - 1900; tm.tm_mon = m - 1; tm.tm_mday = d; tm.tm_isdst = -1;
            filtro_inicio = mktime(&tm);
        }
    }

    // Parse do Fim do filtro
    time_t filtro_fim = 0;
    if (fim) {
        memset(&tm, 0, sizeof(struct tm));
        if (sscanf(fim, "%d-%d-%d", &y, &m, &d) == 3) {
            tm.tm_year = y - 1900; tm.tm_mon = m - 1; tm.tm_mday = d; tm.tm_isdst = -1;
            filtro_fim = mktime(&tm);
        }
    }

    // Lógica de Interseção:
    // A semana é válida se: (FimSemana >= FiltroInicio) E (InicioSemana <= FiltroFim)
    
    if (inicio && t_end < filtro_inicio) return 0; // Semana acaba antes do filtro começar
    if (fim && t_start > filtro_fim) return 0;     // Semana começa depois do filtro acabar

    return 1; // Há sobreposição
}


// =========================================================
// INIT (PRÉ-CÁLCULO) - Executado 1 vez
// =========================================================
void init_query4(GestorReservas *gr, GestorVoos *gv, GestorPassageiros *gp) {
    if (cache_semanas) return;

    // 1. Tabela Hash: DataDomingo -> (Tabela Hash: UserID -> GastoTotal)
    GHashTable *semanas_hash = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)g_hash_table_destroy);

    GHashTable *tabela_reservas = get_tabela_reservas(gr);
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, tabela_reservas);

    // 2. Acumular gastos por semana
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Reservas *r = (Reservas *)value;
        
        // Obter voo
        char *voo_id = extrair_primeiro_voo(get_flight_ids(r));
        if (!voo_id) continue;

        Voos *v = get_voo(gv, voo_id);
        free(voo_id); // Já não precisamos do ID, só da data do voo
        if (!v) continue;

        char *domingo = get_sunday_of_week(get_departure(v));
        if (!domingo) continue;

        // Procurar ou criar tabela da semana
        GHashTable *gastos_semana = g_hash_table_lookup(semanas_hash, domingo);
        if (!gastos_semana) {
            gastos_semana = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free);
            g_hash_table_insert(semanas_hash, g_strdup(domingo), gastos_semana);
        }
        
        // Somar preço ao passageiro
        const char *pid_raw = get_document_number(r);
        // Validar se passageiro existe 
        if (!get_passageiro(gp, pid_raw)) { free(domingo); continue; }

        double preco = get_price(r);
        double *total = g_hash_table_lookup(gastos_semana, pid_raw);
        
        if (total) {
            *total += preco;
        } else {
            double *novo = malloc(sizeof(double));
            *novo = preco;
            g_hash_table_insert(gastos_semana, g_strdup(pid_raw), novo);
        }
        free(domingo);
    }

    // 3. Processar Top 10 para Cache
    total_semanas_cache = g_hash_table_size(semanas_hash);
    if (total_semanas_cache > 0) {
        cache_semanas = malloc(sizeof(SemanaCache) * total_semanas_cache);
        
        GHashTableIter iter_sem;
        gpointer data_dom, table_gastos;
        g_hash_table_iter_init(&iter_sem, semanas_hash);
        
        int idx = 0;
        while (g_hash_table_iter_next(&iter_sem, &data_dom, &table_gastos)) {
            cache_semanas[idx].data_domingo = g_strdup((char*)data_dom);
            
            // Converter Hash em Lista para ordenar
            GList *lista = NULL;
            GHashTableIter iter_g;
            gpointer p_id, p_custo;
            g_hash_table_iter_init(&iter_g, (GHashTable*)table_gastos);
            
            while (g_hash_table_iter_next(&iter_g, &p_id, &p_custo)) {
                PassageiroGasto *pg = malloc(sizeof(PassageiroGasto));
                pg->passageiro_id = (char*)p_id;
                pg->gasto_total = *(double*)p_custo;
                lista = g_list_prepend(lista, pg);
            }
            
            // Ordenar
            lista = g_list_sort(lista, compare_gastos);
            
            // Guardar Top 10
            int k = 0;
            for (GList *l = lista; l != NULL && k < 10; l = l->next) {
                PassageiroGasto *pg = (PassageiroGasto *)l->data;
                cache_semanas[idx].top10_ids[k] = g_strdup(pg->passageiro_id);
                k++;
            }
            cache_semanas[idx].count = k;
            
            g_list_free_full(lista, free);
            idx++;
        }

        // 4. Ordenar Array Cache por Data 
        qsort(cache_semanas, total_semanas_cache, sizeof(SemanaCache), compare_semanas_cache);
    }

    g_hash_table_destroy(semanas_hash);
}

// =========================================================
// QUERY 4 
// =========================================================

void query4(char *data_inicio, char *data_fim, char separador, GestorPassageiros *gp, FILE *output) {
    if (!cache_semanas) {
    }

    GHashTable *contador = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free);

    for (int i = 0; i < total_semanas_cache; i++) {
        char *data_sem = cache_semanas[i].data_domingo;

        // Verifica se a semana se cruza com o filtro
        if (!semana_interseta_datas(data_sem, data_inicio, data_fim)) {
            continue; 
        }

        for (int j = 0; j < cache_semanas[i].count; j++) {
            char *pid = cache_semanas[i].top10_ids[j];
            int *cont = g_hash_table_lookup(contador, pid);
            if (cont) (*cont)++;
            else {
                int *novo = malloc(sizeof(int));
                *novo = 1;
                g_hash_table_insert(contador, pid, novo);
            }
        }
    }

    char *vencedor_id = NULL;
    int max_freq = -1;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, contador);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char *pid = (char*)key;
        int freq = *(int*)value;
        if (freq > max_freq) {
            max_freq = freq;
            vencedor_id = pid;
        } else if (freq == max_freq) {
                    if (vencedor_id == NULL || strcmp(pid, vencedor_id) < 0) vencedor_id = pid;
            }
        
    }

    if (vencedor_id) {
        Passageiros *p = get_passageiro(gp, vencedor_id);
        if (p) {
            const char *nome = get_first_name_passageiro(p);
            const char *apelido = get_last_name_passageiro(p);
            const char *dob = get_birth_date_passageiro(p);
            const char *nac = get_nationality_passageiro(p);
            fprintf(output, "%s%c%s%c%s%c%s%c%s%c%d\n", 
                vencedor_id, separador, 
                nome ? nome : "", separador, 
                apelido ? apelido : "", separador, 
                dob ? dob : "", separador,
                nac ? nac : "", separador,
                max_freq);
        }
    } else {
        fprintf(output, "\n");
    }

    g_hash_table_destroy(contador);

}

