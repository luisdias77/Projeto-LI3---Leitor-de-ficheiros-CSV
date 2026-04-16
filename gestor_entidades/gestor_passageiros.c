#define _GNU_SOURCE
#include "gestor_entidades/gestor_passageiros.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <string.h> 
#include <stdbool.h>
#include <stdio.h>

/*
 * ===================================================================
 * ===================  FUNÇÕES DO GESTOR ============================
 * ===================================================================
 */

// Definição do gestor de passageiros
struct gestor_passageiros {
    GHashTable* tabela_passageiros;  // chave = id do document number do passageiro, valor = Passageiros*
};

// Criação e destruição do gestor de passageiros
GestorPassageiros* criar_gestor_passageiros() {
    GestorPassageiros* g = malloc(sizeof(GestorPassageiros));
    
    g->tabela_passageiros = g_hash_table_new_full(g_str_hash, 
                                                  g_str_equal,
                                                  free, 
                                                  (GDestroyNotify) libertar_passageiro
                                                 );
    return g;       
}

// Função para destruir o gestor de passageiros
void destruir_gestor_passageiros(GestorPassageiros* g) {
    if (g == NULL) return;
    g_hash_table_destroy(g->tabela_passageiros);
    free(g);
}   

// Função para adicionar um aeroporto
void adiciona_passageiro(GestorPassageiros* g, Passageiros* a) {

    if (g == NULL || a == NULL) return;
    
    // Obtem o id do document number do passageiro
    // Aloca memória para a chave (o id do document number do passageiro) 
    char* id_document = g_strdup (get_document_number_passageiro(a)); // usa o getter para obter o id do document number do passageiro
    if (id_document==NULL) return;
    
    // Adiciona o aeroporto à tabela 
    g_hash_table_replace(g->tabela_passageiros,id_document,a);

}


// Função para verificar se um passageiro existe, através do id_document
bool passageiros_existe(GestorPassageiros* g, char* id_document) {

    if (g == NULL || id_document == NULL) {
        return false;
    }

    if (g_hash_table_contains(g->tabela_passageiros, id_document)) {
        return true;
    } else {
        return false;
    }

}

// Função que remove um passageiro
void remove_passageiro(GestorPassageiros* g, char* id_document) {

    if (g == NULL || id_document == NULL) {
        return;
    }

    g_hash_table_remove(g->tabela_passageiros, id_document);

}


// Função que conta o numero de passageiros na hashtable 
int conta_passageiros(GestorPassageiros* g) {
    if (g == NULL) { 
        return 0; 
        }
        else {
            return g_hash_table_size(g->tabela_passageiros);
        }
    
}


/*
 * ===================================================================
 * ===================== VALIDAÇÃO SINTÁTICA  ========================
 * ===================================================================
 */

// Função para validar o género de um passageiro 
int validar_genero(const char* gender) {
    if (gender == NULL || strlen(gender) != 1) return 0;
    return (*gender == 'M' || *gender == 'F' || *gender == 'O');
}


// Função para validar o email de um passageiro
int validar_email(char* email) {
    if (email == NULL) return 0;

    // 1. Encontrar os separadores, isto é o '@' e o '.'. Isto permite dividir o email em tres partes (o username, lstring e rstring)
    char* simbolo = strchr(email, '@');  // uso stringchr que vai procurar o caractere '@' na string email

    char* ponto = strrchr(email, '.');  // uso strrchr que vai começar a procurar no fim, ou seja o ponto que vai encontrar vai ser o que esta antes de com ou pt, por exemplo.

    // 2. Validar se os separadores existem e estão na posição correta

    // 2.1 O @ tem de existir e nao pode ser o primeiro caractere e só pode haver 1
    if (simbolo==NULL || email[0]=='@' || strchr(simbolo + 1, '@')!=NULL) return 0; // simbolo + 1 irá apontar para o próximo caractere depois do '@'

    // 2.2 O . tem de existir, tem de vir depois do @ e não pode ser logo a seguir ao @ (lstring tem de ter pelo menos 1 caractere)
    if (ponto==NULL || ponto < (simbolo + 2)) return 0;

    // 3. Validar se o username (o que esta antes do @) tem pelo menos um caractere no intervalo [a-z0-9], podendo incluir o caractere ponto
    char *username = email; // Aponta para o início do email

    // Se o ponteiro para o início (username) for igual ao ponteiro para o @ (simbolo), o username está vazio.
    if (username == simbolo) return 0; 

    while (username < simbolo) {
        char c = *username;
        // Verficamos se o caractere é valido
        if ((c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            (c == '.')) {
            username++;
        } else {
            return 0; // Caractere inválido encontrado
        }
    }

    // 4. Validar se a lstring (entre o @ e o .) está no intervalo [a-z]. Já sabememos que tem pelo menos um caractere por 2.2
    char *lstring = simbolo + 1; // Aponta para o caractere após o '@'

    while (lstring < ponto) {
        char c = *lstring;
        if (c >= 'a' && c <= 'z') {
            lstring++;
        } else {
            return 0; // Caractere inválido encontrado
        }
    }

    // 5. Validar se a rstring (após o .) está no intervalo [a-z] e tem pelo menos 2 caracteres e no máximo 3
    char *rstring = ponto + 1; // Aponta para o caractere após o '.'
    int rstring_length = strlen(rstring); // Calcula o comprimento da rstring

    // Se o comprimento da rstring for menor que 2 ou maior que 3, é inválido
    if (rstring_length < 2 || rstring_length > 3) return 0;  

    while (*rstring != '\0') {
        char c = *rstring;
        if (c >= 'a' && c <= 'z') {
            rstring++;
        } else {
            return 0; // Caractere inválido encontrado
        }
    }

    return 1; // Email é válidoo
}

/*
 * ===================================================================
 * ===================  GETTERS  =====================================
 * ===================================================================
 */

// Getter para aceder à tabela de passageiros
GHashTable* get_tabela_passageiros(GestorPassageiros* g) {
    if (g == NULL) {
        return NULL;
    }
    return g->tabela_passageiros;
}


// Getters para o gestor de passageiros , para aceder a um passageiro com um determinado id_document ( identificador do document number do passageiro)
Passageiros* get_passageiro(GestorPassageiros* g, const char* id_document) {
    if (g == NULL || id_document == NULL) {
        return NULL;
    }
    return (Passageiros*) g_hash_table_lookup(g->tabela_passageiros, id_document);
}