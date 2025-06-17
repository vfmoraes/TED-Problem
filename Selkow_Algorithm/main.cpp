#include "ted.h"

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

// ========== TESTES DO ALGORITMO DE SELKOW (TED) ==========

void testarTEDBasico() {
    cout << "=== TESTE TED 1: Caso Básico ===" << endl;
    
    // Árvore A1: A -> C
    auto raiz1 = criarNo("A");
    raiz1->adicionarFilho(criarNo("C"));
    Arvore a1(move(raiz1));
    
    // Árvore A2: A -> D
    auto raiz2 = criarNo("A");
    raiz2->adicionarFilho(criarNo("D"));
    Arvore a2(move(raiz2));
    
    cout << "Árvore A1:" << endl;
    cout << a1 << endl;
    
    cout << "Árvore A2:" << endl;
    cout << a2 << endl;
    
    CalculadorDeCustos calculador(1.0, 1.0, 1.0);
    TED ted(a1, a2, calculador);
    
    ted.imprimirMatrizCusto();
    cout << "Custo TED esperado: 1 (C->D)" << endl;
    cout << "Custo TED calculado: " << ted.obterCusto() << endl;
    cout << endl;
}

void testarTEDArvoreIdentica() {
    cout << "=== TESTE TED 2: Árvores Idênticas ===" << endl;
    
    // Ambas árvores: A -> B -> C
    auto raiz1 = criarNo("A");
    auto filho1 = criarNo("B");
    filho1->adicionarFilho(criarNo("C"));
    raiz1->adicionarFilho(move(filho1));
    Arvore a1(move(raiz1));
    
    auto raiz2 = criarNo("A");
    auto filho2 = criarNo("B");
    filho2->adicionarFilho(criarNo("C"));
    raiz2->adicionarFilho(move(filho2));
    Arvore a2(move(raiz2));
    
    cout << "Ambas árvores são idênticas:" << endl;
    cout << a1 << endl;
    
    CalculadorDeCustos calculador(1.0, 1.0, 1.0);
    TED ted(a1, a2, calculador);
    
    ted.imprimirMatrizCusto();
    cout << "Custo TED esperado: 0 (árvores idênticas)" << endl;
    cout << "Custo TED calculado: " << ted.obterCusto() << endl;
    cout << endl;
}

void testarTEDArvoreVazia() {
    cout << "=== TESTE TED 3: Uma Árvore Vazia ===" << endl;
    
    // Árvore A1: apenas A
    auto raiz1 = criarNo("A");
    Arvore a1(move(raiz1));
    
    // Árvore A2: A -> B -> C
    auto raiz2 = criarNo("A");
    auto filho2 = criarNo("B");
    filho2->adicionarFilho(criarNo("C"));
    raiz2->adicionarFilho(move(filho2));
    Arvore a2(move(raiz2));
    
    cout << "Árvore A1 (simples):" << endl;
    cout << a1 << endl;
    
    cout << "Árvore A2 (complexa):" << endl;
    cout << a2 << endl;
    
    CalculadorDeCustos calculador(1.0, 1.0, 1.0);
    TED ted(a1, a2, calculador);
    
    ted.imprimirMatrizCusto();
    cout << "Custo TED (inserir B e C): " << ted.obterCusto() << endl;
    cout << endl;
}

void testarTEDArvoreCompleta() {
    cout << "=== TESTE TED 4: Árvores Complexas ===" << endl;
    
    // Árvore A1: A -> {B -> D, C}
    auto raiz1 = criarNo("A");
    auto filhoB = criarNo("B");
    filhoB->adicionarFilho(criarNo("D"));
    raiz1->adicionarFilho(move(filhoB));
    raiz1->adicionarFilho(criarNo("C"));
    Arvore a1(move(raiz1));
    
    // Árvore A2: A -> {B -> E, F}
    auto raiz2 = criarNo("A");
    auto filhoB2 = criarNo("B");
    filhoB2->adicionarFilho(criarNo("E"));
    raiz2->adicionarFilho(move(filhoB2));
    raiz2->adicionarFilho(criarNo("F"));
    Arvore a2(move(raiz2));
    
    cout << "Árvore A1:" << endl;
    cout << a1 << endl;
    
    cout << "Árvore A2:" << endl;
    cout << a2 << endl;
    
    CalculadorDeCustos calculador(1.0, 1.0, 1.0);
    TED ted(a1, a2, calculador);
    
    ted.imprimirMatrizCusto();
    cout << "Operações necessárias: D->E, C->F" << endl;
    cout << "Custo TED calculado: " << ted.obterCusto() << endl;
    cout << endl;
}

void testarTEDCustosPersonalizados() {
    cout << "=== TESTE TED 5: Custos Personalizados ===" << endl;
    
    // Árvore A1: A -> B
    auto raiz1 = criarNo("A");
    raiz1->adicionarFilho(criarNo("B"));
    Arvore a1(move(raiz1));
    
    // Árvore A2: A -> C
    auto raiz2 = criarNo("A");
    raiz2->adicionarFilho(criarNo("C"));
    Arvore a2(move(raiz2));
    
    cout << "Árvore A1:" << endl;
    cout << a1 << endl;
    
    cout << "Árvore A2:" << endl;
    cout << a2 << endl;
    
    // Teste com custos diferentes
    CalculadorDeCustos calculador1(2.0, 3.0, 1.5); // inserção, deleção, rotulação
    TED ted1(a1, a2, calculador1);
    
    cout << "Com custos personalizados (ins:2.0, del:3.0, rot:1.5):" << endl;
    ted1.imprimirMatrizCusto();
    cout << "Custo TED: " << ted1.obterCusto() << endl;
    
    CalculadorDeCustos calculador2(1.0, 1.0, 1.0); // rotulação cara
    TED ted2(a1, a2, calculador2);
    
    cout << "\nCom rotulação cara (ins:1.0, del:1.0, rot:1.0):" << endl;
    ted2.imprimirMatrizCusto();
    cout << "Custo TED: " << ted2.obterCusto() << endl;
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
    cout << "    TESTES DO ALGORITMO DE SELKOW" << endl;
    cout << "        Tree Edit Distance (TED)" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    testarTEDBasico();
    testarTEDArvoreIdentica();
    testarTEDArvoreVazia();
    testarTEDArvoreCompleta();
    testarTEDCustosPersonalizados();

    cout << "========================================" << endl;
    cout << "           TESTES COMPLETOS" << endl;
    cout << "========================================" << endl;
    
    return 0;
}