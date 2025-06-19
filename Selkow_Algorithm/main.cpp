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
 * @brief Analisa os resultados coletados e imprime estatísticas
 */
void analisarResultados(const vector<ResultadoPerformance>& resultados) {
    cout << "=== ANÁLISE ESTATÍSTICA DOS RESULTADOS ===" << endl;
    cout << endl;
    
    if (resultados.empty()) {
        cout << "Nenhum resultado para analisar." << endl;
        return;
    }
    
    // Calcular estatísticas
    double tempoMin = resultados[0].tempoExecucaoMs;
    double tempoMax = resultados[0].tempoExecucaoMs;
    double tempoTotal = 0.0;
    double memoriaTotal = 0.0;
    
    for (const auto& resultado : resultados) {
        tempoMin = min(tempoMin, resultado.tempoExecucaoMs);
        tempoMax = max(tempoMax, resultado.tempoExecucaoMs);
        tempoTotal += resultado.tempoExecucaoMs;
        memoriaTotal += resultado.memoriaUtilizada;
    }
    
    double tempoMedio = tempoTotal / resultados.size();
    double memoriMedia = memoriaTotal / resultados.size();
    
    cout << "Estatísticas de Tempo de Execução:" << endl;
    cout << "  Tempo mínimo: " << fixed << setprecision(2) << tempoMin << " ms" << endl;
    cout << "  Tempo máximo: " << tempoMax << " ms" << endl;
    cout << "  Tempo médio:  " << tempoMedio << " ms" << endl;
    cout << "  Fator de crescimento: " << (tempoMax / tempoMin) << "x" << endl;
    cout << endl;
    
    cout << "Estatísticas de Uso de Memória:" << endl;
    cout << "  Memória média: " << memoriMedia << " KB" << endl;
    cout << "  Memória total testada: " << memoriaTotal << " KB" << endl;
    cout << endl;
      // Análise de complexidade baseada no produto dos tamanhos (n1 × n2)
    if (resultados.size() >= 3) {
        cout << "Análise de Complexidade usando Regressão Linear:" << endl;
        cout << "Modelo assumido: T(n1,n2) = c × (n1 × n2)^k" << endl;
        cout << endl;
        
        // Calcular regressão linear em escala logarítmica
        double somaLogProduto = 0, somaLogTempo = 0;
        double somaLogProdutoQuadrado = 0, somaLogProdutoTempo = 0;
        int n = resultados.size();
        
        cout << "Dados para regressão:" << endl;
        cout << "Tamanho1 × Tamanho2 | Produto | Tempo(ms) | log(Produto) | log(Tempo)" << endl;
        cout << string(75, '-') << endl;
        
        for (const auto& resultado : resultados) {
            double produto = (double)resultado.tamanhoArvore1 * resultado.tamanhoArvore2;
            double logProduto = log(produto);
            double logTempo = log(resultado.tempoExecucaoMs);
            
            somaLogProduto += logProduto;
            somaLogTempo += logTempo;
            somaLogProdutoQuadrado += logProduto * logProduto;
            somaLogProdutoTempo += logProduto * logTempo;
            
            cout << setw(7) << resultado.tamanhoArvore1 << " × " << setw(7) << resultado.tamanhoArvore2 
                 << " | " << setw(8) << (int)produto 
                 << " | " << setw(8) << fixed << setprecision(2) << resultado.tempoExecucaoMs
                 << " | " << setw(11) << setprecision(3) << logProduto
                 << " | " << setw(9) << setprecision(3) << logTempo << endl;
        }
        
        cout << string(75, '-') << endl;
        
        // Calcular coeficiente angular (expoente) usando mínimos quadrados
        double expoente = (n * somaLogProdutoTempo - somaLogProduto * somaLogTempo) / 
                         (n * somaLogProdutoQuadrado - somaLogProduto * somaLogProduto);
        
        // Calcular coeficiente linear (constante)
        double logConstante = (somaLogTempo - expoente * somaLogProduto) / n;
        double constante = exp(logConstante);
        
        // Calcular coeficiente de correlação R²
        double mediaLogTempo = somaLogTempo / n;
        double ssRes = 0, ssTot = 0;
        
        for (const auto& resultado : resultados) {
            double produto = (double)resultado.tamanhoArvore1 * resultado.tamanhoArvore2;
            double logProduto = log(produto);
            double logTempo = log(resultado.tempoExecucaoMs);
            double logTempoPredict = logConstante + expoente * logProduto;
            
            ssRes += (logTempo - logTempoPredict) * (logTempo - logTempoPredict);
            ssTot += (logTempo - mediaLogTempo) * (logTempo - mediaLogTempo);
        }
        
        double rSquared = 1.0 - (ssRes / ssTot);
        
        cout << endl;
        cout << "Resultados da Regressão Linear:" << endl;
        cout << "  Expoente (k): " << fixed << setprecision(3) << expoente << endl;
        cout << "  Constante (c): " << setprecision(2) << constante << endl;
        cout << "  R² (qualidade do ajuste): " << setprecision(4) << rSquared << endl;
        cout << "  Complexidade aproximada: O((n1 × n2)^" << setprecision(2) << expoente << ")" << endl;
        
        // Interpretação da complexidade
        cout << endl;
        cout << "Interpretação:" << endl;
        if (expoente < 0.8) {
            cout << "  Complexidade sublinear - algoritmo muito eficiente" << endl;
        } else if (expoente >= 0.8 && expoente < 1.2) {
            cout << "  Complexidade aproximadamente linear O(n×m)" << endl;
        } else if (expoente >= 1.2 && expoente < 1.8) {
            cout << "  Complexidade entre linear e quadrática" << endl;
        } else if (expoente >= 1.8 && expoente < 2.2) {
            cout << "  Complexidade aproximadamente quadrática O((n×m)²)" << endl;
        } else {
            cout << "  Complexidade super-quadrática - pode ser problemática para inputs grandes" << endl;
        }
        
        if (rSquared > 0.95) {
            cout << "  Ajuste excelente (R² > 0.95)" << endl;
        } else if (rSquared > 0.90) {
            cout << "  Ajuste bom (R² > 0.90)" << endl;
        } else if (rSquared > 0.80) {
            cout << "  Ajuste razoável (R² > 0.80)" << endl;
        } else {
            cout << "  Ajuste ruim (R² < 0.80) - modelo pode não ser apropriado" << endl;
        }
    }
    
    cout << endl;
}

