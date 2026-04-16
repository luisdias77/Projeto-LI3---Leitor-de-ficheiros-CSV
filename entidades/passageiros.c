#define _GNU_SOURCE
#include "entidades/passageiros.h"
#include <stdlib.h>          
#include <string.h>

struct Passageiros {
   
    char* document_number;
    char* first_name;
    char* last_name;
    char* dob;
    char* nationality;
    char* gender;
    char* email;
    char* phone;
    char* address;
    char* photo;

};



// Função que cria uma estrutura do tipo passageiros(aloca memória para a estrutura em si e para as strings, isto é, os diferentes parametros da struct passageiros, exceto os parametros int)
Passageiros* criar_passageiro(const char* doc_number, const char* fst_name, const char* lst_name, const char* dob, const char* nation, const char* gen, const char* mail, const char* phn, const char* addr, const char* photo_id) {
    // Aqui é alocada a memória para a struct passageiros
    Passageiros* novo_passageiro = (Passageiros*)malloc(sizeof(Passageiros)); 
    if (novo_passageiro == NULL) {    
        return NULL;     
    }
    
    // O strdup aloca a memória exata e copia a string inteira. 
    novo_passageiro->document_number = strdup(doc_number);
    novo_passageiro->first_name = strdup(fst_name);
    novo_passageiro->last_name = strdup(lst_name);
    novo_passageiro->dob = strdup(dob);
    novo_passageiro->nationality = strdup(nation);
    novo_passageiro->gender = strdup(gen);
    novo_passageiro->email = strdup(mail);
    novo_passageiro->phone = strdup(phn);
    novo_passageiro->address = strdup(addr);
    novo_passageiro->photo = strdup(photo_id);

    // Retornar o ponteiro para a estrutura passageiros criada
    return novo_passageiro;
}


// Função que liberta a memória da estrutura Passageiros
void libertar_passageiro(Passageiros* passageiro) {
    if (passageiro != NULL) {
        // 1. Libertar a memória de cada string primeiro
        free(passageiro->document_number);
        free(passageiro->first_name);
        free(passageiro->last_name);
        free(passageiro->dob);
        free(passageiro->nationality);  
        free(passageiro->gender);
        free(passageiro->email);
        free(passageiro->phone);
        free(passageiro->address);
        free(passageiro->photo);
        
        // 2. Libertar a memória da própria estrutura
        free(passageiro);
    }
}

// Getters para os campos da struct Passageiros

const char* get_document_number_passageiro(Passageiros* p) {
    return p->document_number;
}

const char* get_first_name_passageiro(Passageiros* p) {
    return p->first_name;
}

const char* get_last_name_passageiro(Passageiros* p) {
    return p->last_name;
}

const char* get_dob_passageiro(Passageiros* p) {
    return p->dob;
}

const char* get_nationality_passageiro(Passageiros* p) {
    return p->nationality;
}

const char* get_birth_date_passageiro(Passageiros* p) {
    return p->dob;
}

