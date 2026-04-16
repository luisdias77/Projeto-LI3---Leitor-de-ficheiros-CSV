/**
 * @file aeronaves.h
 * @brief Ficheiro de cabeçalho para as funções relacionadas com Aeronaves.
 *
 * Este ficheiro contém as declarações das funções utilizadas para criar, 
 * libertar e aceder aos campos da estrutura aeronave. A estrutura Aeronaves
 * representa uma aeronave, com os campos de indentificador (identifier), fabricante (manufacturer), modelo (model), ,
 * ano de fabrico (year), capacidade (capacity) e alcance (range). 
 */

#ifndef _AEORONAVES_H_
#define _AEORONAVES_H_

#include <stdbool.h>

/**
 * @brief Estrutura que representa uma aeronave.
 *
 * Esta estrutura contém informações sobre uma aeronave, que são ocultadas a quem utiliza este módulo.
 */

typedef struct Aeronaves Aeronaves;

/**
 * @brief Função que cria uma nova aeronave.
 *
 * Esta função aloca memória para uma nova estrutura Aeronaves e vai inicializar os seus campos com os valores fornecidos.
 *
 * @param id        Identificador único da aeronave.
 * @param manuf     Fabricante da aeronave.
 * @param model     Modelo da aeronave.
 * @param year      Ano de fabricação da aeronave.
 * @param cap       Capacidade máxima de passageiros da aeronave.
 * @param range     Alcance máximo da aeronave em km.
 *
 * @return          Apontador para a nova estrutura Aeronaves criada. Caso haja falha na alocação de memória, retorna NULL.
 */

Aeronaves* criar_aeronave(const char* id, const char* manuf, const char* model, int year, int cap, int range);

/**
 * @brief Função que liberta a memória ocupada por uma aeronave.
 *
 * Esta função liberta a memória alocada para a estrutura Aeronaves, bem como para os seus campos de string.
 * 
 * @param aeronave  Apontador para a estrutura Aeronaves que se pretende libertar.
 */

void libertar_aeronave(Aeronaves* aeronave);

/**
 * @brief Getter para o campo identifier da estrutura Aeronaves.
 *
 * Esta função retorna o identificador (identifier) da aeronave.
 *
 * @param a Apontador para a estrutura Aeronaves.
 * @return String com o identificador.
 */

const char* get_identifier_aeronave(Aeronaves* a);

/**
 * @brief Getter para o campo manufacturer da estrutura Aeronaves.
 *
 * Esta função retorna o fabricante (manufacturer) da aeronave.
 *
 * @param a Apontador para a estrutura Aeronaves.
 * @return String com o fabricante.
 */

const char* get_manufacturer_aeronave(Aeronaves* a);

/**
 * @brief Getter para o campo model da estrutura Aeronaves.
 *
 * Esta função retorna o modelo (model) da aeronave.
 *
 * @param a Apontador para a estrutura Aeronaves.
 * @return String com o modelo.
 */

const char* get_model_aeronave(Aeronaves* a);

/**
 * @brief Getter para o campo year da estrutura Aeronaves.
 *
 * Esta função retorna o ano de fabricação (year) da aeronave.
 *
 * @param a Apontador para a estrutura Aeronaves.
 * @return Inteiro com o ano de fabricação.
 */

int get_year_aeronave(Aeronaves* a);

/**
 * @brief Getter para o campo capacity da estrutura Aeronaves.
 *
 * Esta função retorna a capacidade máxima de passageiros (capacity) da aeronave.
 *
 * @param a Apontador para a estrutura Aeronaves.
 * @return Inteiro com a capacidade máxima de passageiros.
 */
int get_capacity_aeronave(Aeronaves* a);

/**
 * @brief Getter para o campo range da estrutura Aeronaves.
 *
 * Esta função retorna o alcance máximo (range) da aeronave em km.
 *
 * @param a Apontador para a estrutura Aeronaves.
 * @return Inteiro com o alcance máximo em km.
 */
 
int get_range_aeronave(Aeronaves* a);


#endif



