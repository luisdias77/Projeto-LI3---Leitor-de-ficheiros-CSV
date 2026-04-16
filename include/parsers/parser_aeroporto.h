/** 
 * @file parser_aeroporto.h
 * @brief Ficheiro de cabeçalho para as funções de parsing de Aeroportos.
 * 
 * Este ficheiro contém as declarações das funções utilizadas para fazer o parsing
 * das linhas do ficheiro CSV de aeroportos e carregar os dados para o gestor de aeroportos.
 * 
 */

#ifndef PARSERS_AEROPORTO_H
#define PARSERS_AEROPORTO_H

#include "gestor_entidades/gestor_aeroportos.h"


/**
 * @brief Função que faz o parsing de uma linha do ficheiro CSV de aeroportos.
 *
 * Esta função lê uma linha do ficheiro CSV, valida os dados e cria uma estrutura Aeroportos.
 *
 * @param linha String que contém a linha do ficheiro CSV.
 * @return Apontador para a estrutura Aeroportos criada. Em caso de falha, retorna NULL.
 */
Aeroportos* parser_aeroporto(char* linha);

/**
 * @brief Função que carrega os aeroportos de um ficheiro CSV para o gestor de aeroportos.
 *
 * Esta função lê o ficheiro CSV linha por linha, faz o parsing de cada linha e adiciona
 * os aeroportos que são válidos ao gestor de aeroportos.
 *
 * @param g Apontador para o gestor de aeroportos onde os dados serão carregados.
 * @param filename Nome do ficheiro CSV que vai ser lido.
 * @return Número de aeroportos que foram carregados corretamente. Caso haja erro, retorna -1.
 */
int carregar_aeroportos_csv (GestorAeroportos* g, const char* filename);

#endif 