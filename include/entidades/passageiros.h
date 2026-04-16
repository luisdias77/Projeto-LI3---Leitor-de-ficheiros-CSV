/** 
 * @file passageiros.h
 * @brief Ficheiro de cabeçalho para as funções relacionadas com Passageiros.
 *
 * Este ficheiro contém as declarações das funções utilizadas para criar, libertar
 * e aceder aos campos da estrutura passageiro. A estrutura Passageiros
 * representa um passageiro, com os campos de número de documento (doc_number), primeiro nome (fst_name),
 * último nome (lst_name), data de nascimento (dob), nacionalidade (nation), género (gen), email (mail),
 * telefone (phn), morada (addr) e identificação fotográfica (photo_id).
 */

#ifndef _PASSAGEIROS_H_
#define _PASSAGEIROS_H_

#include <stdbool.h>


/**
 * @brief Estrutura que representa um passageiro.
 *
 * Esta estrutura contém informações sobre um passageiro, que são ocultadas a quem utiliza este módulo.
 */
typedef struct Passageiros Passageiros;

/**
 * @brief Função que cria um novo passageiro.
 *
 * Esta função aloca memória para uma nova estrutura Passageiros e inicializa os seus campos com os valores fornecidos.
 *
 * @param doc_number    Número de documento do passageiro.
 * @param fst_name      Primeiro nome do passageiro.
 * @param lst_name      Último nome do passageiro.
 * @param dob           Data de nascimento do passageiro.
 * @param nation        Nacionalidade do passageiro.
 * @param gen           Género do passageiro.
 * @param mail          Email do passageiro.
 * @param phn           Telefone do passageiro.
 * @param addr          Morada do passageiro.
 * @param photo_id      Identificação fotográfica do passageiro.
 *
 * @return              Apontador para a nova estrutura Passageiros criada. Caso haja falha na alocação de memória, retorna NULL.
 */
Passageiros* criar_passageiro(const char* doc_number, const char* fst_name, const char* lst_name, const char* dob, const char* nation, const char* gen, const char* mail, const char* phn, const char* addr, const char* photo_id);

/**
 * @brief Função que liberta a memória ocupada por um passageiro.
 *
 * Esta função liberta a memória alocada para a estrutura Passageiros, bem como para os seus campos de string.
 * 
 * @param passageiro  Apontador para a estrutura Passageiros que se pretende libertar.
 */
void libertar_passageiro(Passageiros* passageiro);

/**
 * @brief Getter para o campo doc_number da estrutura Passageiros.
 *
 * Esta função retorna o número de documento (doc_number) do passageiro.
 *
 * @param p Apontador para a estrutura Passageiros.
 * @return String com o número de documento.
 */
const char* get_document_number_passageiro(Passageiros* p);

/**
 * @brief Getter para o campo fst_name da estrutura Passageiros.
 *
 * Esta função retorna o primeiro nome (fst_name) do passageiro.
 *
 * @param p Apontador para a estrutura Passageiros.
 * @return String com o primeiro nome.
 */
const char* get_first_name_passageiro(Passageiros* p);

/**
 * @brief Getter para o campo lst_name da estrutura Passageiros.
 *
 * Esta função retorna o último nome (lst_name) do passageiro.
 *
 * @param p Apontador para a estrutura Passageiros.
 * @return String com o último nome.
 */
const char* get_last_name_passageiro(Passageiros* p);

/**
 * @brief Getter para o campo dob da estrutura Passageiros.
 *
 * Esta função retorna a data de nascimento (dob) do passageiro.
 *
 * @param p Apontador para a estrutura Passageiros.
 * @return String com a data de nascimento.
 */
const char* get_dob_passageiro(Passageiros* p);

/**
 * @brief Getter para o campo nation da estrutura Passageiros.
 *
 * Esta função retorna a nacionalidade (nation) do passageiro.
 *
 * @param p Apontador para a estrutura Passageiros.
 * @return String com a nacionalidade.
 */
const char* get_nationality_passageiro(Passageiros* p);

/**
 * @brief Getter para o campo dob da estrutura Passageiros.
 *
 * Esta função retorna a data de nascimento (dob) do passageiro.
 *
 * @param p Apontador para a estrutura Passageiros.
 * @return String com a data de nascimento.
 */
const char* get_birth_date_passageiro(Passageiros* p);


#endif
