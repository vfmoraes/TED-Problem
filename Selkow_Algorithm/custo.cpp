#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <utility>
#include <cmath>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include <ctime>
#include "arvore.h"
#include "custo.h"

using namespace std;

// Implementações dos métodos da classe CalculadorDeCustos

// Construtor
CalculadorDeCustos::CalculadorDeCustos(double custoIns, double custoDel, double custoRot) 
    : custoInsercaoBasico(custoIns), custoDelecaoBasico(custoDel), custoRotulacaoBasico(custoRot) {
}

// Método privado para cálculo recursivo de inserção
double CalculadorDeCustos::calcularCustoInsercaoRecursivo(const No* no) const {
    if (no == nullptr) return 0.0;
    
    auto it = cacheInsercao.find(no);
    if (it != cacheInsercao.end()) {
        return it->second;
    }
    
    double custoTotal = custoInsercaoUnico(no);
    
    for (const auto& filho : no->filhos) {
        custoTotal += calcularCustoInsercaoRecursivo(filho.get());
    }
    
    cacheInsercao[no] = custoTotal;
    return custoTotal;
}

// Método privado para cálculo recursivo de deleção
double CalculadorDeCustos::calcularCustoDelecaoRecursivo(const No* no) const {
    if (no == nullptr) return 0.0;
    
    auto it = cacheDelecao.find(no);
    if (it != cacheDelecao.end()) {
        return it->second;
    }
    
    double custoTotal = custoDelecaoUnico(no);
    
    for (const auto& filho : no->filhos) {
        custoTotal += calcularCustoDelecaoRecursivo(filho.get());
    }
    
    cacheDelecao[no] = custoTotal;
    return custoTotal;
}

// Implementação dos métodos públicos
double CalculadorDeCustos::custoInsercaoUnico(const No* no) const {
    if (no == nullptr) return 0.0;
    double fatorRotulo = 1.0; //(no->rotulo.length() * 0.1);
    return custoInsercaoBasico * fatorRotulo;
}

double CalculadorDeCustos::custoDelecaoUnico(const No* no) const {
    if (no == nullptr) return 0.0;
    double fatorRotulo = 1.0;  //+ (no->rotulo.length() * 0.1);
    return custoDelecaoBasico * fatorRotulo;
}

/*double CalculadorDeCustos::custoRotulacao(const string& rotuloOrigem, const string& rotuloDestino) const {
        if (rotuloOrigem == rotuloDestino) {
            return 0.0;
        }
        
        auto chave = make_pair(rotuloOrigem, rotuloDestino);
        auto it = cacheRotulacao.find(chave);
        if (it != cacheRotulacao.end()) {
            return it->second;
        }
        
        double custo = custoRotulacaoBasico;
        
        int diferencaComprimento = abs(static_cast<int>(rotuloOrigem.length()) - 
                                     static_cast<int>(rotuloDestino.length()));
        custo += diferencaComprimento * 0.5;
        
        int caracteresComuns = 0;
        int maxComprimento = max(rotuloOrigem.length(), rotuloDestino.length());
        
        for (size_t i = 0; i < min(rotuloOrigem.length(), rotuloDestino.length()); ++i) {
            if (rotuloOrigem[i] == rotuloDestino[i]) {
                caracteresComuns++;
            }
        }        
        if (maxComprimento > 0) {
            double similaridade = static_cast<double>(caracteresComuns) / maxComprimento;
            custo *= (2.0 - similaridade);
        }
        
        cacheRotulacao[chave] = custo;
        return custo;
}*/

double CalculadorDeCustos::custoRotulacao(const string& rotuloOrigem, const string& rotuloDestino) const {
    if (rotuloOrigem == rotuloDestino) {
        return 0.0;
    }
    
    // Verificar cache
    auto chave = make_pair(rotuloOrigem, rotuloDestino);
    auto it = cacheRotulacao.find(chave);
    if (it != cacheRotulacao.end()) {
        return it->second;
    }
    
    // Calcular distância de Levenshtein
    double distanciaLevenshtein = calcularDistanciaLevenshtein(rotuloOrigem, rotuloDestino);
    
    // Custo baseado na distância de Levenshtein multiplicado pelo custo básico
    double custo = custoRotulacaoBasico * distanciaLevenshtein;
    
    // Armazenar no cache
    cacheRotulacao[chave] = custo;
    return custo;
}

double CalculadorDeCustos::calcularDistanciaLevenshtein(const string& s1, const string& s2) const {
    const size_t len1 = s1.size();
    const size_t len2 = s2.size();
    
    // Caso especial: strings vazias
    if (len1 == 0) return static_cast<double>(len2);
    if (len2 == 0) return static_cast<double>(len1);
    
    // Matriz para programação dinâmica
    vector<vector<int>> dp(len1 + 1, vector<int>(len2 + 1));
    
    // Inicializar primeira linha e coluna
    for (size_t i = 0; i <= len1; ++i) {
        dp[i][0] = static_cast<int>(i);
    }
    for (size_t j = 0; j <= len2; ++j) {
        dp[0][j] = static_cast<int>(j);
    }
    
    // Preencher a matriz
    for (size_t i = 1; i <= len1; ++i) {
        for (size_t j = 1; j <= len2; ++j) {
            // Custo de substituição (0 se caracteres são iguais, 1 se diferentes)
            int custoSubstituicao = (s1[i-1] == s2[j-1]) ? 0 : 1;
            
            dp[i][j] = min({
                dp[i-1][j] + 1,          // Deleção
                dp[i][j-1] + 1,          // Inserção
                dp[i-1][j-1] + custoSubstituicao  // Substituição
            });
        }
    }
    
    return static_cast<double>(dp[len1][len2]);
}

double CalculadorDeCustos::custoInsercaoSubarvore(const No* no) const {
    return calcularCustoInsercaoRecursivo(no);
}

double CalculadorDeCustos::custoDelecaoSubarvore(const No* no) const {
    return calcularCustoDelecaoRecursivo(no);
}

void CalculadorDeCustos::limparCache() {
    cacheInsercao.clear();
    cacheDelecao.clear();
    cacheRotulacao.clear();
}