/**
 * @brief Executa uma bateria de testes para diferentes tamanhos de árvores
 */
void executarTestesComplexidade() {
    cout << "=== ANÁLISE DE COMPLEXIDADE DO ALGORITMO DE SELKOW ===" << endl;
    cout << endl;
    
    vector<ResultadoPerformance> resultados;
    vector<int> tamanhos = {10, 100, 1000, 10000};
    
    cout << "Executando testes para diferentes tamanhos de árvores..." << endl;
    cout << "Formato: [Tamanho1] x [Tamanho2] -> Tempo (ms), Custo TED, Cache, Memória (KB)" << endl;
    cout << "ATENÇÃO: Testes maiores (10K+ nós) podem demorar vários minutos!" << endl;
    cout << string(100, '-') << endl;
    
    for (int tamanho : tamanhos) {
        cout << "Testando árvores de tamanho " << tamanho << "...";
        cout.flush(); // Força a impressão imediata
        
        auto inicioTeste = high_resolution_clock::now();
        
        // Teste com árvores de mesmo tamanho
        ResultadoPerformance resultado = executarTestePerformance(tamanho, tamanho, 
                                                                 tamanho * 10, 
                                                                 tamanho * 20);
        resultados.push_back(resultado);
        
        auto fimTeste = high_resolution_clock::now();
        auto duracaoTeste = duration_cast<milliseconds>(fimTeste - inicioTeste);
        
        cout << " Concluído em " << duracaoTeste.count() << " ms" << endl;
        
        cout << fixed << setprecision(2);
        cout << setw(7) << resultado.tamanhoArvore1 << " x " << setw(7) << resultado.tamanhoArvore2 
             << " -> " << setw(10) << resultado.tempoExecucaoMs << " ms, "
             << "Custo: " << setw(8) << resultado.custoTED << ", "
             << "Cache: " << setw(8) << resultado.tamanhoCache << ", "
             << "Mem: " << setw(8) << resultado.memoriaUtilizada << " KB" << endl;
          // Para tamanhos grandes, adicionar informação extra
        if (tamanho >= 1000) {
            double produto = (double)tamanho * tamanho; // Para árvores de mesmo tamanho
            cout << "    Cache/(n1×n2) ratio: " << setprecision(4) 
                 << resultado.tamanhoCache / produto << endl;
            cout << "    Tempo por (n1×n2): " << setprecision(8) 
                 << resultado.tempoExecucaoMs / produto << " ms/(n1×n2)" << endl;
        }
    }
    
    cout << string(100, '-') << endl;
    cout << endl;
    
    // Salvar resultados em arquivo
    salvarResultadosCSV(resultados, "resultados_complexidade.csv");
    
    // Análise dos resultados
    analisarResultados(resultados);
}

