#include "ted.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

// Implementação PURA do algoritmo de Selkow para Tree Edit Distance

TED::TED(const Arvore& a1, const Arvore& a2, const CalculadorDeCustos& calc) 
    : calculador(&calc), arvore1(&a1), arvore2(&a2), custoFinal(0.0) {
    
    // Calcular a distância de edição entre as duas árvores
    const No* raiz1 = a1.obterNoRaiz();
    const No* raiz2 = a2.obterNoRaiz();
    
    custoFinal = selkowRecursivo(raiz1, raiz2);
}

double TED::selkowRecursivo(const No* a1, const No* a2) const {
    // Verificar cache primeiro
    auto chave = make_pair(a1, a2);
    auto it = cache.find(chave);
    if (it != cache.end()) {
        return it->second;
    }
    
    double resultado;
    
    // Caso base 1: uma árvore é nula (vazia)
    if (a1 == nullptr && a2 == nullptr) {
        resultado = 0.0;
    }
    else if (a1 == nullptr) {
        // Inserir toda a subárvore a2
        resultado = calculador->custoInsercaoSubarvore(a2);
    }
    else if (a2 == nullptr) {
        // Deletar toda a subárvore a1
        resultado = calculador->custoDelecaoSubarvore(a1);
    }    else {
        // Ambas as árvores não são nulas - aplicar algoritmo de Selkow
        
        vector<const No*> filhos1 = arvore1->obterFilhos(a1);
        vector<const No*> filhos2 = arvore2->obterFilhos(a2);
        
        // Algoritmo de Selkow: renomear raiz + editar florestas de filhos
        double custoRenomeacao = calculador->custoRotulacao(a1->rotulo, a2->rotulo);
        double custoFlorestas = calcularCustoFlorestas(filhos1, filhos2);
        
        resultado = custoRenomeacao + custoFlorestas;
    }
    
    // Armazenar no cache
    cache[chave] = resultado;
    return resultado;
}

double TED::calcularCustoFlorestas(const vector<const No*>& floresta1, 
                                  const vector<const No*>& floresta2) const {
    
    int m = floresta1.size();
    int n = floresta2.size();
    
    // Armazenar para debug
    ultimaFlorestaA1 = floresta1;
    ultimaFlorestaA2 = floresta2;
    
    // Matriz de programação dinâmica para florestas
    vector<vector<double>> dp(m + 1, vector<double>(n + 1, 0.0));
    
    // Inicialização: deletar todos os elementos da floresta1
    for (int i = 1; i <= m; i++) {
        dp[i][0] = dp[i-1][0] + calculador->custoDelecaoSubarvore(floresta1[i-1]);
    }
    
    // Inicialização: inserir todos os elementos da floresta2
    for (int j = 1; j <= n; j++) {
        dp[0][j] = dp[0][j-1] + calculador->custoInsercaoSubarvore(floresta2[j-1]);
    }
    
    // Preenchimento da matriz
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            // Opção 1: Deletar árvore i da floresta1
            double delecao = dp[i-1][j] + calculador->custoDelecaoSubarvore(floresta1[i-1]);
            
            // Opção 2: Inserir árvore j da floresta2
            double insercao = dp[i][j-1] + calculador->custoInsercaoSubarvore(floresta2[j-1]);
            
            // Opção 3: Editar árvore i em árvore j (CHAMADA RECURSIVA)
            double edicao = dp[i-1][j-1] + selkowRecursivo(floresta1[i-1], floresta2[j-1]);
            
            dp[i][j] = min(delecao, insercao, edicao);
        }
    }
    
    // Armazenar matriz para debug
    ultimaMatrizCalculada = dp;
    
    return dp[m][n];
}

double TED::min(double a, double b, double c) const {
    return std::min({a, b, c});
}

double TED::obterCusto() const {
    return custoFinal;
}

void TED::limparCache() {
    cache.clear();
}

