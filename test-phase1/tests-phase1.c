#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
#include <ctype.h> 


// --- Função Auxiliar: compare_files_detailed ---
int compare_files_detailed(const char *expected_file_path, const char *obtained_file_path) {
    
    FILE *f_expected = fopen(expected_file_path, "r");
    FILE *f_obtained = fopen(obtained_file_path, "r");

    // Verificar se o ficheiro esperado existe
    if (f_expected == NULL) {
        fprintf(stderr, "ERRO CRÍTICO: Não foi possível abrir o ficheiro ESPERADO: %s\n", expected_file_path);
        if (f_obtained) fclose(f_obtained);
        return 1;
    }
    // Verificar se o programa-principal gerou o ficheiro
    if (f_obtained == NULL) {
        fprintf(stderr, "  [FALHA] O seu programa não gerou o ficheiro de output: %s\n", obtained_file_path);
        fclose(f_expected);
        return 1; 
    }

    char line_expected[4096];
    char line_obtained[4096];
    int line_num = 0;
    int result = 0; // 0 = sucesso, 1 = falha

    while (1) {
        line_num++;

        char *res_expected = fgets(line_expected, 4096, f_expected);
        char *res_obtained = fgets(line_obtained, 4096, f_obtained);

        // Limpar o '\n' do final das linhas
        if (res_expected) line_expected[strcspn(line_expected, "\n")] = 0;
        if (res_obtained) line_obtained[strcspn(line_obtained, "\n")] = 0;

        // Verificar Fim de Ficheiro
        if (res_expected == NULL && res_obtained == NULL) {
            break; // Sucesso, ambos terminaram
        }

        // Ficheiro obtido terminou cedo
        if (res_obtained == NULL) {
            fprintf(stderr, "\n  [FALHA] O ficheiro obtido (%s) terminou mais cedo.\n", obtained_file_path);
            fprintf(stderr, "    Esperado na linha %d: \"%s\"\n", line_num, line_expected);
            result = 1;
            break;
        }

        // Ficheiro obtido tem linhas extra
        if (res_expected == NULL) {
            fprintf(stderr, "\n  [FALHA] O ficheiro obtido (%s) tem linhas extra.\n", obtained_file_path);
            fprintf(stderr, "    Obtido na linha %d: \"%s\"\n", line_num, line_obtained);
            result = 1;
            break;
        }

        // Comparar conteúdo
        if (strcmp(line_expected, line_obtained) != 0) {
            fprintf(stderr, "\n  [FALHA] Incongruência no ficheiro '%s' \n", obtained_file_path);
            fprintf(stderr, "    Linha %d:\n", line_num);
            fprintf(stderr, "    Esperado: \"%s\"\n", line_expected);
            fprintf(stderr, "    Obtido:   \"%s\"\n", line_obtained);
            result = 1;
            break; // Para na primeira incongruência
        }
    }

    fclose(f_expected);
    fclose(f_obtained);
    return result;
}

// --- Função Auxiliar: print_log_file ---
void print_log_file(const char *log_path) {
    FILE *f = fopen(log_path, "r");
    if (f == NULL) {
        fprintf(stderr, "  AVISO: Ficheiro de log '%s' não encontrado. O programa-principal não o gerou?\n", log_path);
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), f) != NULL) {
        printf("    %s", line); // Imprime a linha (já tem \n)
    }
    fclose(f);
}

