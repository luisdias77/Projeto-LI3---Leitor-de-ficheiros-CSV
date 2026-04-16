/**
 * @file query6.h
 * @brief Ficheiro de cabeçalho para as funções da Query 6.
 *
 * Este ficheiro contém as declarações das funções utilizadas para processar a Query 6,
 * que determina o aeroporto de destino mais frequente para uma determinada nacionalidade.
 */

 #ifndef QUERY6_H
 #define QUERY6_H
 
 #include <stdio.h>
 #include "gestor_entidades/gestor_reservas.h"
 #include "gestor_entidades/gestor_voos.h"
 #include "gestor_entidades/gestor_passageiros.h"

 /**
 * @brief Inicializa a cache da Query 6 (Pré-processamento).
 * 
 * Esta função percorre todas as reservas uma única vez no início do programa.
 * Conta os destinos por nacionalidade e guarda apenas o aeroporto "vencedor" numa tabela de acesso rápido.
 * É chamada na main logo após o carregamento dos ficheiros CSV.
 * 
 * @param gestor_reservas     Apontador para o gestor de reservas.
 * @param gestor_voos         Apontador para o gestor de voos (necessário para validar destinos).
 * @param gestor_passageiros  Apontador para o gestor de passageiros (necessário para saber a nacionalidade).
 */
void init_query6(GestorReservas *gestor_reservas, GestorVoos *gestor_voos, GestorPassageiros *gestor_passageiros);

/**
 * @brief Executa a Query 6.
 * 
 * Consulta a cache pré-calculada para devolver instantaneamente o aeroporto de destino
 * mais frequente para a nacionalidade pedida.
 * 
 * @param nacionalidade   String com a nacionalidade a pesquisar
 * @param gr              Gestor de Reservas (usado para init se a cache estiver vazia).
 * @param gv              Gestor de Voos.
 * @param gp              Gestor de Passageiros.
 * @param separador       Caractere separador para o output.
 * @param output          Ficheiro onde será escrita a resposta.
 */
void query6(char* nacionalidade, GestorReservas* gr, GestorVoos* gv, GestorPassageiros* gp, char separador, FILE* output);

/**
 * @brief Limpa a memória da cache da Query 6.
 * * Deve ser chamada no final da execução do programa para libertar a memória alocada
 * pela tabela de resultados auxiliares.
 */
void destroy_query6(void);

#endif