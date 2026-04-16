#include "gestor_entidades/gestor_reservas.h"
#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_passageiros.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * ===================================================================
 * ===================  FUNÇÕES DO GESTOR ============================
 * ===================================================================
 */

// Definição do gestor de reservas
struct gestor_reservas {
    GHashTable* tabela_reservas;  // chave = id da reserva, valor = Reservas*
};
/*
// Definição do gestor de passageiros
struct gestor_passageiros {
    GHashTable* tabela_passageiros;  // chave = id do document number do passageiro, valor = Passageiros*
};

// Definição do gestor de voos
struct gestor_voos {
    GHashTable* tabela_voos;  // chave = id do voo, valor = Voos*
};
*/

// Criação e destruição do gestor de reservas
GestorReservas* criar_gestor_reservas() {
    GestorReservas* g = malloc(sizeof(GestorReservas));
    g->tabela_reservas = g_hash_table_new_full(g_str_hash, 
                                               g_str_equal,
                                               g_free, 
                                               (GDestroyNotify) libertar_reserva
                                               );
    return g;
}   

// Função para destruir o gestor de reservas
void destruir_gestor_reservas(GestorReservas* g) {
    if (g == NULL) return;
    g_hash_table_destroy(g->tabela_reservas);
    free(g);
}   


// Função para adicionar uma reserva
void adiciona_reservas(GestorReservas* g, Reservas* a) {

    if (g == NULL || a == NULL) return;
    
    // Obtem o id da reserva (chave)
    char* id_reserva = g_strdup(get_reservation_id(a)); // usa o getter para obter o id da reserva
    if (id_reserva==NULL) return;
    
    // Adiciona a reserva à tabela 
    g_hash_table_replace(g->tabela_reservas,id_reserva,a);

}


// Função para verificar se uma reserva existe, através do id_reserva 
bool reserva_existe(GestorReservas* g, char* id_reserva) {

    if (g == NULL || id_reserva == NULL) {
        return false;
    }

    if (g_hash_table_contains(g->tabela_reservas, id_reserva)) {
        return true;
    } else {
        return false;
    }

}


// Função que remove uma reserva
void remove_reserva(GestorReservas* g, char* id_reserva) {

    if (g == NULL || id_reserva == NULL) {
        return;
    }

    g_hash_table_remove(g->tabela_reservas, id_reserva);

}

// Função que conta o numero de reservas na hashtable 
int conta_reservas(GestorReservas* g) {
    if (g == NULL) { 
        return 0; 
        }
        else {
            return g_hash_table_size(g->tabela_reservas);
        }
}

/*
 * ===================================================================
 * ===================  FUNÇÕES DE VALIDAÇÃO  ========================
 * ===================================================================
*/

// Função para validar se uma string é o ID de reserva
int validar_reservation_id(const char* id) {
    // Verifica se a string é NULL ou vazia
    if (id == NULL || strlen(id) != 10)  return 0; // ID de reserva deve ter exatamente 10 caracteres (R seguido de 9 dígitos)

    // Verifica se o primeiro caractere é 'R'
    if (id[0] != 'R') return 0; 

    // Verifica se os próximos 9 caracteres são dígitos
    for (int i = 1; i < 10; i++) 
        if (!isdigit(id[i])) return 0; 
    
    return 1;
}

/*
===================================================================
======================== VALIDAÇÃO LÓGICA =========================
===================================================================
*/

// Função que verifica se um passageiro existe, através do document_number
bool passageiro_existe (GestorPassageiros* p, const char* document_number) {
    if (p == NULL || document_number == NULL) {
        return false;
    }
    if (g_hash_table_contains (get_tabela_passageiros(p), (gconstpointer)document_number)) {
        return true;
    } else {
        return false;
    }
    return false;
}