// --- Função Principal ---
int main(int argc, char *argv[]) {

    // 1. Verificação dos Argumentos 
    if (argc != 4) {
        fprintf(stderr, "Erro: Número de argumentos incorreto.\n");
        fprintf(stderr, "Uso: %s <pasta_datasets> <ficheiro_inputs> <pasta_outputs_esperados>\n", argv[0]);
        fprintf(stderr, "NOTA: O programa-principal DEVE ser executado MANUALMENTE antes deste teste.\n");
        return 1;
    }

    // 2. Atribuição de Argumentos a Variáveis Locais
    const char *dataset_path = argv[1]; // Não usado diretamente, mas recebido
    const char *input_file_path = argv[2]; // Usado para contar queries por tipo
    const char *expected_outputs_dir = argv[3]; // Usado para encontrar os outputs esperados

    // Caminhos fixos (hardcoded) para os diretórios e logs
    const char *program_query_output_dir = "resultados/"; 
    const char *query_times_log_file = "test-phase1/query_times.log";
    const char *memory_usage_log_file = "test-phase1/memory_usage.log";

    // Contadores para o requisito "ocorrências corretas para cada tipo de query" 
    int q1_correct = 0, q1_total = 0;
    int q2_correct = 0, q2_total = 0;
    int q3_correct = 0, q3_total = 0;
    int q4_correct = 0, q4_total = 0;
    int q5_correct = 0, q5_total = 0;
    int q6_correct = 0, q6_total = 0;

    
    int global_fail = 0; // Variável para indicar se houve alguma falha global

    printf("\n=== PROGRAMA DE TESTES (Fase 1) ===\n");
    printf("  Dataset: '%s'\n", dataset_path);
    printf("A comparar outputs de '%s' com os esperados de '%s'\n\n", program_query_output_dir, expected_outputs_dir);

    // 3. Verificar se as pastas de output existem
    if (access(program_query_output_dir, F_OK) == -1) {
        fprintf(stderr, "ERRO CRÍTICO: A pasta '%s' não existe.\n", program_query_output_dir);
        fprintf(stderr, "Sugestão: Execute o programa-principal antes de correr os testes.\n");
        return 1;
    }
    if (access(expected_outputs_dir, F_OK) == -1) {
        fprintf(stderr, "ERRO CRÍTICO: A pasta de outputs esperados '%s' não existe.\n", expected_outputs_dir);
        return 1;
    }

    // 4. Abrir ficheiro de inputs para iterar pelas queries 
    FILE *input_file = fopen(input_file_path, "r");
    if (input_file == NULL) {
        fprintf(stderr, "ERRO CRÍTICO: Não foi possível abrir o ficheiro de inputs '%s'\n", input_file_path);
        return 1;
    }

    char line_buffer[1024];
    int command_index = 1; // Os outputs começam em command1_output.txt

    // Loop principal: Lê o input, determina o tipo de query, e compara o output correspondente
    while (fgets(line_buffer, sizeof(line_buffer), input_file) != NULL) {
        
        // Ignorar linhas vazias
        if (isspace((unsigned char)line_buffer[0])) {
            continue;
        }

        int query_type = 0;
        // Tenta ler o primeiro número (ID da query)
        sscanf(line_buffer, "%d", &query_type);

        // Incrementar contadores de total por tipo de query
        if (query_type == 1) q1_total++;
        else if (query_type == 2) q2_total++;
        else if (query_type == 3) q3_total++;
        else if (query_type == 4) q4_total++;
        else if (query_type == 5) q5_total++;
        else if (query_type == 6) q6_total++;
        
        else continue; // Ignora linhas que não comecem por um ID de query conhecido

        // Construir os caminhos dos ficheiros a comparar
        char expected_file[512];
        char obtained_file[512];
        
        // Ex: "expected_outputs/command1_output.txt"
        snprintf(expected_file, 512, "%s/command%d_output.txt", expected_outputs_dir, command_index);
        // Ex: "resultados/command1_output.txt"
        snprintf(obtained_file, 512, "%s/command%d_output.txt", program_query_output_dir, command_index);

        printf("--- A testar Comando %d (Query %d) ---\n", command_index, query_type);
        
        // Chamar a função de comparação detalhada
        int res = compare_files_detailed(expected_file, obtained_file);

        if (res == 0) {
            printf("  [PASSOU]\n");
            // Incrementar os contadores de sucesso por tipo de query
            if (query_type == 1) q1_correct++;
            else if (query_type == 2) q2_correct++;
            else if (query_type == 3) q3_correct++;
            else if (query_type == 4) q4_correct++;
            else if (query_type == 5) q5_correct++;
            else if (query_type == 6) q6_correct++;
            // ...
        } else {
            global_fail = 1;
            fprintf(stderr, "\n>>> TESTAGEM INTERROMPIDA devido a falha no Comando %d.\n", command_index);
            break; // Para a testagem ao encontrar a primeira falha
        }

        command_index++; // Próximo ficheiro (command2_output.txt)
    }
    fclose(input_file);


    // 5. Apresentar Sumário Final
    printf("\n=== SUMÁRIO DE CORREÇÃO ===\n");
    printf("Q1: %d de %d testes ok!\n", q1_correct, q1_total);
    printf("Q2: %d de %d testes ok!\n", q2_correct, q2_total);
    printf("Q3: %d de %d testes ok!\n", q3_correct, q3_total);
    printf("Q4: %d de %d testes ok!\n", q4_correct, q4_total);
    printf("Q5: %d de %d testes ok!\n", q5_correct, q5_total);
    printf("Q6: %d de %d testes ok!\n", q6_correct, q6_total);  


    if (global_fail) {
        printf("\nRESULTADO: FALHA (Incongruência encontrada)\n");
        return 1;
    }

    // Se passou todos os testes de correção, mostra os logs de performance
    printf("\nTODOS OS TESTES DE CORREÇÃO PASSARAM!\n");

    // 6. Apresentar os Tempos por Query
    printf("\n=== TEMPOS DE EXECUÇÃO (Reportado por 'programa-principal') ===\n");
    print_log_file(query_times_log_file);

    // 7. Apresentar o Uso de Memória 
    printf("\n=== USO DE MEMÓRIA (Reportado por 'programa-principal') ===\n");
    print_log_file(memory_usage_log_file);

    printf("\nRESULTADO: SUCESSO\n");
    return 0;
}