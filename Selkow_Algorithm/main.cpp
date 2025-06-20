#include "ted.h"
#include <chrono>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std::chrono;

// ========== ESTRUTURA PARA RESULTADOS DE PERFORMANCE ==========

struct ResultadoPerformance {
    int tamanhoArvore1;
    int tamanhoArvore2;
    double tempoExecucaoMs;
    double custoTED;
    double espacoUtilizadoBytes;
};

// ========== FUNÇÕES DE ANÁLISE DE PERFORMANCE ==========

/**
 * @brief Executa um teste de performance para um par de árvores
 */
ResultadoPerformance executarTestePerformance(int tamanho1, int tamanho2, int seed1, int seed2) {
    // Criar árvores aleatórias
    Arvore arvore1(criarArvoreAleatoria(tamanho1, seed1));
    Arvore arvore2(criarArvoreAleatoria(tamanho2, seed2));
    
    // Configurar calculador de custos
    CalculadorDeCustos calculador(1.0, 1.0, 1.0);
    
    // Medir tempo de execução
    auto inicio = high_resolution_clock::now();
    
    TED ted(arvore1, arvore2, calculador);
    double custo = ted.obterCusto();
    
    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<microseconds>(fim - inicio);
      ResultadoPerformance resultado;
    resultado.tamanhoArvore1 = tamanho1;
    resultado.tamanhoArvore2 = tamanho2;
    resultado.tempoExecucaoMs = duracao.count() / 1000.0; // Converter para millisegundos
    resultado.custoTED = custo;
    resultado.espacoUtilizadoBytes = ted.obterEspacoUtilizado();
    
    return resultado;
}

/**
 * @brief Executa um teste de performance para um par de árvores completas
 */
ResultadoPerformance executarTestePerformanceCompletas(int tamanho1, int tamanho2) {
    // Criar árvores completas
    Arvore arvore1(criarArvoreCompleta(tamanho1));
    Arvore arvore2(criarArvoreCompleta(tamanho2));
    
    // Configurar calculador de custos
    CalculadorDeCustos calculador(1.0, 1.0, 1.0);
    
    // Medir tempo de execução
    auto inicio = high_resolution_clock::now();
    
    TED ted(arvore1, arvore2, calculador);
    double custo = ted.obterCusto();
    
    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<microseconds>(fim - inicio);
    
    ResultadoPerformance resultado;
    resultado.tamanhoArvore1 = tamanho1;
    resultado.tamanhoArvore2 = tamanho2;
    resultado.tempoExecucaoMs = duracao.count() / 1000.0; // Converter para millisegundos
    resultado.custoTED = custo;
    resultado.espacoUtilizadoBytes = ted.obterEspacoUtilizado();
    
    return resultado;
}

/**
 * @brief Salva os resultados em um arquivo CSV para análise posterior
 */
void salvarResultadosCSV(const vector<ResultadoPerformance>& resultados, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cout << "Erro: Não foi possível criar o arquivo " << nomeArquivo << endl;
        return;
    }    // Cabeçalho CSV
    arquivo << "Tamanho1,Tamanho2,TempoMs,CustoTED,EspacoBytes" << endl;
    
    // Dados
    for (const auto& resultado : resultados) {
        arquivo << resultado.tamanhoArvore1 << ","
                << resultado.tamanhoArvore2 << ","
                << fixed << setprecision(4) << resultado.tempoExecucaoMs << ","
                << resultado.custoTED << ","
                << resultado.espacoUtilizadoBytes << endl;
    }
    
    arquivo.close();
    cout << "Resultados salvos em: " << nomeArquivo << endl;
}

/**
 * @brief Função principal - Análise de Performance do Algoritmo de Selkow
 * 
 * Este programa testa o algoritmo de Selkow para o problema de Tree Edit Distance (TED),
 * medindo o tempo de execução e o espaço utilizado para árvores de diferentes tamanhos.
 * 
 * Os testes são executados com árvores aleatórias de tamanhos 10, 100, 1000 e 10000 nós,
 * medindo o tempo real de execução em millisegundos e o espaço das matrizes em bytes.
 */
