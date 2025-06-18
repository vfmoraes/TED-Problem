#include "ted.h"

double TED::min(double a, double b, double c) {
    double minimo = a;
    if (b < minimo) minimo = b;
    if (c < minimo) minimo = c;
    return minimo;
}

TED::TED(const Arvore& a1, const Arvore& a2, const CalculadorDeCustos& calculador) {
    // Obter sequências pós-ordem ORIGINAIS
    vector<const No*> temp1 = a1.obterNosEmPosOrdem();
    vector<const No*> temp2 = a2.obterNosEmPosOrdem();
    
    // IMPORTANTE: Inverter para trabalhar com PREFIXOS (raiz primeiro)
    nosA1.clear();
    nosA2.clear();
    
    // Rearranjar em ordem DECRESCENTE: último elemento primeiro (raiz primeiro)
    for (int i = (int) temp1.size() - 1; i >= 0; i--) {
        nosA1.push_back(temp1[i]);
    }
    for (int i = (int) temp2.size() - 1; i >= 0; i--) {
        nosA2.push_back(temp2[i]);
    }
    
    int m = nosA1.size();  // Tamanho da árvore A1
    int n = nosA2.size();  // Tamanho da árvore A2
    
    // Matriz (m+1) x (n+1) - PREFIXOS CRESCENTES
    matrizCusto.resize(m + 1, vector<double>(n + 1, 0.0));
    
    // ALGORITMO CORRETO - TRABALHA COM PREFIXOS!
    // matrizCusto[i][j] = custo de transformar prefixo A1[0..i-1] em prefixo A2[0..j-1]
    
    // INICIALIZAÇÃO:
    // [0][0]: árvore vazia → árvore vazia = 0
    matrizCusto[0][0] = 0.0;
    
    // Primeira linha: árvore vazia → prefixos de A2 (só inserções)
    for (int j = 1; j <= n; j++) {
        matrizCusto[0][j] = matrizCusto[0][j-1] + calculador.custoInsercaoUnico(nosA2[j-1]); 
        //Alternativa
        //calculador.custoInsercaoSubarvore(nosA2[j-1]);
    }
    
    // Primeira coluna: prefixos de A1 → árvore vazia (só deleções)
    for (int i = 1; i <= m; i++) {
        matrizCusto[i][0] = matrizCusto[i-1][0] + calculador.custoDelecaoUnico(nosA1[i-1]);
        //Alternativa
        //calculador.custoDelecaoSubarvore(nosA1[i-1]);
    }
    
    // PREENCHIMENTO DA MATRIZ (esquerda→direita, cima→baixo):
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            // Custo de rotulação A1[i-1] → A2[j-1]
            double custoRotulacao = calculador.custoRotulacao(nosA1[i-1]->rotulo, nosA2[j-1]->rotulo);
            
            // Custo de deleção do nó A1[i-1]
            double custoDelecao = calculador.custoDelecaoUnico(nosA1[i-1]);
            
            // Custo de inserção do nó A2[j-1]
            double custoInsercao = calculador.custoInsercaoUnico(nosA2[j-1]);
            
            // Escolher a operação de menor custo
            matrizCusto[i][j] = min(
                matrizCusto[i-1][j-1] + custoRotulacao,  // Rotulação/manutenção
                matrizCusto[i-1][j] + custoDelecao,      // Deleção
                matrizCusto[i][j-1] + custoInsercao      // Inserção
            );
        }
    }
    
    // O resultado final está na posição [m][n] (árvores completas)
    custoFinal = matrizCusto[m][n];
}

double TED::obterCusto() const {
    return custoFinal;
}

void TED::imprimirMatrizCusto() const {
    cout << "Matriz de Custos TED:" << endl;
    
    // Cabeçalho das colunas (prefixos de A2)
    cout << "        ∅    ";
    for (int j = 0; j < (int)nosA2.size(); j++) {
        // Mostrar prefixo A2[0..j]
        for (int k = 0; k <= j; k++) {
            cout << nosA2[k]->rotulo;
            if (k < j) cout << ",";
        }
        cout << "   ";
    }
    cout << endl;
    
    // Linhas da matriz
    for (int i = 0; i <= (int)nosA1.size(); i++) {
        // Cabeçalho da linha (prefixo de A1)
        if (i == 0) {
            cout << "∅       ";
        } else {
            // Mostrar prefixo A1[0..i-1]
            for (int k = 0; k < i; k++) {
                cout << nosA1[k]->rotulo;
                if (k < i-1) cout << ",";
            }
            // Alinhamento
            if (i == 1) cout << "       ";
            else cout << "     ";
        }
        
        // Valores da linha
        for (int j = 0; j <= (int)nosA2.size(); j++) {
            cout << matrizCusto[i][j] << "    ";
        }
        cout << endl;
    }
}

const vector<vector<double>>& TED::obterMatrizCusto() const {
    return matrizCusto;
}