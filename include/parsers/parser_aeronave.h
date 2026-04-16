/**
 * @file parser_aeronave.h
 * @brief Ficheiro de cabeçalho para as funções de parsing de Aeronaves.
 *
 * Este ficheiro contém as declarações das funções utilizadas para fazer o parsing
 * das linhas do ficheiro CSV de aeronaves e carregar os dados para o gestor de aeronaves.
 */

#ifndef PARSER_AERONAVE_H
#define PARSER_AERONAVE_H

#include "gestor_entidades/gestor_aeronaves.h"

/**
 * @brief Função que faz o parsing de uma linha do ficheiro CSV de aeronaves.
 *
 * Esta função lê uma linha do ficheiro CSV, valida os dados e cria uma estrutura Aeronaves.
 *
 * @param linha String que contém a linha do ficheiro CSV.
 * @return Apontador para a estrutura Aeronaves criada. Em caso de falha, retorna NULL.
 */
Aeronaves* parser_aeronave(char* linha);

/**
 * @brief Função que carrega as aeronaves de um ficheiro CSV para o gestor de aeronaves.
 *
 * Esta função lê o ficheiro CSV linha por linha, faz o parsing de cada linha e adiciona
 * as aeronaves que são válidas ao gestor de aeronaves.
 *
 * @param g Apontador para o gestor de aeronaves onde os dados serão carregados.
 * @param filename Nome do ficheiro CSV que vai ser lido.
 * @return Número de aeronaves que foram carregadas corretamente. Caso haja erro, retorna -1.
 */
int carregar_aeronaves_csv (GestorAeronaves* g, const char* filename);

#endif