#include "ted.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

// Implementação PURA do algoritmo de Selkow para Tree Edit Distance

TED::TED(const Arvore& a1, const Arvore& a2, const CalculadorDeCustos& calc) 
    : calculador(&calc), arvore1(&a1), arvore2(&a2), custoFinal(0.0), espacoTotalMatrizes(0.0) {
    
    // Calcular a distância de edição entre as duas árvores
    const No* raiz1 = a1.obterNoRaiz();
    const No* raiz2 = a2.obterNoRaiz();
    
    custoFinal = selkowRecursivo(raiz1, raiz2);
    
    // Multiplicar pelo tamanho de um double para obter o espaço em bytes
    espacoTotalMatrizes *= sizeof(double);
}

double TED::selkowRecursivo(const No* a1, const No* a2) const {
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
        resultado = calculador->custoDelecaoSubarvore(a1);    } else {    
        // Algoritmo de Selkow:
       vector<const No*> filhos1 = arvore1->obterFilhos(a1);
       vector<const No*> filhos2 = arvore2->obterFilhos(a2);
       int m = filhos1.size();
       int n = filhos2.size();

       // Contabilizar o espaço da matriz atual (linhas × colunas)
       espacoTotalMatrizes += (m + 1) * (n + 1);

       //Soma-se 1 às dimensões para incluir as raízes A1 e A2
       vector<vector<double>> matrizCustos(m + 1, vector<double>(n + 1, 0.0));

       //Inicializa a primeira posição da matriz com o custo de edição da raíz A para a raíz B
       matrizCustos[0][0] = calculador->custoRotulacao(a1->rotulo, a2->rotulo);

       // Inicializa a primeira coluna (deletando subárvores de A1)
         for (int i = 1; i <= m; i++) {
              matrizCustos[i][0] = matrizCustos[i-1][0] + calculador->custoDelecaoSubarvore(filhos1[i-1]);
         }

       // Inicializa a primeira linha (inserindo subárvores de A2)
         for (int j = 1; j <= n; j++) {
              matrizCustos[0][j] = matrizCustos[0][j-1] + calculador->custoInsercaoSubarvore(filhos2[j-1]);
         }

       // Preencheenchendo a matriz de custos
       for(int i = 1; i < m + 1; i++) {
           for(int j = 1; j < n + 1; j++) {
               // Opção 1: Deletar subárvore de A1
               double custoDelecao = matrizCustos[i][j - 1] + calculador->custoDelecaoSubarvore(filhos1[i - 1]);
               
               // Opção 2: Inserir subárvore de A2
               double custoInsercao = matrizCustos[i - 1][j] + calculador->custoInsercaoSubarvore(filhos2[j - 1]);
               
               // Opção 3: Editar subárvore de A1 para A2 (CHAMADA RECURSIVA)
               double custoEdicao = matrizCustos[i - 1][j - 1] + selkowRecursivo(filhos1[i - 1], filhos2[j - 1]);
               
               matrizCustos[i][j] = min(custoDelecao, custoInsercao, custoEdicao);
           }
       }
       resultado = matrizCustos[m][n];
    }
    
    return resultado;
}

double TED::min(double a, double b, double c) const {
    return std::min({a, b, c});
}

double TED::obterCusto() const {
    return custoFinal;
}

double TED::obterEspacoUtilizado() const {
    return espacoTotalMatrizes;
}


void TED::imprimirDetalhesCalculo() const {
    cout << "=== Detalhes do Cálculo TED (Selkow) ===" << endl;
    
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
             << calculador->custoDelecaoSubarvore(raiz1) << endl;        return;
    }
    
    cout << "Cenário em que ambas as raízes não são nulas:" << endl;
    cout << "Comparando raízes: '" << raiz1->rotulo << "' vs '" << raiz2->rotulo << "'" << endl;
    double custoRenomeacao = calculador->custoRotulacao(raiz1->rotulo, raiz2->rotulo);
    cout << "Custo de rotulação da raiz: " << custoRenomeacao << endl;
    
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
    }    cout << endl;
    
    // O algoritmo de Selkow executa uma chamada recursiva que constrói a matriz
    // matrizCustos[0][0] = custo de rotulação das raízes
    // matrizCustos[m][n] = resultado final da transformação
    double custoFinalSelkow = selkowRecursivo(raiz1, raiz2);
    cout << "Custo final do algoritmo de Selkow: " << custoFinalSelkow << endl;
    cout << "  (Matriz inicia com rotulação das raízes em [0][0], custo de inserção de A2 de [0][1] até [0][n] e custo de remoção de A1 de [1][0] até [m][0]. Resultado final encontra-se na posição [m][n])" << endl;
}

void TED::imprimirMatrizesCustos() const {
    cout << "=== ALGORITMO DE SELKOW (IMPLEMENTAÇÃO RECURSIVA) ===" << endl;
    
    cout << "O algoritmo de Selkow implementado funciona de forma recursiva:" << endl;
    cout << "1. Para cada par de subárvores com raíz em (a1, a2):" << endl;
    cout << "   - Cria matriz (m+1) x (n+1) onde m=|filhos(a1)|, n=|filhos(a2)|" << endl;
    cout << "   - matrizCustos[0][0] = custo rotulação(a1.rotulo, a2.rotulo)" << endl;
    cout << "   - Primeira linha: custos acumulados de inserção das subárvores de a2" << endl;
    cout << "   - Primeira coluna: custos acumulados de deleção das subárvores de a1" << endl;
    cout << "   - matrizCustos[i][j] = min(deleção, inserção, edição recursiva)" << endl;
    cout << endl;
    cout << "2. A recursão aplica este processo para cada subproblema gerando uma matriz para cada diagonal calculada" << endl;
    cout << "3. O resultado final, dado na posição [m][n] da primeira matriz, contém o custo mínimo de operações para transformar a árovre A1 em A2" << endl;
    cout << endl;
    cout << "==========================================" << endl;
    cout << "Custo final calculado: " << custoFinal << endl;
    cout << "==========================================" << endl;
}