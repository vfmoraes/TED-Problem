#ifndef TED_H
#define TED_H

#include "arvore.h"
#include "custo.h"
#include <vector>
#include <iostream>

using namespace std;

class TED {
private:
    vector<vector<double>> matrizCusto;
    vector<const No*> nosA1;
    vector<const No*> nosA2;
    double custoFinal;
    
    double min(double a, double b, double c);

public:
    TED(const Arvore& a1, const Arvore& a2, const CalculadorDeCustos& calculador);
    
    double obterCusto() const;
    void imprimirMatrizCusto() const;
    const vector<vector<double>>& obterMatrizCusto() const;
};

#endif // TED_H