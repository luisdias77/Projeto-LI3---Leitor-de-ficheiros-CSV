/** 
 * @file parser_voos.h
 * @brief Ficheiro de cabeçalho para as funções de parsing de Voos.
 * 
 * Este ficheiro contém as declarações das funções utilizadas para fazer o parsing
 * das linhas do ficheiro CSV de voos e carregar os dados para o gestor de voos.
 *
 */

#ifndef PARSERS_VOOS_H
#define PARSERS_VOOS_H

#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_aeronaves.h"

/**
 * @brief Função que faz o parsing de uma linha do ficheiro CSV de voos.
 *
 * Esta função lê uma linha do ficheiro CSV, valida os dados e cria uma estrutura Voos.
 *
 * @param linha String que contém a linha do ficheiro CSV.
 * @return Apontador para a estrutura Voos criada. Em caso de falha, retorna NULL.
 */
Voos* parser_voos(char* linha);

/**
 * @brief Função que carrega os voos de um ficheiro CSV para o gestor de voos.
 *
 * Esta função lê o ficheiro CSV linha por linha, faz o parsing de cada linha e adiciona
 * os voos que são válidos ao gestor de voos.
 *
 * @param gv Apontador para o gestor de voos onde os dados serão carregados.
 * @param ga Apontador para o gestor de aeronaves, que é necessário para fazer a validação das aeronaves associadas aos voos.
 * @param filename Nome do ficheiro CSV que vai ser lido.
 * @return Número de voos que foram carregados corretamente. Caso haja erro, retorna -1.
 */
int carregar_voos_csv (GestorVoos* gv, GestorAeronaves* ga, char* filename);

#endif 