void TED::imprimirDetalhesCalculo() const {
    cout << "=== Detalhes do Cálculo TED (Selkow PURO) ===" << endl;
    
    const No* raiz1 = arvore1->obterNoRaiz();
    const No* raiz2 = arvore2->obterNoRaiz();
    
    if (raiz1 == nullptr && raiz2 == nullptr) {
        cout << "Ambas as árvores são vazias: custo = 0" << endl;
        return;
    }
    
    if (raiz1 == nullptr) {
        cout << "Árvore A1 vazia, inserindo árvore A2: custo = " 
             << calculador->custoInsercaoSubarvore(raiz2) << endl;
        return;
    }
    
    if (raiz2 == nullptr) {
        cout << "Árvore A2 vazia, deletando árvore A1: custo = " 
             << calculador->custoDelecaoSubarvore(raiz1) << endl;
        return;
    }
    
    cout << "Comparando raízes: '" << raiz1->rotulo << "' vs '" << raiz2->rotulo << "'" << endl;
    cout << "Custo de rotulação: " << calculador->custoRotulacao(raiz1->rotulo, raiz2->rotulo) << endl;
    
    vector<const No*> filhos1 = arvore1->obterFilhos(raiz1);
    vector<const No*> filhos2 = arvore2->obterFilhos(raiz2);
    
    cout << "Floresta A1 tem " << filhos1.size() << " subárvores: ";
    for (size_t i = 0; i < filhos1.size(); ++i) {
        cout << filhos1[i]->rotulo;
        if (i < filhos1.size() - 1) cout << ", ";
    }
    cout << endl;
    
    cout << "Floresta A2 tem " << filhos2.size() << " subárvores: ";
    for (size_t i = 0; i < filhos2.size(); ++i) {
        cout << filhos2[i]->rotulo;
        if (i < filhos2.size() - 1) cout << ", ";
    }
    cout << endl;
      double custoFlorestas = calcularCustoFlorestas(filhos1, filhos2);
    cout << "Custo edição florestas: " << custoFlorestas << endl;
    
    double custoRenomeacao = calculador->custoRotulacao(raiz1->rotulo, raiz2->rotulo);
    double custoTotal = custoRenomeacao + custoFlorestas;
    
    cout << "Custo de renomeação da raiz: " << custoRenomeacao << endl;
    cout << "Custo total (renomeação + florestas): " << custoTotal << endl;
}

void TED::imprimirMatrizesCustos() const {
    cout << "=== MATRIZ DE CUSTOS DO ALGORITMO DE SELKOW (PURO) ===" << endl;
    
    if (ultimaMatrizCalculada.empty()) {
        cout << "Nenhuma matriz de florestas foi calculada ainda." << endl;
        return;
    }
    
    int m = ultimaFlorestaA1.size();
    int n = ultimaFlorestaA2.size();
    
    cout << "Última matriz calculada: (" << (m+1) << " x " << (n+1) << ")" << endl;
    cout << "Comparando florestas:" << endl;
    
    cout << "Floresta A1: ";
    if (m == 0) {
        cout << "(vazia)";
    } else {
        for (int i = 0; i < m; ++i) {
            cout << ultimaFlorestaA1[i]->rotulo;
            if (i < m - 1) cout << ", ";
        }
    }
    cout << endl;
    
    cout << "Floresta A2: ";
    if (n == 0) {
        cout << "(vazia)";
    } else {
        for (int j = 0; j < n; ++j) {
            cout << ultimaFlorestaA2[j]->rotulo;
            if (j < n - 1) cout << ", ";
        }
    }
    cout << endl << endl;
    
    // Imprimir cabeçalho das colunas
    cout << "     ∅    ";
    for (int j = 0; j < n; j++) {
        cout << setw(6) << ultimaFlorestaA2[j]->rotulo << " ";
    }
    cout << endl;
    
    // Imprimir linhas da matriz
    for (int i = 0; i <= m; i++) {
        // Cabeçalho da linha
        if (i == 0) {
            cout << "∅  ";
        } else {
            cout << setw(2) << ultimaFlorestaA1[i-1]->rotulo << " ";
        }
        
        // Valores da linha
        for (int j = 0; j <= n; j++) {
            cout << setw(6) << fixed << setprecision(1) << ultimaMatrizCalculada[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << "\nCusto final da edição de florestas: " << ultimaMatrizCalculada[m][n] << endl;
    cout << "======================================" << endl;
}

size_t TED::obterTamanhoCache() const {
    return cache.size();
}