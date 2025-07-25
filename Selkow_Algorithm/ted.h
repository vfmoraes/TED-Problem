#ifndef TED_H
#define TED_H

#include "arvore.h"
#include "custo.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

class TED {
private:
    // Cache para memoização - evita recálculos
    mutable map<pair<const No*, const No*>, double> cache;
    
    const CalculadorDeCustos* calculador;
    const Arvore* arvore1;
    const Arvore* arvore2;
    mutable double custoFinal;
    
    // Variável para armazenar o somatório das proporções das matrizes
    mutable double espacoTotalMatrizes;
    
    // Estrutura para armazenar informações de debug
    mutable vector<vector<double>> ultimaMatrizCalculada;
    mutable vector<const No*> ultimaFlorestaA1;
    mutable vector<const No*> ultimaFlorestaA2;
    
    // Métodos auxiliares para o algoritmo de Selkow
    double selkowRecursivo(const No* a1, const No* a2) const;
    double min(double a, double b, double c) const;

public:
    TED(const Arvore& a1, const Arvore& a2, const CalculadorDeCustos& calculador);
    
    double obterCusto() const;
    double obterEspacoUtilizado() const;
    
    // Métodos para debug/análise
    void imprimirDetalhesCalculo() const;
    void imprimirMatrizesCustos() const;
};

#endif // TED_H