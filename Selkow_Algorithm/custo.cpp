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
    double fatorRotulo = 1.0 + (no->rotulo.length() * 0.1);
    return custoInsercaoBasico * fatorRotulo;
}

double CalculadorDeCustos::custoDelecaoUnico(const No* no) const {
    if (no == nullptr) return 0.0;
    double fatorRotulo = 1.0 + (no->rotulo.length() * 0.1);
    return custoDelecaoBasico * fatorRotulo;
}

double CalculadorDeCustos::custoRotulacao(const string& rotuloOrigem, const string& rotuloDestino) const {
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

// ========== FUNÇÕES DE TESTE ==========

// Teste 1: Custos básicos
void testarCustosBasicos() {
    cout << "=== TESTE 1: Custos Básicos ===" << endl;
    
    CalculadorDeCustos calculador(1.0, 1.0, 1.0);
    
    // Criar nó simples
    auto no = criarNo("A");
    
    cout << "Nó 'A':" << endl;
    cout << "  Custo inserção único: " << calculador.custoInsercaoUnico(no.get()) << endl;
    cout << "  Custo deleção único: " << calculador.custoDelecaoUnico(no.get()) << endl;
    cout << "  Custo inserção subárvore: " << calculador.custoInsercaoSubarvore(no.get()) << endl;
    cout << "  Custo deleção subárvore: " << calculador.custoDelecaoSubarvore(no.get()) << endl;
    
    cout << "\nTestes de rotulação:" << endl;
    cout << "  A -> A: " << calculador.custoRotulacao("A", "A") << " (deve ser 0)" << endl;
    cout << "  A -> B: " << calculador.custoRotulacao("A", "B") << endl;
    cout << "  ABC -> XYZ: " << calculador.custoRotulacao("ABC", "XYZ") << endl;
    cout << "  Test -> Testing: " << calculador.custoRotulacao("Test", "Testing") << endl;
    cout << endl;
}

// Teste 2: Árvore com subárvores
void testarArvoreComSubarvores() {
    cout << "=== TESTE 2: Árvore com Subárvores ===" << endl;
    
    // Criar árvore: A -> B(D, E), C
    auto noD = criarNo("D");
    auto noE = criarNo("E");
    
    auto noB = criarNo("B");
    noB->adicionarFilho(move(noD));
    noB->adicionarFilho(move(noE));
    
    auto noC = criarNo("C");
    
    auto noA = criarNo("A");
    noA->adicionarFilho(move(noB));
    noA->adicionarFilho(move(noC));
    
    Arvore arvore(move(noA));
    
    cout << "Árvore de teste:" << endl;
    cout << arvore << endl;
    
    CalculadorDeCustos calculador;
    
    // Testar custos para cada nó
    vector<const No*> nos = arvore.obterNosEmPosOrdem();
    
    cout << "Análise de custos por nó:" << endl;
    for (const No* no : nos) {
        cout << "Nó '" << no->rotulo << "':" << endl;
        cout << "  Inserção única: " << calculador.custoInsercaoUnico(no) << endl;
        cout << "  Deleção única: " << calculador.custoDelecaoUnico(no) << endl;
        cout << "  Inserção subárvore: " << calculador.custoInsercaoSubarvore(no) << endl;
        cout << "  Deleção subárvore: " << calculador.custoDelecaoSubarvore(no) << endl;
        cout << "  É folha: " << (arvore.ehFolha(no) ? "Sim" : "Não") << endl;
        cout << endl;
    }
}

// Teste 3: Validação das propriedades do algoritmo de Selkow
void testarPropriedadesSankoff() {
    cout << "=== TESTE 3: Propriedades do Algoritmo de Selkow ===" << endl;
    
    CalculadorDeCustos calculador;
    
    // Teste da propriedade: CL(si, si) = 0
    cout << "Propriedade CL(si, si) = 0:" << endl;
    cout << "  A -> A: " << calculador.custoRotulacao("A", "A") << endl;
    cout << "  Test -> Test: " << calculador.custoRotulacao("Test", "Test") << endl;
    
    // Teste da desigualdade triangular (aproximada)
    cout << "\nTeste de consistência (desigualdade triangular):" << endl;
    string s1 = "A", s2 = "B", s3 = "C";
    double c12 = calculador.custoRotulacao(s1, s2);
    double c23 = calculador.custoRotulacao(s2, s3);
    double c13 = calculador.custoRotulacao(s1, s3);
    
    cout << "  " << s1 << " -> " << s2 << ": " << c12 << endl;
    cout << "  " << s2 << " -> " << s3 << ": " << c23 << endl;
    cout << "  " << s1 << " -> " << s3 << ": " << c13 << endl;
    cout << "  Desigualdade triangular: " << c13 << " <= " << (c12 + c23) 
         << " ? " << (c13 <= c12 + c23 ? "OK" : "FALHOU") << endl;
    cout << endl;
}

// Teste 4: Performance e cache
void testarPerformanceCache() {
    cout << "=== TESTE 4: Performance e Cache ===" << endl;
    
    // Criar árvore maior
    srand(42); // Seed fixo para reproduzibilidade
    
    auto raiz = criarNo("0");
    vector<No*> nosDisponiveis;
    nosDisponiveis.push_back(raiz.get());
    
    // Criar árvore com 10 nós
    for (int i = 1; i < 10; ++i) {
        auto novoNo = criarNo(to_string(i));
        No* pai = nosDisponiveis[rand() % nosDisponiveis.size()];
        pai->adicionarFilho(move(novoNo));
        nosDisponiveis.push_back(pai->filhos.back().get());
    }
    
    Arvore arvore(move(raiz));
    
    cout << "Árvore de teste (10 nós):" << endl;
    cout << arvore << endl;
    
    CalculadorDeCustos calculador;
    
    // Primeiro cálculo (sem cache)
    clock_t inicio = clock();
    double custoInsercao = calculador.custoInsercaoSubarvore(arvore.obterNoRaiz());
    clock_t fim = clock();
    
    cout << "Primeiro cálculo (sem cache):" << endl;
    cout << "  Custo inserção total: " << custoInsercao << endl;
    cout << "  Tempo: " << (double)(fim - inicio) / CLOCKS_PER_SEC << "s" << endl;
    
    // Segundo cálculo (com cache)
    inicio = clock();
    double custoInsercao2 = calculador.custoInsercaoSubarvore(arvore.obterNoRaiz());
    fim = clock();
    
    cout << "Segundo cálculo (com cache):" << endl;
    cout << "  Custo inserção total: " << custoInsercao2 << endl;
    cout << "  Tempo: " << (double)(fim - inicio) / CLOCKS_PER_SEC << "s" << endl;
    cout << "  Resultados iguais: " << (custoInsercao == custoInsercao2 ? "OK" : "ERRO") << endl;
    cout << endl;
}

// Teste 5: Casos extremos
void testarCasosExtremos() {
    cout << "=== TESTE 5: Casos Extremos ===" << endl;
    
    CalculadorDeCustos calculador;
    
    // Teste com nó nulo
    cout << "Teste com nó nulo:" << endl;
    cout << "  custoInsercaoUnico(nullptr): " << calculador.custoInsercaoUnico(nullptr) << endl;
    cout << "  custoDelecaoUnico(nullptr): " << calculador.custoDelecaoUnico(nullptr) << endl;
    cout << "  custoInsercaoSubarvore(nullptr): " << calculador.custoInsercaoSubarvore(nullptr) << endl;
    cout << "  custoDelecaoSubarvore(nullptr): " << calculador.custoDelecaoSubarvore(nullptr) << endl;
    
    // Teste com strings vazias
    cout << "\nTeste com strings vazias:" << endl;
    cout << "  '' -> '': " << calculador.custoRotulacao("", "") << endl;
    cout << "  '' -> 'A': " << calculador.custoRotulacao("", "A") << endl;
    cout << "  'A' -> '': " << calculador.custoRotulacao("A", "") << endl;
    
    // Teste com strings muito longas
    cout << "\nTeste com strings longas:" << endl;
    string longa1(100, 'A');
    string longa2(100, 'B');
    cout << "  String longa A -> String longa B: " << calculador.custoRotulacao(longa1, longa2) << endl;
    
    cout << endl;
}

int main() {
    cout << "========================================" << endl;
    cout << "    TESTE DO SISTEMA DE CUSTOS" << endl;
    cout << "      Algoritmo de Selkow" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    testarCustosBasicos();
    testarArvoreComSubarvores();
    testarPropriedadesSankoff();
    testarPerformanceCache();
    testarCasosExtremos();
    
    cout << "========================================" << endl;
    cout << "           TESTES COMPLETOS" << endl;
    cout << "========================================" << endl;
    
    return 0;
}