// Função que extrai um id de um voo de uma lista de ids 
int extrair_id_voo(const char* flight_ids, int flight_index, char* id_destino) {
    int id_atual_encontrado = 0; // contador de ids encontrados
    int pos_no_id_destino = 0;  // posição atual dentro do flight_id
    bool dentro_das_plicas = false; // "Flag" para saber se estamos dentro de plicas ('')

    // Percorre cada caractere da string de Ids
    for (int i = 0; flight_ids[i] != '\0'; i++) {
        char c = flight_ids[i];

        if (c == '\'') {
            if (!dentro_das_plicas) { 
                // Encontrámos aspas de abertura
                dentro_das_plicas = true;
                pos_no_id_destino = 0;
                // Se este é o ID que procuramos (ex: o 0-ésimo)
                if (id_atual_encontrado == flight_index) {
                    id_destino[0] = '\0'; // Limpa o destino
                }
            } else { 
                // Encontrámos aspas de fecho
                dentro_das_plicas = false;
                if (id_atual_encontrado == flight_index) {
                    id_destino[pos_no_id_destino] = '\0'; // Fecha a string
                    return 1; // Encontrámos o ID
                }
                id_atual_encontrado++; // Passa para o próximo ID
            }
        } else if (dentro_das_plicas && id_atual_encontrado == flight_index) {
            // Estamos dentro das plicas E este é o ID que queremos
            if (pos_no_id_destino < 15) { // 7 chars + \0
                id_destino[pos_no_id_destino++] = c;
            } else {
                return 0; // ID demasiado longo (ex: 'TI123456')
            }
        }
    }
    return 0; // Formato inválido ou ID não encontrado
}


// Função que valida logicamente uma reserva
int valida_reserva_logica(Reservas* r, GestorVoos* gv, GestorPassageiros* gp) {
    if (r == NULL || gv == NULL || gp == NULL) {
        return 0;
    }

    // 1. Verifica se o passageiro existe
    if (passageiro_existe(gp, get_document_number(r)) == false) {
        return 0; // Passageiro não existe
    } 

    char id_voo1[16]; // Buffer para "ID1" , um id tem a estrutra ccddddd
    char id_voo2[16]; // Buffer para "ID2"
    Voos* primeiro_voo = NULL;
    Voos* segundo_voo = NULL;

    // 2. Extrai e valida o primeiro voo
    if (!extrair_id_voo(get_flight_ids(r), 0, id_voo1)) {
        return 0; // Formato da lista inválido ou sem voos
    }

    // 2.1 Valida formato do primeiro flight ID
    if (!valida_flight_id(id_voo1)) {
        return 0; // Formato inválido
    }
    
    primeiro_voo = g_hash_table_lookup(get_tabela_voos(gv), id_voo1);
    if (primeiro_voo == NULL) {
        return 0; // Primeiro voo não existe no GestorVoos
    }

    // 3. Tenta extrair o segundo voo
    if (extrair_id_voo(get_flight_ids(r), 1, id_voo2)) {
        // 3.1 Valida formato do segundo flight ID
        if (!valida_flight_id(id_voo2)) {
            return 0; // Formato inválido
        }
        // Se existe um segundo voo...
        segundo_voo = g_hash_table_lookup(get_tabela_voos(gv), id_voo2);
        if (segundo_voo == NULL) {
            return 0; // Segundo voo não existe no GestorVoos
        }

        // 4. Regra de validação para 2 voos 
        if (strcmp(get_destination(primeiro_voo), get_origin(segundo_voo)) != 0) {
            return 0; // Inválido (destino != origem)
        }

        // O 1º voo tem de chegar antes do 2º partir.
        const char* chegada_v1 = get_arrival(primeiro_voo);
        const char* partida_v2 = get_departure(segundo_voo);

        if (strcmp(chegada_v1, partida_v2) >= 0) {
            return 0; // Inválido: Voo 2 parte antes do Voo 1 chegar!
        }
    }

    // 5. Garante que não há um terceiro voo
    char id_voo3[16];
    if (extrair_id_voo(get_flight_ids(r), 2, id_voo3)) {
        return 0; // Inválido (Mais de 2 voos)
    }

    return 1; // Válido
}

/*
 * ===================================================================
 * ===================  GETTERS  =====================================
 * ===================================================================
 */

// Getter para aceder à tabela de reservas
GHashTable* get_tabela_reservas(GestorReservas* g) {
    if (g == NULL) {
        return NULL;
    }
    return g->tabela_reservas;
}


// Getters para o gestor de reservas , para aceder a uma reserva com um determinado id_reserva ( identificador da reserva)
Reservas* get_reserva(GestorReservas* g, const char* id_reserva) {
    if (g == NULL || id_reserva == NULL) {
        return NULL;
    }
    return g_hash_table_lookup(g->tabela_reservas, id_reserva);
}



