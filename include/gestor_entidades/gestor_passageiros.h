/**
 * @file gestor_passageiros.h
 * @brief Ficheiro de cabeçalho para as funções associadas ao gestor de passageiros.
 * 
 * Este ficheiro contém as declarações das funções utilizadas para criar, destruir,
 * adicionar, remover e consultar aeroportos no gestor de passageiros, bem como os getters usados para aceder aos passageiros armazenados,
 * ou a campos específicos dos mesmos.
 */

#ifndef _GESTOR_PASSAGEIROS_H_
#define _GESTOR_PASSAGEIROS_H_

#include <glib.h>
#include "entidades/passageiros.h"  
#include <stdbool.h>

/**
 * @brief Estrutura que representa o gestor de passageiros.
 * 
 * Esta estrutura contém uma tabela hash que armazena os passageiros,
 * onde a chave é o id_document do passageiro (char*) e o valor é um apontador para a estrutura Passageiros*.
 */

typedef struct gestor_passageiros GestorPassageiros;

/**
 * @brief Função para criar o gestor de passageiros.
 * 
 * Esta função aloca memória para uma nova estrutura GestorPassageiros
 * inicializando a tabela hash que irá armazenar os passageiros.
 * 
 * @return Apontador para a nova estrutura GestorPassageiros criada.
 */

GestorPassageiros* criar_gestor_passageiros();

/**
 * @brief Função para destruir o gestor de passageiros.
 * 
 * Esta função liberta a memória ocupada pela estrutura GestorPassageiros,
 * incluindo a tabela hash e todos os passageiros que estão armazenados nela.
 * 
 * @param g Apontador para a estrutura GestorPassageiros que vai destruída.
 */

void destruir_gestor_passageiros(GestorPassageiros* g);

/**
 * @brief Função para adicionar um passageiro ao gestor de passageiros.
 * 
 * Esta função insere um novo passageiro na tabela hash do gestor,
 * usando o id_document do passageiro como chave.
 * 
 * @param g Apontador para a estrutura GestorPassageiros onde será adiconada o respetivo passageiro.
 * @param a Apontador para a estrutura Passageiros que será adicionada ao gestor.
 */

void adiciona_passageiro(GestorPassageiros* g, Passageiros* a);

/**
 * @brief Função para verificar se um passageiro existe no gestor de passageiros.
 * 
 * Esta função verifica se um passageiro com um id_document fornecido
 * existe na tabela hash do gestor.
 * 
 * @param g Apontador para a estrutura GestorPassageiros onde será irá ser verificado se o passageiro existe.
 * @param id_document String que representa o id_document do passageiro a ser verificado.
 * @return true se o passageiro existir, ou false, no caso de o passageiro não existir.
 */

bool passageiros_existe(GestorPassageiros* g, char* id_document);

/**
 * @brief Função que remove um passageiro do gestor de passageiros.
 *
 * Esta função remove um passageiro da tabela hash do gestor,
 * usando o id_document do passageiro como chave.
 *
 * @param g Apontador para a estrutura GestorPassageiros onde será removido o respetivo passageiro.
 * @param codigo String que representa o id_document do passageiro a ser removido.
 */

void remove_passageiro(GestorPassageiros* g, char* id_document);

/**
 * @brief Função que conta o numero de passageiros na hashtable.
 * 
 * Esta função percorre a tabela hash do gestor de passageiros
 * e conta o número total de passageiros armazenados.
 * 
 * @param g Apontador para a estrutura GestorPassageiros onde será contado o número de passageiros.
 * @return Inteiro representando o número total de passageiros na tabela hash.
 */

int conta_passageiros(GestorPassageiros* g);

/**
 * @brief Função que valida o genéro de um passageiro.
 *
 * Esta função verifica se o género fornecido é válido
 * com base nos valores permitidos: "M", "F" ou "O".
 *
 * @param gender String que representa o género a ser validado.
 * @return Inteiro que indica se o género é válido (1) ou inválido (0).
 */

int validar_genero(const char* gender);

/**
 * @brief Função que valida o email de um passageiro.
 *
 * Esta função verifica se o email fornecido está em um formato válido,
 * seguindo as regras definidas no enunciado.
 *
 * @param email String que representa o email a ser validado.
 * @return Inteiro que indica se o email é válido (1) ou inválido (0).
 */

int validar_email(char* email);

/**
 * @brief Getter para o gestor de passageiros , para aceder a um passageiro com um determinado id_document (identificador do document number do passageiro).
 *
 * Esta função retorna um apontador para a estrutura Passageiro
 * que corresponde ao id_document fornecido.
 *
 * @param g Apontador para a estrutura GestorPassageiros onde será feita a procura do passageiro.
 * @param code String que representa o id_document do passageiro a ser procurado.
 *
 * @return Apontador para a estrutura Passageiros associada ao id_document fornecido.
 */

Passageiros* get_passageiro(GestorPassageiros* g, const char* id_document);

/**
 * @brief Getter para aceder a tabela de passageiros.
 *
 * Esta função retorna a tabela hash que armazena os passageiros
 * no gestor de passageiros.
 *
 * @param g Apontador para a estrutura GestorPassageiros.
 *
 * @return Apontador para a tabela hash de passageiros.
 */

GHashTable* get_tabela_passageiros(GestorPassageiros* g);

#endif