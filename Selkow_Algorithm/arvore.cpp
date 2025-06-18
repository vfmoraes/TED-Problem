#include "arvore.h"

// Implementações da classe No
No::No(string rotuloDoNo) : rotulo(move(rotuloDoNo)) {}

void No::adicionarFilho(unique_ptr<No> ponteiroParaNoFilho) {
    filhos.push_back(move(ponteiroParaNoFilho));
}

// Implementações da classe Arvore
Arvore::Arvore(unique_ptr<No> ponteiroParaNoRaiz) : noRaiz(move(ponteiroParaNoRaiz)) {}

const No* Arvore::obterNoRaiz() const {
    return noRaiz.get();
}

void Arvore::obterNosEmPosOrdemRecursivamente(const No* noAtual, vector<const No*>& listaDeNos) const {
    if (noAtual == nullptr) return;
    for (const unique_ptr<No>& ponteiroParaFilho : noAtual->filhos) {
        obterNosEmPosOrdemRecursivamente(ponteiroParaFilho.get(), listaDeNos);
    }
    listaDeNos.push_back(noAtual);
}

vector<const No*> Arvore::obterNosEmPosOrdem() const {
    vector<const No*> listaDeNosResultado;
    obterNosEmPosOrdemRecursivamente(noRaiz.get(), listaDeNosResultado);
    return listaDeNosResultado;
}

void Arvore::obterProfundidadesRecursivamente(const No* noAtual, int profundidadeAtual, unordered_map<const No*, int>& mapaDeProfundidades) const {
    if (noAtual == nullptr) return;
    mapaDeProfundidades[noAtual] = profundidadeAtual;
    for (const unique_ptr<No>& ponteiroParaFilho : noAtual->filhos) {
        obterProfundidadesRecursivamente(ponteiroParaFilho.get(), profundidadeAtual + 1, mapaDeProfundidades);
    }
}

unordered_map<const No*, int> Arvore::obterProfundidades() const {
    unordered_map<const No*, int> mapaNoParaProfundidade;
    obterProfundidadesRecursivamente(noRaiz.get(), 0, mapaNoParaProfundidade);
    return mapaNoParaProfundidade;
}

int Arvore::obterTamanhosDasSubarvoresRecursivamente(const No* noAtual, unordered_map<const No*, int>& mapaDeTamanhos) const {
    if (noAtual == nullptr) return 0;
    int tamanhoAcumulado = 1; // Começa em 1 para contar o próprio nó.
    for (const unique_ptr<No>& ponteiroParaFilho : noAtual->filhos) {
        tamanhoAcumulado += obterTamanhosDasSubarvoresRecursivamente(ponteiroParaFilho.get(), mapaDeTamanhos);
    }
    mapaDeTamanhos[noAtual] = tamanhoAcumulado;
    return tamanhoAcumulado;
}

unordered_map<const No*, int> Arvore::obterTamanhosDasSubarvores() const {
    unordered_map<const No*, int> mapaNoParaTamanho;
    obterTamanhosDasSubarvoresRecursivamente(noRaiz.get(), mapaNoParaTamanho);
    return mapaNoParaTamanho;
}

int Arvore::obterProfundidadeDoNoRecursivamente(const No* noAtual, const No* noProcurado, int profundidadeAtual) const {
    if (noAtual == nullptr) return -1;
    if (noAtual == noProcurado) return profundidadeAtual;
    
    for (const unique_ptr<No>& filho : noAtual->filhos) {
        int resultado = obterProfundidadeDoNoRecursivamente(filho.get(), noProcurado, profundidadeAtual + 1);
        if (resultado != -1) return resultado;
    }
    return -1;
}

int Arvore::obterProfundidadeDoNo(const No* no) const {
    return obterProfundidadeDoNoRecursivamente(noRaiz.get(), no, 0);
}

int Arvore::contarNosRecursivamente(const No* noAtual) const {
    if (noAtual == nullptr) return 0;
    int contador = 1; // Conta o nó atual
    for (const unique_ptr<No>& filho : noAtual->filhos) {
        contador += contarNosRecursivamente(filho.get());
    }
    return contador;
}

int Arvore::contarNos() const {
    return contarNosRecursivamente(noRaiz.get());
}

bool Arvore::ehFolha(const No* no) const {
    return no != nullptr && no->filhos.empty();
}

// Funções utilitárias globais
void imprimirArvoreRecursivamente(ostream& streamDeSaida, const No* noAtual, const string& prefixoDeIndentacao, bool ehUltimoFilho) {
    if (noAtual == nullptr) return;

    // Imprime a linha do nó atual
    streamDeSaida << prefixoDeIndentacao << (ehUltimoFilho ? "└── " : "├── ") << noAtual->rotulo << "\n";

    // Prepara o prefixo para os nós filhos
    const string novoPrefixo = prefixoDeIndentacao + (ehUltimoFilho ? "    " : "│   ");
    const vector<unique_ptr<No>>& filhosDoNoAtual = noAtual->filhos;
    for (size_t i = 0; i < filhosDoNoAtual.size(); ++i) {
        imprimirArvoreRecursivamente(streamDeSaida, filhosDoNoAtual[i].get(), novoPrefixo, i == filhosDoNoAtual.size() - 1);
    }
}

