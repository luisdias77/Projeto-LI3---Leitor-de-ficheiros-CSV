/**
 * @file gestor_voos.h
 * @brief Ficheiro de cabeçalho para as funções associadas ao gestor de voos.
 * 
 * Este ficheiro contém as declarações das funções utilizadas para criar, destruir,
 * adicionar, remover e consultar voos no gestor de voos, bem como os getters usados para aceder a voos armazenados,
 * ou a campos específicos dos mesmos.
 */

#ifndef GESTOR_VOOS_H
#define GESTOR_VOOS_H

#include <glib.h>
#include "entidades/voos.h"
#include "gestor_entidades/gestor_aeronaves.h"
#include <stdbool.h>

/**
 * @brief Estrutura que representa o gestor de voos.
 * 
 * Esta estrutura contém uma tabela hash que armazena os voos,
 * onde a chave é o id do voo (char*) e o valor é um apontador para a estrutura Voos*.
 */

typedef struct gestor_voos GestorVoos;

/**
 * @brief Função para criar o gestor de voos.
 * 
 * Esta função aloca memória para uma nova estrutura GestorVoos
 * inicializando a tabela hash que irá armazenar os voos.
 * 
 * @return Apontador para a nova estrutura GestorVoos criada.
 */

GestorVoos* criar_gestor_voos();

/**
 * @brief Função para destruir o gestor de voos.
 * 
 * Esta função liberta a memória ocupada pela estrutura GestorVoos,
 * incluindo a tabela hash e todos os voos que estão armazenados nela.
 * 
 * @param g Apontador para a estrutura GestorVoos que vai destruída.
 */

void destruir_gestor_voos(GestorVoos* g);

/**
 * @brief Função para adicionar um voo ao gestor de voos.
 * 
 * Esta função insere um novo voo na tabela hash do gestor,
 * usando o id do voo como chave.
 * 
 * @param g Apontador para a estrutura GestorVoos onde será adiconada o respetivo voo.
 * @param a Apontador para a estrutura Voos que será adicionada ao gestor.
 */

void adiciona_voos(GestorVoos* g, Voos* a);

/**
 * @brief Função para verificar se um voo existe, através do id_voo 
 * 
 * Esta função verifica se um voo com um id_voo fornecido
 * existe na tabela hash do gestor de voos.
 * 
 * @param g Apontador para a estrutura GestorVoos onde será irá ser verificado se o voo existe.
 * @param id_voo String que representa o id_voo do voo a ser verificado.
 * 
 * @return true se o voo existir, ou false, no caso de o voo não existir. 
 */

bool voo_existe(GestorVoos* g, char* id_voo);

/**
 * @brief Função que remove um voo do gestor de voos.
 *
 * Esta função remove um voo da tabela hash do gestor,
 * usando o id_voo do voo como chave.
 *
 * @param g Apontador para a estrutura GestorVoos onde será removido o respetivo voo.
 * @param id_reserva String que representa o id_voo do voo a ser removido.
 */

void remove_voo(GestorVoos* g, char* id_voo);

/** 
 * @brief Função que conta o numero de voos na hashtable
 *
 * Esta função percorre a tabela hash do gestor de voos
 * e conta o número total de voos armazenados.
 *
 * @param g Apontador para a estrutura GestorVoos onde será contado o número de voos.
 *
 * @return Inteiro com o número total de voos.
 */

int conta_voos(GestorVoos* g);

/**
 * @brief Função que valida logicamente o id de um voo
 *
 * Esta função verifica se o id do voo fornecido é válido
 * de acordo com os critérios que são definidos no enunciado.
 *
 * @param id String que representa o id do voo a ser validado.
 *
 * @return Inteiro que indica se o id é válido (1) ou inválido (0).
 */

int valida_voo_logica(Voos* v, GestorAeronaves* g);

/** 
 * @brief Getters para o gestor de voos, para aceder a um voo com um determinado id_voo (identificador do voo)
 *
 * Esta função retorna um apontador para a estrutura Voos
 * que corresponde ao id_voo fornecido.
 *
 * @param g Apontador para a estrutura GestorVoos onde será irá ser procurado o voo.
 * @param id_reserva String que representa o id_voo do voo a ser procurado.
 *
 * @return Apontador para a estrutura Voos correspondente ao id_voo fornecido. Em caso de não existir, retorna NULL.
 */

Voos* get_voo(GestorVoos* g, const char* id);

/**
 * @brief Getter para aceder à tabela de voos
 * 
 * Esta função retorna a tabela hash que armazena os voos
 * no gestor de voos.
 * 
 * @param g Apontador para a estrutura GestorVoos.
 * @return Apontador para a tabela hash de voos.
 */

GHashTable* get_tabela_voos(GestorVoos* g);

#endif