/**
 * @file gestor_aeroportos.h
 * @brief Ficheiro de cabeçalho para as funções associadas ao gestor de aeroportos.
 * 
 * Este ficheiro contém as declarações das funções utilizadas para criar, destruir,
 * adicionar, remover e consultar aeroportos no gestor de aeroportos, bem como os getters usados para aceder aos aeroportos armazenados,
 * ou a campos específicos dos mesmos.
 */

#ifndef _GESTOR_AEROPORTOS_H_
#define _GESTOR_AEROPORTOS_H_

#include <glib.h>
#include "entidades/aeroportos.h"

/**
 * @brief Estrutura que representa o gestor de aeroportos.
 * 
 * Esta estrutura contém uma tabela hash que armazena os aeroportos,
 * onde a chave é o código do aeroporto (char*) e o valor é um apontador para a estrutura Aeroportos*.
 */

typedef struct gestor_aeroportos GestorAeroportos;  

/**
 * @brief Função para criar o gestor de aeroportos.
 * 
 * Esta função aloca memória para uma nova estrutura GestorAeroportos
 * inicializando a tabela hash que irá armazenar os aeroportos.
 * 
 * @return Apontador para a nova estrutura GestorAeroportos criada.
 */

GestorAeroportos* criar_gestor_aeroportos();

/**
 * @brief Função para destruir o gestor de aeroportos.
 * 
 * Esta função liberta a memória ocupada pela estrutura GestorAeroportos,
 * incluindo a tabela hash e todos os aeroportos que estão armazenados nela.
 * 
 * @param g Apontador para a estrutura GestorAeroportos que vai destruída.
 */

void destruir_gestor_aeroportos(GestorAeroportos* g);

/**
 * @brief Função para adicionar um aeroporto ao gestor de aeroportos.
 * 
 * Esta função insere um novo aeroporto na tabela hash do gestor,
 * usando o código do aeroporto como chave.
 * 
 * @param g Apontador para a estrutura GestorAeroportos onde será adiconada o respetivo aeroporto.
 * @param a Apontador para a estrutura Aeroportos que será adicionada ao gestor.
 */

void adiciona_aeroporto(GestorAeroportos* g, Aeroportos* a);

/**
 * @brief Função para verificar se um aeroporto existe no gestor de aeroportos.
 * 
 * Esta função verifica se um aeroporto com um código fornecido
 * existe na tabela hash do gestor.
 * 
 * @param g Apontador para a estrutura GestorAeroportos onde será irá ser verificado se o aeroporto existe.
 * @param codigo String que representa o código do aeroporto a ser verificado.
 * @return true se o aeroporto existir, ou false, no caso de o aeroporto não existir.
 */

bool aeroporto_existe(GestorAeroportos* g, char* codigo);

/**
 * @brief Função que remove um aeroporto do gestor de aeroportos.
 *
 * Esta função remove um aeroporto da tabela hash do gestor,
 * usando o código do aeroporto como chave.
 *
 * @param g Apontador para a estrutura GestorAeroportos onde será removido o respetivo aeroporto.
 * @param codigo String que representa o código do aeroporto a ser removido.
 */

void remove_aeroporto(GestorAeroportos* g, char* codigo);

/**
 * @brief Função que conta o numero de aeroportos na hashtable.
 * 
 * Esta função percorre a tabela hash do gestor de aeroportos
 * e conta o número total de aeroportos armazenados.
 * 
 * @param g Apontador para a estrutura GestorAeroportos onde será contado o número de aeroportos.
 * @return Inteiro representando o número total de aeroportos na tabela hash.
 */
 
int conta_aeroportos(GestorAeroportos* g);

/**
 * @brief Função para validar o tipo de aeroporto.
 *
 * Esta função verifica se o tipo de aeroporto fornecido é válido
 * com base numa lista definida de tipos que são permitidos, tais como "small_airport", "medium_airport", "large_airport", "heliport", e "seaplane_base".
 *
 * @param type String que representa o tipo de aeroporto que será validado.
 * @return Inteiro que indica se o tipo é válido (1) ou inválido (0).
 */

int validar_tipo_aeroporto(const char* type);

/**
 * @brief Função para validar a latitude e longitude de um aeroporto.
 * 
 * Esta função verifica se o formato da latitude ou longitude fornecida é válido.
 * 
 * @param token String que representa a latitude ou longitude a ser validada.
 * @return true se o valor for inválido, ou false se for válido.
 */

int valida_formato (const char* token);

/**
 * @brief Função para validar a latitude.
 *
 * Esta função verifica se a latitude fornecida está dentro do intervalo válido de -90 a 90 graus.
 * 
 * @param token String que representa a latitude a ser validada.
 *
 * @return true se a latitude for inválida, ou false se for válida.
 */

bool validacaoLatitude(const char *token);

/**
 * @brief Função para validar a longitude.
 *
 * Esta função verifica se a longitude fornecida está dentro do intervalo válido de -180 a 180 graus.
 * 
 * @param token String que representa a longitude a ser validada.
 *
 * @return true se a longitude for inválida, ou false se for válida.
 */

bool validacaoLongitude(const char *token);

/**
 * @brief Getter para o gestor de aeroportos , para aceder a um aeroporto com um determinado code (codigo IATA).
 *
 * Esta função retorna um apontador para a estrutura Aeroportos
 * que corresponde ao código iata fornecido.
 *
 * @param g Apontador para a estrutura GestorAeroportos onde será procurado o aeroporto.
 * @param code String que representa o código IATA do aeroporto a ser procurado.
 *
 * @return Apontador para a estrutura Aeroportos correspondente ao código fornecido, ou NULL se não for encontrado.
 */

Aeroportos* get_aeroporto(GestorAeroportos* g, const char* code);

/**
 * @brief Getter para a tabela hash de aeroportos.
 * 
 * Esta função retorna a tabela hash que armazena os aeroportos no gestor.
 *
 * @param g Apontador para a estrutura GestorAeroportos onde será acedida a tabela de aeroportos.
 *
 * @return Apontador para a tabela hash de aeroportos.
 */

GHashTable* get_tabela_aeroportos(GestorAeroportos* g);


#endif
