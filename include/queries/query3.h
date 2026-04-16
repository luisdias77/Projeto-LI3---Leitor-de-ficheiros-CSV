/**
 * @file query3.h
 * @brief Ficheiro de cabeçalho para as funções da Query 3.
 *
 * Este ficheiro contém as declarações das funções utilizadas para processar a Query 3,
 * que identifica o aeroporto com mais partidas num intervalo de tempo.
 */

 #ifndef QUERY3_H
 #define QUERY3_H
 
 #include <stdio.h>
 #include "gestor_entidades/gestor_voos.h"
 #include "gestor_entidades/gestor_aeroportos.h"
 
 /**
  * @brief Inicializa a cache da Query 3 (Pré-processamento).
  * 
  * Esta função cria um array auxiliar contendo apenas a data e a origem dos voos.
  * Filtra imediatamente voos cancelados ou inválidos e ordena o array por data.
  * Deve ser chamada na main após o carregamento dos CSVs.
  * 
  * * @param gv Apontador para o Gestor de Voos.
  */
 void init_query3(GestorVoos *gv);
 
 /**
  * @brief Calcula o aeroporto com mais partidas num intervalo de tempo.
  * 
  * Acede à cache pré-ordenada para encontrar rapidamente os voos dentro do intervalo,
  * parando a pesquisa assim que a data ultrapassa o limite final.
  * 
  * @param g            Apontador para o Gestor de Voos (usado para init se necessário).
  * @param a            Apontador para o Gestor de Aeroportos (para obter detalhes do vencedor).
  * @param data_inicio  String da data de início (formato YYYY-MM-DD...).
  * @param data_fim     String da data de fim (formato YYYY-MM-DD...).
  * @param separador    Caractere separador para o output.
  * @param output       Apontador para o ficheiro de escrita.
  */
 void query3(GestorVoos* g, GestorAeroportos* a, char* data_inicio, char* data_fim, char separador, FILE* output);
 
 /**
  * @brief Limpa a memória da cache da Query 3.
  * * Deve ser chamada no final da execução do programa para evitar leaks de memória.
  */
 void destroy_query3(void);
 
 #endif