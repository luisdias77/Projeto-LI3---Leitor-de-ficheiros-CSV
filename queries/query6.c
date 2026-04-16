#define _GNU_SOURCE 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "queries/query6.h"
#include "entidades/reservas.h"
#include "entidades/voos.h"
#include "entidades/passageiros.h"
#include "gestor_entidades/gestor_reservas.h"
#include "utils/utils.h"

// --------------------------
// ESTRUTURAS E VARIÁVEIS
// -------------------------

// Estrutura para guardar o resultado final processado para cada nacionalidade
typedef struct {
    char *codigo_aeroporto;
    int numero_viagens;
} ResultadoQuery6;

// Tabela Hash Global (Cache) que vai guardar a resposta final.
// Chave: Nacionalidade (string) -> Valor: ResultadoQuery6 (struct com o vencedor)
static GHashTable *TabelaResultadosPorNacionalidade = NULL;

// -------------------------------
// FUNÇÕES AUXILIARES DE LIMPEZA
// -------------------------------

// Função chamada no final do programa para limpar a memória da cache
void destroy_query6() {
    if (TabelaResultadosPorNacionalidade) {
        g_hash_table_destroy(TabelaResultadosPorNacionalidade);
        TabelaResultadosPorNacionalidade = NULL;
    }
}

// Função auxiliar para libertar a estrutura de resultado (usada pelo g_hash_table_new_full)
static void libertar_resultado_query6(gpointer data) {
    ResultadoQuery6 *resultado = (ResultadoQuery6*)data;
    if (resultado) {
        free(resultado->codigo_aeroporto);
        free(resultado);
    }
}

