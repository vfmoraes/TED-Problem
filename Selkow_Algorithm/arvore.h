#ifndef ARVORE_H
#define ARVORE_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <unordered_map>
#include <cstdlib>
#include <ctime>

using namespace std;

/**
 * @brief Representa um único nó (ou vértice) em uma árvore.
 * Cada nó contém um dado (o rótulo) e ponteiros para seus descendentes diretos (os filhos).
 */
class No {
public:
    string rotulo;
    vector<unique_ptr<No>> filhos;

    /**
     * @brief Construtor que cria um nó com um rótulo específico.
     * @param rotuloDoNo O valor textual que este nó irá armazenar.
     */
    No(string rotuloDoNo);

    /**
     * @brief Adiciona um nó à lista de filhos deste nó.
     * A posse do ponteiro do filho é transferida para a lista de filhos.
     * @param ponteiroParaNoFilho O ponteiro inteligente para o nó a ser adicionado.
     */
    void adicionarFilho(unique_ptr<No> ponteiroParaNoFilho);
};

/**
 * @brief Representa a árvore como um todo, gerenciando o nó raiz e
 * fornecendo métodos úteis para interagir com a estrutura.
 */
class Arvore {
private:
    unique_ptr<No> noRaiz;

    // Funções Auxiliares Recursivas
     const No* encontrarNoPorRotuloRecursivamente(const No* noAtual, const string& rotulo) const;
    void obterNosEmPosOrdemRecursivamente(const No* noAtual, vector<const No*>& listaDeNos) const;
    void obterProfundidadesRecursivamente(const No* noAtual, int profundidadeAtual, unordered_map<const No*, int>& mapaDeProfundidades) const;
    int obterTamanhosDasSubarvoresRecursivamente(const No* noAtual, unordered_map<const No*, int>& mapaDeTamanhos) const;
    int obterProfundidadeDoNoRecursivamente(const No* noAtual, const No* noProcurado, int profundidadeAtual) const;
    int contarNosRecursivamente(const No* noAtual) const;

public:
    /**
     * @brief Constrói um objeto Arvore, tomando posse do nó raiz fornecido.
     * @param ponteiroParaNoRaiz O ponteiro inteligente para o nó que será a raiz da árvore.
     */
    explicit Arvore(unique_ptr<No> ponteiroParaNoRaiz);

    /**
     * @brief Fornece acesso de apenas leitura ao nó raiz da árvore.
     */
    const No* obterNoRaiz() const;

    /**
     * @brief Retorna um vetor com ponteiros para todos os nós da árvore em pós-ordem.
     * A ordem é: filhos da esquerda, filhos da direita, e por último o pai.
     */
    vector<const No*> obterNosEmPosOrdem() const;

    /**
     * @brief Retorna um mapa que associa cada nó à sua profundidade. A raiz tem profundidade 0.
     */
    unordered_map<const No*, int> obterProfundidades() const;

    /**
     * @brief Retorna um mapa que associa cada nó ao tamanho total de sua subárvore
     * (incluindo o próprio nó).
     */
    unordered_map<const No*, int> obterTamanhosDasSubarvores() const;

    /**
     * @brief Obtém a profundidade de um nó específico
     * @param no Ponteiro para o nó
     * @return Profundidade do nó (-1 se não encontrado)
     */
    int obterProfundidadeDoNo(const No* no) const;

    /**
     * @brief Conta o número total de nós na árvore
     * @return Número total de nós
     */
    int contarNos() const;

    /**
     * @brief Verifica se um nó é folha (não tem filhos)
     * @param no Ponteiro para o nó
     * @return true se é folha, false caso contrário
     */
    bool ehFolha(const No* no) const;

    /**
     * @brief Obtém os filhos de um nó específico
     * @param no Ponteiro para o nó
     * @return Vetor com ponteiros para os filhos do nó
     */
    vector<const No*> obterFilhos(const No* no) const;

    /**
     * @brief Obtém um nó específico pelo seu índice na ordenação pós-ordem
     * @param indice Índice do nó na lista pós-ordem (0-based)
     * @return Ponteiro para o nó ou nullptr se índice inválido
     */
    const No* obterNoPorIndicePosOrdem(int indice) const;

    /**
     * @brief Encontra um nó pelo seu rótulo na árvore
     * @param rotulo Rótulo do nó a ser encontrado
     * @return Ponteiro para o primeiro nó encontrado com esse rótulo ou nullptr se não encontrado
     */
    const No* encontrarNoPorRotulo(const string& rotulo) const;

    /**
     * @brief Obtém o índice de um nó na ordenação pós-ordem
     * @param no Ponteiro para o nó
     * @return Índice do nó na lista pós-ordem (-1 se não encontrado)
     */
    int obterIndicePosOrdem(const No* no) const;
};

// Funções utilitárias
void imprimirArvoreRecursivamente(ostream& streamDeSaida, const No* noAtual, const string& prefixoDeIndentacao, bool ehUltimoFilho);
ostream& operator<<(ostream& streamDeSaida, const Arvore& arvoreParaImprimir);
unique_ptr<No> criarNo(const string& rotulo);
unique_ptr<No> criarArvoreAleatoria(int numNos, int seed);

#endif // ARVORE_H