ostream& operator<<(ostream& streamDeSaida, const Arvore& arvoreParaImprimir) {
    const No* ponteiroParaRaiz = arvoreParaImprimir.obterNoRaiz();
    if (ponteiroParaRaiz == nullptr) {
        streamDeSaida << "[A arvore esta vazia]\n";
        return streamDeSaida;
    }

    streamDeSaida << ponteiroParaRaiz->rotulo << "\n";
    const vector<unique_ptr<No>>& filhosDaRaiz = ponteiroParaRaiz->filhos;
    for (size_t i = 0; i < filhosDaRaiz.size(); ++i) {
        imprimirArvoreRecursivamente(streamDeSaida, filhosDaRaiz[i].get(), "", i == filhosDaRaiz.size() - 1);
    }
    return streamDeSaida;
}

unique_ptr<No> criarNo(const string& rotulo) {
    return make_unique<No>(rotulo);
}

unique_ptr<No> criarArvoreAleatoria(int numNos, int seed) {
    int numeroNo = 0;
    if (numNos <= 0) {
        return nullptr;
    }
    
    //srand(time(nullptr)); // Seed para números aleatórios
    srand(seed);
    vector<No*> nosDisponiveis; // Pool de nós que podem receber filhos
    
    // Criar nó raiz
    auto raiz = criarNo(to_string(numeroNo)); 
    numeroNo++;
    nosDisponiveis.push_back(raiz.get());
    
    // Adicionar nós restantes
    for (int i = 1; i < numNos; ++i) {
        auto novoNo = criarNo(to_string(numeroNo));
        numeroNo++;
        // Escolher pai aleatório
        No* pai = nosDisponiveis[rand() % nosDisponiveis.size()];
        pai->adicionarFilho(move(novoNo));
        
        // Adicionar novo nó ao pool (usar o ponteiro antes do move)
        nosDisponiveis.push_back(pai->filhos.back().get());
    }
    
    return raiz;
}

/**
 * @brief Função principal que demonstra o uso da estrutura de árvore
 */
// int main() {
//     cout << "=== Demonstracao da Estrutura de Arvore ===" << endl;
//     cout << endl;
    
//     // Criando os nós folha
//     auto noE = criarNo("E");
//     auto noF = criarNo("F");
//     auto noG = criarNo("G");
//     auto noH = criarNo("H");
    
//     // Criando os nós intermediários
//     auto noB = criarNo("B");
//     noB->adicionarFilho(move(noE));
//     noB->adicionarFilho(move(noF));
    
//     auto noC = criarNo("C");
    
//     auto noD = criarNo("D");
//     noD->adicionarFilho(move(noG));
//     noD->adicionarFilho(move(noH));
    
//     // Criando a raiz e montando a árvore
//     auto noA = criarNo("A");
//     noA->adicionarFilho(move(noB));
//     noA->adicionarFilho(move(noC));
//     noA->adicionarFilho(move(noD));
    
//     // Criando o objeto árvore
//     Arvore arvore(move(noA));
    
//     // Imprimindo a árvore
//     cout << "Estrutura da arvore:" << endl;
//     cout << arvore << endl;
    
//     // Demonstrando funcionalidades
//     cout << "=== Analise da Arvore ===" << endl;
    
//     // Obtendo nós em pós-ordem
//     vector<const No*> nosEmPosOrdem = arvore.obterNosEmPosOrdem();
//     cout << "Nos em pos-ordem: ";
//     for (size_t i = 0; i < nosEmPosOrdem.size(); ++i) {
//         cout << nosEmPosOrdem[i]->rotulo;
//         if (i < nosEmPosOrdem.size() - 1) cout << " -> ";
//     }
//     cout << endl;
    
//     // Obtendo profundidades
//     auto profundidades = arvore.obterProfundidades();
//     cout << "Profundidades dos nos:" << endl;
//     for (const No* no : nosEmPosOrdem) {
//         cout << "  " << no->rotulo << ": profundidade " << profundidades[no] << endl;
//     }
    
//     // Obtendo tamanhos das subárvores
//     auto tamanhos = arvore.obterTamanhosDasSubarvores();
//     cout << "Tamanhos das subarvores:" << endl;
//     for (const No* no : nosEmPosOrdem) {
//         cout << "  Subarvore de " << no->rotulo << ": " << tamanhos[no] << " nos" << endl;
//     }
    
//     cout << endl;
//     cout << "=== Fim da Demonstracao ===" << endl;
    
//     // Demonstração da árvore aleatória
//     cout << endl;
//     cout << "=== Arvore Aleatoria ===" << endl;
    
//     auto arvoreAleatoria = criarArvoreAleatoria(6);
    
//     if (arvoreAleatoria) {
//         Arvore arvoreRandom(move(arvoreAleatoria));
//         cout << "Estrutura da arvore aleatoria:" << endl;
//         cout << arvoreRandom << endl;
        
//         // Demonstrar funções úteis para Selkow
//         cout << "=== Funcoes Uteis para Selkow ===" << endl;
//         cout << "Total de nos: " << arvoreRandom.contarNos() << endl;
        
//         // Testar profundidade de um nó específico
//         const No* raiz = arvoreRandom.obterNoRaiz();
//         if (raiz) {
//             cout << "Raiz eh folha? " << (arvoreRandom.ehFolha(raiz) ? "Sim" : "Nao") << endl;
//         }
//     }
    
//     return 0;
// }