// ----------------------------
// LÓGICA DE PRÉ-PROCESSAMENTO 
// ----------------------------
// Preenche a tabela 'TabelaResultadosPorNacionalidade' 
void init_query6(GestorReservas *gestor_reservas, GestorVoos *gestor_voos, GestorPassageiros *gestor_passageiros) {
    
    // Se a tabela já estiver preenchida, não preciso de fazer nada
    if (TabelaResultadosPorNacionalidade != NULL) return; 

    // 1. Inicializo a Tabela Hash Final (Cache)
    TabelaResultadosPorNacionalidade = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertar_resultado_query6);

    // 2. Crio uma Tabela Temporária para fazer as contagens.
    // Estrutura: Chave=Nacionalidade -> Valor=TabelaHash(Aeroporto -> Contagem)
    GHashTable *TabelaAuxiliarContagem = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)g_hash_table_destroy);

    GHashTableIter iterador_reservas;
    gpointer chave_reserva, valor_reserva;
    
    // Inicializo o iterador para percorrer todas as reservas do sistema
    g_hash_table_iter_init(&iterador_reservas, get_tabela_reservas(gestor_reservas));

    // Fase 1: Percorro todas as reservas para fazer as contagens
    while (g_hash_table_iter_next(&iterador_reservas, &chave_reserva, &valor_reserva)) {
        Reservas* reserva_atual = (Reservas*)valor_reserva;
        
        const char* id_passageiro = get_document_number(reserva_atual);
        Passageiros* passageiro = get_passageiro(gestor_passageiros, id_passageiro);

        // Só processo se o passageiro existir
        if (passageiro) {
            const char* nacionalidade = get_nationality_passageiro(passageiro);
            
            // Em vez de calcular só o destino final, percorremos todos os voos da reserva
            
            char id_voo_buffer[20];
            int indice_voo = 0;
            const char* lista_ids_voos = get_flight_ids(reserva_atual);

            while (extrair_id_voo(lista_ids_voos, indice_voo, id_voo_buffer)) {
                
                Voos* voo_atual = get_voo(gestor_voos, id_voo_buffer);
                
                // Validações do voo
                if (voo_atual && strcmp(get_status(voo_atual), "Cancelled") != 0) {
                    
                    // Contamos o destino deste voo (seja escala ou final)
                    const char* destino_atual = get_destination(voo_atual);

                    if (nacionalidade && destino_atual) {
                        
                        // Obter/Criar tabela da nacionalidade
                        GHashTable *TabelaAeroportosDaNacionalidade = g_hash_table_lookup(TabelaAuxiliarContagem, nacionalidade);
                        if (TabelaAeroportosDaNacionalidade == NULL) {
                            TabelaAeroportosDaNacionalidade = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
                            g_hash_table_insert(TabelaAuxiliarContagem, g_strdup(nacionalidade), TabelaAeroportosDaNacionalidade);
                        }

                        // Incremento a contagem
                        int contagem_atual = GPOINTER_TO_INT(g_hash_table_lookup(TabelaAeroportosDaNacionalidade, destino_atual));
                        contagem_atual++;
                        g_hash_table_insert(TabelaAeroportosDaNacionalidade, g_strdup(destino_atual), GINT_TO_POINTER(contagem_atual));
                    }
                }
                indice_voo++;
            }
        }
    }

    // Fase 2: Determinar o vencedor para cada nacionalidade
    // Agora que tenho as contagens, percorro a tabela auxiliar para encontrar o máximo de cada nacionalidade
    
    GHashTableIter iterador_nacionalidades;
    gpointer chave_nacionalidade, valor_tabela_aeroportos;
    g_hash_table_iter_init(&iterador_nacionalidades, TabelaAuxiliarContagem);

    while (g_hash_table_iter_next(&iterador_nacionalidades, &chave_nacionalidade, &valor_tabela_aeroportos)) {
        char *nacionalidade_atual = (char *)chave_nacionalidade;
        GHashTable *TabelaAeroportos = (GHashTable *)valor_tabela_aeroportos;

        char *aeroporto_vencedor = NULL;
        int maximo_viagens = 0;

        GHashTableIter iterador_aeroportos;
        gpointer chave_aeroporto, valor_contagem;
        g_hash_table_iter_init(&iterador_aeroportos, TabelaAeroportos);

        // Encontro o aeroporto com mais viagens nesta nacionalidade
        while (g_hash_table_iter_next(&iterador_aeroportos, &chave_aeroporto, &valor_contagem)) {
            char *aeroporto_em_analise = (char *)chave_aeroporto;
            int contagem_em_analise = GPOINTER_TO_INT(valor_contagem);

            if (contagem_em_analise > maximo_viagens) {
                maximo_viagens = contagem_em_analise;
                aeroporto_vencedor = aeroporto_em_analise;
            } 
            else if (contagem_em_analise == maximo_viagens) {
                // Critério de desempate: Ordem alfabética do código do aeroporto
                if (aeroporto_vencedor == NULL || strcmp(aeroporto_em_analise, aeroporto_vencedor) < 0) {
                    aeroporto_vencedor = aeroporto_em_analise;
                }
            }
        }

        // Guardo o resultado final na Cache Global
        if (aeroporto_vencedor) {
            ResultadoQuery6 *resultado_final = malloc(sizeof(ResultadoQuery6));
            resultado_final->codigo_aeroporto = g_strdup(aeroporto_vencedor);
            resultado_final->numero_viagens = maximo_viagens;
            
            g_hash_table_insert(TabelaResultadosPorNacionalidade, g_strdup(nacionalidade_atual), resultado_final);
        }
    }

    // Liberto a tabela auxiliar pois já extraí a informação necessária para a cache
    g_hash_table_destroy(TabelaAuxiliarContagem);
}


// --------
// QUERY 6
// --------
void query6(char* nacionalidade, GestorReservas* gr, GestorVoos* gv, GestorPassageiros* gp, char separador, FILE* output){

    // Garanto que a inicialização foi feita (caso não tenha sido chamada na main)
    if (TabelaResultadosPorNacionalidade == NULL) {
        init_query6(gr, gv, gp);
    }

    // Consulto diretamente a tabela de resultados 
    ResultadoQuery6 *melhor_destino = g_hash_table_lookup(TabelaResultadosPorNacionalidade, nacionalidade);

    if (melhor_destino) {
        fprintf(output, "%s%c%d\n", melhor_destino->codigo_aeroporto, separador, melhor_destino->numero_viagens);
    } else {
        fprintf(output, "\n");
    }
}