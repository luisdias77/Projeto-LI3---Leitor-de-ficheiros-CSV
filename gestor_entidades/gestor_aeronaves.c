#include "gestor_entidades/gestor_aeronaves.h"
#include <stdlib.h>
#include <string.h>


// Definição do gestor de aeronaves
struct gestor_aeronaves {
    GHashTable* tabela_aeronaves;  // chave = id da aeronave, valor = Aeronaves*
};

/*
// Criação e destruição do gestor de aeronaves
Consultando a biblioteca GLib, usamos g_hash_table_new_full para criar a tabela de hash. Esta função é definida como:
GHashTable*
g_hash_table_new_full (
  GHashFunc hash_func,
  GEqualFunc key_equal_func,
  GDestroyNotify key_destroy_func,
  GDestroyNotify value_destroy_func
)
pelo que para o primeiro parâmetro usamos g_str_hash (função de hash para strings) uma vez que a chave (ID da aeronave) é do tipo char*. 
Para o segundo usamos g_str_equal (função de comparação para strings), que vai comparar se duas chaves são iguais, isto é se dois IDs de aeronaves são iguais.
Para o terceiro usamos free (função de libertação de memória padrão), que vai libertar a chave quando a hashtable for destruída. 
Para o quarto usamos a função libertar_aeronave definida em aeronaves.c, que vai libertar o valor, ou seja, cada Aeronaves* armazenada na tabela de hash.

*/

GestorAeronaves* criar_gestor_aeronaves() {
    GestorAeronaves* g = malloc(sizeof(GestorAeronaves));
    g->tabela_aeronaves = g_hash_table_new_full(g_str_hash, 
                                                g_str_equal,
                                                g_free, 
                                                (GDestroyNotify) libertar_aeronave
                                                );
    return g;
}

// Função para destruir o gestor de aeronaves
void destruir_gestor_aeronaves(GestorAeronaves* g) {
    if (g == NULL) return;
    g_hash_table_destroy(g->tabela_aeronaves);
    free(g);
}


// Função para adicionar uma aeronave
void adiciona_aeronave(GestorAeronaves* g, Aeronaves* a) {

    if (g == NULL || a == NULL) return;
    
    // Obtem o id da aeronave
    char* id_aeronave = g_strdup(get_identifier_aeronave(a)); // usa o getter para obter o id da aeronave
    if (id_aeronave == NULL) return;
    
    // Adiciona a aeronaves à tabela 
    g_hash_table_replace(g->tabela_aeronaves,id_aeronave,a);

}

// Função para verificar se uma aeronave existe, através do id da aeronave
bool aeronave_existe(GestorAeronaves* g, const char* id_aeronave) {

    if (g == NULL || id_aeronave == NULL) {
        return false;
    }

    if (g_hash_table_contains(g->tabela_aeronaves, id_aeronave)) {
        return true;
    } else {
        return false;
    }

}


// Função que remove uma aeronave
void remove_aeronave(GestorAeronaves* g, char* id_aeronave) {

    if (g == NULL || id_aeronave == NULL) {
        return;
    }

    g_hash_table_remove(g->tabela_aeronaves, id_aeronave);
        

}


// Função que conta o numero de aeronaves na hashtable 
int conta_aeronaves(GestorAeronaves* g) {
    if (g == NULL) { 
        return 0; 
        }
        else {
            return g_hash_table_size(g->tabela_aeronaves);
        }
    
}

/*
 * ===================================================================
 * ===================  GETTERS  =====================================
 * ===================================================================
 */

// Getter para aceder à tabela de aeronaves
GHashTable* get_tabela_aeronaves(GestorAeronaves* g) {
    if (g == NULL) {
        return NULL;
    }
    return g->tabela_aeronaves;
}

// Getters para o gestor de aeronaves , para aceder a uma aeronave com um determinado id ( identificador da aeronave)
Aeronaves* get_aeronave(GestorAeronaves* g, const char* id) {
    if (g == NULL || id == NULL) {
        return NULL;
    }
    return g_hash_table_lookup(g->tabela_aeronaves, id);
}