/**
 * @brief Testa diferentes combinações de tamanhos de árvores
 */
void executarTestesVariados() {
    cout << "=== TESTES COM DIFERENTES COMBINAÇÕES DE TAMANHOS ===" << endl;
    cout << endl;
    
    vector<pair<int, int>> combinacoes = {
        {100, 50}, {50, 100},     // Tamanhos diferentes
        {1000, 500}, {500, 1000}, // Proporção 2:1
        {100, 100},               // Tamanhos iguais pequenos
        {1000, 1000},             // Tamanhos iguais médios
        {5000, 2500}, {2500, 5000} // Proporção 2:1 maior
    };
    
    cout << "Analisando diferentes proporções de tamanhos..." << endl;
    cout << string(70, '-') << endl;
    
    for (auto& combo : combinacoes) {
        ResultadoPerformance resultado = executarTestePerformance(combo.first, combo.second, 
                                                                 combo.first * 7, 
                                                                 combo.second * 13);
        
        cout << fixed << setprecision(2);
        cout << "Árvores " << setw(2) << combo.first << " x " << setw(2) << combo.second 
             << ": " << setw(8) << resultado.tempoExecucaoMs << " ms, "
             << "Cache: " << setw(4) << resultado.tamanhoCache << " entradas" << endl;
    }
    
    cout << string(70, '-') << endl;
    cout << endl;
}

/**
 * @brief Analisa o comportamento do cache durante a execução
 */
void analisarComportamentoCache() {
    cout << "=== ANÁLISE DO COMPORTAMENTO DO CACHE ===" << endl;
    cout << endl;
    
    vector<int> tamanhos = {10, 100, 1000, 5000};
      cout << "Analisando eficiência do cache..." << endl;
    cout << "Tamanho | Cache | Ratio Cache/(n1×n2) | Eficiência" << endl;
    cout << string(60, '-') << endl;
    
    for (int tamanho : tamanhos) {
        ResultadoPerformance resultado = executarTestePerformance(tamanho, tamanho, 
                                                                 tamanho * 3, 
                                                                 tamanho * 5);
        
        double produto = (double)tamanho * tamanho; // n1 × n2 para árvores iguais
        double ratioCache = resultado.tamanhoCache / produto;
          string eficiencia;
        if (ratioCache < 0.01) eficiencia = "Excelente";
        else if (ratioCache < 0.05) eficiencia = "Boa";
        else if (ratioCache < 0.1) eficiencia = "Regular";
        else eficiencia = "Baixa";
        
        cout << setw(7) << tamanho << " | " 
             << setw(5) << resultado.tamanhoCache << " | "
             << setw(18) << fixed << setprecision(5) << ratioCache << " | "
             << eficiencia << endl;
    }
    
    cout << string(60, '-') << endl;
    cout << endl;
}

/**
 * @brief Teste específico para medir escalabilidade
 */
