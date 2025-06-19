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
    size_t tamanhoCache;
    double memoriaUtilizada; // Em KB aproximadamente
};

// ========== FUNÇÕES DE ANÁLISE DE PERFORMANCE ==========

/**
 * @brief Calcula o uso aproximado de memória do cache do algoritmo TED
 */
double calcularUsoMemoria(const TED& ted) {
    size_t tamanhoCache = ted.obterTamanhoCache();
    // Estimativa: cada entrada do cache tem 2 ponteiros (16 bytes) + double (8 bytes) = 24 bytes
    return (tamanhoCache * 24.0) / 1024.0; // Converter para KB
}

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
    
    // Calcular uso de memória
    double memoria = calcularUsoMemoria(ted);
    
    ResultadoPerformance resultado;
    resultado.tamanhoArvore1 = tamanho1;
    resultado.tamanhoArvore2 = tamanho2;
    resultado.tempoExecucaoMs = duracao.count() / 1000.0; // Converter para millisegundos
    resultado.custoTED = custo;
    resultado.tamanhoCache = ted.obterTamanhoCache();
    resultado.memoriaUtilizada = memoria;
    
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
    }
    
    // Cabeçalho CSV
    arquivo << "Tamanho1,Tamanho2,TempoMs,CustoTED,TamanhoCache,MemoriaKB" << endl;
    
    // Dados
    for (const auto& resultado : resultados) {
        arquivo << resultado.tamanhoArvore1 << ","
                << resultado.tamanhoArvore2 << ","
                << fixed << setprecision(4) << resultado.tempoExecucaoMs << ","
                << resultado.custoTED << ","
                << resultado.tamanhoCache << ","
                << resultado.memoriaUtilizada << endl;
    }
    
    arquivo.close();
    cout << "Resultados salvos em: " << nomeArquivo << endl;
}

/**
 * @brief Função principal com menu de testes
 */
int main() {
    cout << "========================================" << endl;
    cout << "  ANÁLISE DE PERFORMANCE - ALGORITMO DE SELKOW" << endl;
    cout << "       Tree Edit Distance (TED)" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    vector<ResultadoPerformance> resultados;
    vector<int> tamanhos = {10, 100, 1000, 10000};
    
    cout << "Executando testes para diferentes tamanhos de árvores..." << endl;
    cout << "Tamanhos testados: 10, 100, 1000, 10000 nós" << endl;
    cout << string(60, '-') << endl;
    
    for (int tamanho : tamanhos) {
        cout << "Testando árvores de tamanho " << tamanho << "...";
        cout.flush();
        
        auto inicioTeste = high_resolution_clock::now();
        
        // Teste com árvores de mesmo tamanho
        ResultadoPerformance resultado = executarTestePerformance(tamanho, tamanho, 
                                                                 tamanho * 10, 
                                                                 tamanho * 20);
        resultados.push_back(resultado);
        
        auto fimTeste = high_resolution_clock::now();
        auto duracaoTeste = duration_cast<milliseconds>(fimTeste - inicioTeste);
        
        cout << " Concluído em " << duracaoTeste.count() << " ms" << endl;
        
        // Mostrar resultados básicos
        cout << "  Tempo algoritmo: " << fixed << setprecision(2) 
             << resultado.tempoExecucaoMs << " ms" << endl;
        cout << "  Custo TED: " << resultado.custoTED << endl;
        cout << "  Cache: " << resultado.tamanhoCache << " entradas" << endl;
        cout << "  Memória: " << setprecision(2) << resultado.memoriaUtilizada << " KB" << endl;
        cout << endl;
    }
    
    cout << string(60, '-') << endl;
    
    // Salvar resultados em arquivo CSV
    salvarResultadosCSV(resultados, "resultados_complexidade.csv");
    
    cout << "========================================" << endl;
    cout << "           TESTES COMPLETOS" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    return 0;
}
