#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_aeronaves.h" 
#include <stdlib.h>
#include <string.h>

/*
 * ===================================================================
 * ===================  FUNÇÕES DO GESTOR ============================
 * ===================================================================
 */

// Definição do gestor de voos
struct gestor_voos {
    GHashTable* tabela_voos;  // chave = id do voo, valor = Voos*
};

// Criação e destruição do gestor de voos
GestorVoos* criar_gestor_voos() {
    GestorVoos* g = malloc(sizeof(GestorVoos));
    g->tabela_voos = g_hash_table_new_full(g_str_hash, 
                                               g_str_equal,
                                               g_free, 
                                               (GDestroyNotify) libertar_voo
                                               );
    return g;
}   

// Função para destruir o gestor de voos
void destruir_gestor_voos(GestorVoos* g) {
    if (g == NULL) return;
    g_hash_table_destroy(g->tabela_voos);
    free(g);
}   

// Função para adicionar um voo
void adiciona_voos(GestorVoos* g, Voos* a) {

    if (g == NULL || a == NULL) return;
    
    // Obtem o id do voo (chave)
    char* id_voo = g_strdup(get_flight_id(a)); // usa o getter para obter o id do voo
    if (id_voo==NULL) return;
    
    // Adiciona o voo à tabela 
    g_hash_table_replace(g->tabela_voos,id_voo,a);

}


// Função para verificar se um voo existe, através do id_voo 
bool voo_existe(GestorVoos* g, char* id_voo) {

    if (g == NULL || id_voo == NULL) {
        return false;
    }

    if (g_hash_table_contains(g->tabela_voos, id_voo)) {
        return true;
    } else {
        return false;
    }

}


// Função que remove um voo
void remove_voo(GestorVoos* g, char* id_voo) {

    if (g == NULL || id_voo == NULL) {
        return;
    }

    g_hash_table_remove(g->tabela_voos, id_voo);
   

}


// Função que conta o numero de voos na hashtable 
int conta_voos(GestorVoos* g) {
    if (g == NULL) { 
        return 0; 
        }
        else {
            return g_hash_table_size(g->tabela_voos);
        }
    
}



/*
===================================================================
======================== VALIDAÇÃO LÓGICA =========================
===================================================================
*/

// Função que valida logicamente um voo 
int valida_voo_logica(Voos* v, GestorAeronaves* g) {
    if (v == NULL || g == NULL) {
        return 0; // Caso o voo ou o gestor aeronaves sejam NULL, retorna 0 
    }

    // Verifica se o campo destination de um voo é diferente do campo origin
    if (strcmp(get_destination(v), get_origin(v)) == 0){
        return 0; // Inválido, pois o campo destination é igual ao campo origin 
    }

    // Verifica se os campos arrival é anterior ao respetivo campo departure
    // Como formato de campos com data e hora segue o esquema aaaa−mm−dd hh : mm, podemos usar a função strcmp para comparar os campos do voo acima referidos
    if (strcmp (get_arrival(v), get_departure(v)) < 0) {
        return 0; // Inválido, pois a hora de chegada é anterior à hora de partida 
    }

    // Verifica se o campo aircraft de um voo corresponde a uma aeronave existente no gestor 
    if (aeronave_existe(g, get_aircraft(v)) == false){
        return 0; // Inválido, pois a aeronave correspondente ao campo aircraft do voo nao existe no gestor 
    }

    // Caso o campo status de um voo tome o valor Cancelled, os campos actual departure e actual arrival deverão conter o valor "N/A"
    if (strcmp(get_status(v), "Cancelled") == 0) {
        if (strcmp(get_actual_departure(v), "N/A") != 0 || strcmp(get_actual_arrival(v), "N/A") != 0) {
            return 0; // Inválido: Voo cancelado, mas as datas não são "N/A"
        }
    } else {
        // Se nao for "Cancelled", 'actual_arrival' >= 'actual_departure'
        if (strcmp (get_actual_arrival(v), get_actual_departure(v)) < 0) {
            return 0; // Inválido: Chegada real antes da partida real
        }
    }

    // Caso o campo status de um voo tome o valor Delayed, os campos actual departure e actual arrival não poderão ser anteriores aos respetivos campos departure e arrival
    if (strcmp (get_status(v), "Delayed") == 0) { // Se o campo status de um voo tiver o valor Delayed
        if (strcmp(get_actual_departure(v), get_departure(v)) < 0 || strcmp(get_actual_arrival(v), get_arrival(v)) < 0) {
            return 0; // Inválido, pois os campos actual departure e actual arrival são anteriores a departure e arrrival 
        }
    }

    return 1;

}

/*
 * ===================================================================
 * ===================  GETTERS  =====================================
 * ===================================================================
 */

// Getter para aceder à tabela de voos
GHashTable* get_tabela_voos(GestorVoos* g) {
    if (g == NULL) {
        return NULL;
    }
    return g->tabela_voos;
}


// Getters para o gestor de voos , para aceder a um voo com um determinado id_voo ( identificador do voo)

Voos* get_voo(GestorVoos* g, const char* id_voo) {
    if (g == NULL || id_voo == NULL) {
        return NULL;
    }
    return g_hash_table_lookup(g->tabela_voos, id_voo);
}