void testeEscalabilidade() {
    cout << "=== TESTE DE ESCALABILIDADE ===" << endl;
    cout << endl;
    
    vector<int> tamanhosCriticos = {60, 70, 80, 90, 100};
    
    cout << "Testando limites de escalabilidade..." << endl;
    cout << string(60, '-') << endl;
    
    for (int tamanho : tamanhosCriticos) {
        cout << "Testando árvores de tamanho " << tamanho << "... ";
        cout.flush();
        
        auto inicio = high_resolution_clock::now();
        
        try {
            ResultadoPerformance resultado = executarTestePerformance(tamanho, tamanho, 
                                                                     tamanho * 11, 
                                                                     tamanho * 17);
            
            auto fim = high_resolution_clock::now();
            auto duracaoTotal = duration_cast<milliseconds>(fim - inicio);
            
            cout << "OK (" << duracaoTotal.count() << " ms total)" << endl;
            cout << "  Tempo algoritmo: " << fixed << setprecision(1) 
                 << resultado.tempoExecucaoMs << " ms" << endl;
            cout << "  Cache: " << resultado.tamanhoCache << " entradas" << endl;
            cout << "  Memória: " << setprecision(1) << resultado.memoriaUtilizada << " KB" << endl;
            
        } catch (const exception& e) {
            cout << "ERRO: " << e.what() << endl;
        } catch (...) {
            cout << "ERRO: Exceção desconhecida" << endl;
        }
        
        cout << endl;
    }
    
    cout << string(60, '-') << endl;
}

/**
 * @brief Teste de consistência dos resultados
 */
void testeConsistencia() {
    cout << "=== TESTE DE CONSISTÊNCIA DOS RESULTADOS ===" << endl;
    cout << endl;
    
    int tamanhoTeste = 1000;
    
    cout << "Verificando consistência com múltiplas execuções..." << endl;
    cout << "Tamanho das árvores: " << tamanhoTeste << endl;
    cout << string(50, '-') << endl;
    
    // Testar mesmas árvores múltiplas vezes para verificar consistência
    vector<double> resultados;
    vector<double> tempos;
    
    for (int i = 0; i < 5; i++) {
        // Criar árvores aleatórias (mesmo seed para resultados iguais)
        Arvore arvore1(criarArvoreAleatoria(tamanhoTeste, 42));
        Arvore arvore2(criarArvoreAleatoria(tamanhoTeste, 84));
        
        CalculadorDeCustos calculador(1.0, 1.0, 1.0); // Sempre custos unitários
        
        auto inicio = high_resolution_clock::now();
        TED ted(arvore1, arvore2, calculador);
        double custo = ted.obterCusto();
        auto fim = high_resolution_clock::now();
        
        auto duracao = duration_cast<microseconds>(fim - inicio);
        
        resultados.push_back(custo);
        tempos.push_back(duracao.count() / 1000.0);
        
        cout << "Execução " << (i+1) << ": Custo = " << fixed << setprecision(1) 
             << custo << ", Tempo = " << setprecision(2) << tempos.back() << " ms" << endl;
    }
    
    // Verificar consistência
    bool consistente = true;
    double custoReferencia = resultados[0];
    
    for (double custo : resultados) {
        if (abs(custo - custoReferencia) > 0.001) {
            consistente = false;
            break;
        }
    }
    
    cout << string(50, '-') << endl;
    cout << "Consistência dos resultados: " << (consistente ? "OK" : "ERRO") << endl;
    
    if (!tempos.empty()) {
        double tempoMedio = 0.0;
        for (double t : tempos) tempoMedio += t;
        tempoMedio /= tempos.size();
        
        cout << "Tempo médio de execução: " << fixed << setprecision(2) 
             << tempoMedio << " ms" << endl;
    }
    
    cout << endl;
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
      // Executar todos os testes de performance
    try {
        
        // 1. Análise principal de complexidade
        executarTestesComplexidade();
        
        // 2. Testes com diferentes proporções
        executarTestesVariados();
        
        // 3. Análise do comportamento do cache
        analisarComportamentoCache();
        
        // 4. Teste de consistência dos resultados
        testeConsistencia();
        
    } catch (const exception& e) {
        cout << "ERRO durante execução: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
