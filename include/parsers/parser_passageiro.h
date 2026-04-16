/** 
 * @file parser_passageiro.h
 * @brief Ficheiro de cabeçalho para as funções de parsing de Passageiros.
 * 
 * Este ficheiro contém as declarações das funções utilizadas para fazer o parsing
 * das linhas do ficheiro CSV de passageiros e carregar os dados para o gestor de passageiros.
 * 
 */

#ifndef PARSERS_PASSAGEIRO_H
#define PARSERS_PASSAGEIRO_H

#include "gestor_entidades/gestor_passageiros.h"

/**
 * @brief Função que faz o parsing de uma linha do ficheiro CSV de passageiros.
 *
 * Esta função lê uma linha do ficheiro CSV, valida os dados e cria uma estrutura Passageiros.
 *
 * @param linha String que contém a linha do ficheiro CSV.
 * @return Apontador para a estrutura Passageiros criada. Em caso de falha, retorna NULL.
 */
Passageiros* parser_passageiro(char* linha);

/**
 * @brief Função que carrega os passageiros de um ficheiro CSV para o gestor de passageiros.
 *
 * Esta função lê o ficheiro CSV linha por linha, faz o parsing de cada linha e adiciona
 * os passageiros que são válidos ao gestor de passageiros.
 *
 * @param g Apontador para o gestor de passageiros onde os dados serão carregados.
 * @param filename Nome do ficheiro CSV que vai ser lido.
 * @return Número de passageiros que foram carregados corretamente. Caso haja erro, retorna -1.
 */
int carregar_passageiros_csv (GestorPassageiros* g, const char* filename);

#endif 