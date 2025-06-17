#ifndef CUSTO_H
#define CUSTO_H

#include "arvore.h"
#include <string>

struct PairHash {
    size_t operator()(const pair<string, string>& p) const {
        return hash<string>{}(p.first) ^ (hash<string>{}(p.second) << 1);
    }
};

class CalculadorDeCustos {
public:
    CalculadorDeCustos(double custoIns = 1.0, double custoDel = 1.0, double custoRot = 1.0);

    double custoInsercaoUnico(const No* no) const;
    double custoDelecaoUnico(const No* no) const;
    double custoRotulacao(const std::string& rotuloOrigem, const std::string& rotuloDestino) const;
    double calcularDistanciaLevenshtein(const std::string& s1, const std::string& s2) const;

    double custoInsercaoSubarvore(const No* no) const;
    double custoDelecaoSubarvore(const No* no) const;

    void limparCache();

private:
    mutable std::unordered_map<const No*, double> cacheInsercao;
    mutable std::unordered_map<const No*, double> cacheDelecao;
    mutable std::unordered_map<std::pair<std::string, std::string>, double, struct PairHash> cacheRotulacao;

    double custoInsercaoBasico;
    double custoDelecaoBasico;
    double custoRotulacaoBasico;

    double calcularCustoInsercaoRecursivo(const No* no) const;
    double calcularCustoDelecaoRecursivo(const No* no) const;
};

#endif // CUSTO_H