int main() {
    cout << "========================================" << endl;
    cout << "     ANÁLISE DE PERFORMANCE" << endl;
    cout << "       ALGORITMO DE SELKOW - TED" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
      vector<ResultadoPerformance> resultados;
    vector<int> tamanhos = {10, 100, 1000, 10000};
    
    cout << "EXECUTANDO TESTES EXPERIMENTAIS:" << endl;
    cout << "Tamanhos de árvores testados: 10, 100, 1000, 10000 nós" << endl;
    cout << "Cada teste compara duas árvores aleatórias de mesmo tamanho." << endl;
    cout << string(70, '-') << endl;
    
    for (int tamanho : tamanhos) {
        cout << "Testando árvores de tamanho " << tamanho << " nós...";
        cout.flush();
        
        auto inicioTeste = high_resolution_clock::now();
        
        // Teste com árvores de mesmo tamanho
        ResultadoPerformance resultado = executarTestePerformance(tamanho, tamanho, 
                                                                 tamanho * 10, 
                                                                 tamanho * 20);
        resultados.push_back(resultado);
        
        auto fimTeste = high_resolution_clock::now();
        auto duracaoTeste = duration_cast<milliseconds>(fimTeste - inicioTeste);
        
        cout << " Concluído em " << duracaoTeste.count() << " ms" << endl;        // Mostrar resultados básicos
        cout << "  Tempo de execução: " << fixed << setprecision(2) 
             << resultado.tempoExecucaoMs << " ms" << endl;
        cout << "  Custo TED (operações mínimas): " << resultado.custoTED << endl;
        cout << "  Espaço utilizado: " << setprecision(1) 
             << (resultado.espacoUtilizadoBytes / 1024.0) << " KB" << endl;
        cout << endl;
    }    cout << string(70, '-') << endl;
      // Análise comparativa dos resultados
    cout << "RESUMO DOS RESULTADOS:" << endl;
    cout << endl;
    cout << "Tamanho\tTempo(ms)\tEspaço(KB)\tCusto TED" << endl;
    cout << string(50, '-') << endl;
    for (const auto& resultado : resultados) {
        cout << resultado.tamanhoArvore1 << "\t"
             << fixed << setprecision(2) << resultado.tempoExecucaoMs << "\t\t"
             << setprecision(1) << (resultado.espacoUtilizadoBytes / 1024.0) << "\t\t"
             << resultado.custoTED << endl;
    }
    cout << endl;
      // Salvar resultados em arquivo CSV
    salvarResultadosCSV(resultados, "resultados_selkow.csv");
    
    cout << "========================================" << endl;
    cout << "        TESTES COM ÁRVORES COMPLETAS" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    vector<ResultadoPerformance> resultadosCompletas;
    
    cout << "EXECUTANDO TESTES COM ÁRVORES COMPLETAS:" << endl;
    cout << "Tamanhos de árvores testados: 10, 100, 1000, 10000 nós" << endl;
    cout << "Cada árvore completa tem 1 raiz com N-1 filhos (todas folhas)." << endl;
    cout << string(70, '-') << endl;
    
    for (int tamanho : tamanhos) {
        cout << "Testando árvores completas de tamanho " << tamanho << " nós...";
        cout.flush();
        
        auto inicioTeste = high_resolution_clock::now();
        
        // Teste com árvores completas de mesmo tamanho
        ResultadoPerformance resultado = executarTestePerformanceCompletas(tamanho, tamanho);
        resultadosCompletas.push_back(resultado);
        
        auto fimTeste = high_resolution_clock::now();
        auto duracaoTeste = duration_cast<milliseconds>(fimTeste - inicioTeste);
        
        cout << " Concluído em " << duracaoTeste.count() << " ms" << endl;
        
        // Mostrar resultados básicos
        cout << "  Tempo de execução: " << fixed << setprecision(2) 
             << resultado.tempoExecucaoMs << " ms" << endl;
        cout << "  Custo TED (operações mínimas): " << resultado.custoTED << endl;
        cout << "  Espaço utilizado: " << setprecision(1) 
             << (resultado.espacoUtilizadoBytes / 1024.0) << " KB" << endl;
        cout << endl;
    }
    
    cout << string(70, '-') << endl;
    
    // Análise comparativa dos resultados para árvores completas
    cout << "RESUMO DOS RESULTADOS (ÁRVORES COMPLETAS):" << endl;
    cout << endl;
    cout << "Tamanho\tTempo(ms)\tEspaço(KB)\tCusto TED" << endl;
    cout << string(50, '-') << endl;
    for (const auto& resultado : resultadosCompletas) {
        cout << resultado.tamanhoArvore1 << "\t"
             << fixed << setprecision(2) << resultado.tempoExecucaoMs << "\t\t"
             << setprecision(1) << (resultado.espacoUtilizadoBytes / 1024.0) << "\t\t"
             << resultado.custoTED << endl;
    }
    cout << endl;
    
    // Salvar resultados das árvores completas em arquivo CSV separado
    salvarResultadosCSV(resultadosCompletas, "resultados_selkow_completas.csv");
    
    cout << "========================================" << endl;
    cout << "        TESTES COMPLETOS" << endl;
    cout << "========================================" << endl;
    cout << endl;
    cout << "OBSERVAÇÕES:" << endl;
    cout << "1. Foram executados dois conjuntos de testes:" << endl;
    cout << "   - Árvores aleatórias: estrutura variável" << endl;
    cout << "   - Árvores completas: 1 raiz + (N-1) folhas" << endl;
    cout << "2. O tempo de execução varia conforme a estrutura da árvore." << endl;
    cout << "3. O espaço utilizado representa a memória das matrizes" << endl;
    cout << "   criadas durante a execução do algoritmo." << endl;
    cout << "4. O custo TED representa o número mínimo de operações" << endl;
    cout << "   (inserção, deleção, substituição) necessárias." << endl;    cout << "5. Os resultados foram salvos em dois arquivos CSV separados." << endl;
    cout << endl;
    
    return 0;
}
