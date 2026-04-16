/**
 * @file query2.h
 * @brief Ficheiro de cabeçalho para as funções da Query 2.
 *
 * Este ficheiro contém as declarações das funções utilizadas para processar a Query 2,
 * que determina as aeronaves com maior número de voos.
 */

 #ifndef QUERY2_H
 #define QUERY2_H
 
 #include <stdio.h>
 #include "gestor_entidades/gestor_aeronaves.h"
 #include "gestor_entidades/gestor_voos.h"
 
 /**
  * @brief Inicializa a cache da Query 2 (Pré-processamento).
  * 
  * Esta função percorre todos os voos uma única vez, conta as ocorrências por aeronave,
  * ordena os resultados e guarda-os numa estrutura estática.
  * 
  * @param gv Apontador para o Gestor de Voos.
  * @param ga Apontador para o Gestor de Aeronaves.
  */
 void init_query2(GestorVoos *gv, GestorAeronaves *ga);
 
 /** 
  * @brief Determina o Top N das aeronaves com o maior número de voos realizados.
  *
  * Acede à cache pré-calculada para retornar os resultados instantaneamente.
  * 
  * @param top              Número (N) de aeronaves que devem constar no output.
  * @param filtro           String opcional do fabricante a ser utilizada como filtro.
  * @param gestor_voos      (Não usado se a cache já estiver iniciada) Gestor de Voos.
  * @param gestor_aeronaves (Não usado se a cache já estiver iniciada) Gestor de Aeronaves.
  * @param separador        Caractere a utilizar como separador de campos.
  * @param output           Apontador para o ficheiro de output.
  */
 void query2(int top, char *filtro, GestorVoos *gestor_voos, GestorAeronaves *gestor_aeronaves, char separador, FILE *output);
 
 /**
  * @brief Limpa a memória utilizada pela cache da Query 2.
  * * Deve ser chamada no final do programa para garantir que não existem memory leaks.
  */
 void destroy_query2(void);
 
 #endif