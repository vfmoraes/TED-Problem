#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include "Tree.h"
#include "Tree_Editing.h"
#include <unordered_set>

using namespace std;

// Função para imprimir a matriz de distância em um formato legível
void print_matrix(const vector<vector<int>>& matrix, const vector<Node*>& nodes1, const vector<Node*>& nodes2, const string& title) {
    cout << "\n" << title << ":\n";
    cout << setw(8) << " ";
    cout << setw(10) << "∅";
    for (Node* node : nodes2) {
        cout << setw(8) << node->label;
    }
    cout << endl;
    cout << string(8 * (nodes2.size() + 2), '-') << endl;
    cout << setw(10) << "∅";
    for (size_t j = 0; j <= nodes2.size(); ++j) {
        cout << setw(8) << matrix[0][j];
    }
    cout << endl;
    for (size_t i = 0; i < nodes1.size(); ++i) {
        cout << setw(8) << nodes1[i]->label;
        for (size_t j = 0; j <= nodes2.size(); ++j) {
            cout << setw(8) << matrix[i+1][j];
        }
        cout << endl;
    }
    cout << endl;
}

// Função para imprimir os nós de uma árvore
void print_tree_nodes(Tree& tree, const string& title) {
    cout << title << endl;
    for (Node* node : tree.get_indices()) {
        cout << "Nó: " << node->label 
             << ", walking_index: " << node->walking_index
             << ", li: " << node->li << endl;
    }
}

// Função para imprimir keyroots
void print_keyroots(const vector<Node*>& keyroots, const string& title) {
    cout << title << endl;
    for (Node* n : keyroots) {
        if (n)
            cout << "Nó: " << n->label << ", walking_index: " << n->walking_index 
                 << ", li: " << n->li << endl;
    }
}

// Função para criar a árvore 1
Tree create_test_tree1(vector<Node*>& nodes) {
    Node* a1 = new Node('a', -1, -1);
    Node* b1 = new Node('b', -1, -1);
    Node* c1 = new Node('c', -1, -1);
    Node* d1 = new Node('d', -1, -1);
    Node* e1 = new Node('e', -1, -1);
    c1->add_child(d1);
    c1->add_child(e1);
    a1->add_child(b1);
    a1->add_child(c1);
    nodes = {a1, b1, c1, d1, e1};
    Tree tree1(a1);

    // Realiza a travessia pós-ordem para atribuir índices e valores Li
    int counter1 = 0;
    tree1.post_order(tree1.get_root(), counter1);
    // print_tree_nodes(tree1, "Árvore 1 (post-order):");
    
    // Encontra os keyroots da árvore 1
    int last_li1 = -1;
    tree1.find_keyroots(tree1.get_root(), last_li1);
    vector<Node*> keyroots1 = tree1.get_LR_keyroots();
    reverse(keyroots1.begin(), keyroots1.end());
    // print_keyroots(keyroots1, "\nKeyroots da Árvore 1 (após inversão):");

    return tree1;
}

// Função para criar a árvore 2
Tree create_test_tree2(vector<Node*>& nodes) {
    Node* a2 = new Node('a', -1, -1);
    Node* b2 = new Node('b', -1, -1);
    Node* f2 = new Node('f', -1, -1);
    a2->add_child(b2);
    a2->add_child(f2);
    nodes = {a2, b2, f2};

    Tree tree2(a2);

    // Realiza a travessia pós-ordem para atribuir índices e valores Li
    int counter2 = 0;
    tree2.post_order(tree2.get_root(), counter2);
    // print_tree_nodes(tree2, "\nÁrvore 2 (post-order):");

    // Encontra os keyroots da árvore 2
    int last_li2 = -1;
    tree2.find_keyroots(tree2.get_root(), last_li2);
    vector<Node*> keyroots2 = tree2.get_LR_keyroots();
    reverse(keyroots2.begin(), keyroots2.end());
    // print_keyroots(keyroots2, "\nKeyroots da Árvore 2 (após inversão):");
    return tree2;
}


// Função utilitária para criar um novo Node*
Node* createNode(const char& label) {
    return new Node(label, -1, -1);
}

// Função para criar uma árvore aleatória com numNos nós e semente seed
Tree createRandomTree(int numNos, int seed, bool debug) {
    if (numNos <= 0) {
        cerr << "Número de nós deve ser maior que zero." << endl;
        return Tree(nullptr); // Retorna uma árvore vazia
    }
    char charNo = 'a'; // Começar com 'a' para os rótulos dos nós
    srand(seed);
    vector<Node*> nosDisponiveis;
    // Criar nó raiz
    Node* raiz = createNode(charNo);
    charNo++;
    nosDisponiveis.push_back(raiz);
    // Adicionar nós restantes
    for (int i = 1; i < numNos; ++i) {
        Node* novoNo = createNode(charNo);
        charNo = charNo < 'z' ? charNo + 1 : 'a'; // Ciclar de 'z' para 'a'
        // Escolher pai aleatório
        Node* pai = nosDisponiveis[rand() % nosDisponiveis.size()];
        pai->add_child(novoNo);
        // Adicionar novo nó ao pool
        nosDisponiveis.push_back(novoNo);
    }
    
    Tree tree(raiz);
    // Realiza a travessia pós-ordem para atribuir índices e valores Li
    int counter = 0;
    tree.post_order(tree.get_root(), counter);

    if (debug) {
        print_tree_nodes(tree, "Árvore Aleatória (post-order):");
    }

    // Encontra os keyroots da árvore 2
    int last_li2 = -1;
    tree.find_keyroots(tree.get_root(), last_li2);
    vector<Node*> keyroots2 = tree.get_LR_keyroots();
    reverse(keyroots2.begin(), keyroots2.end());
    if (debug) {
        print_keyroots(keyroots2, "\nKeyroots da Árvore Aleatória (após inversão):");
    }
    return tree;
}

// Função para liberar memória dos nós
void free_nodes(vector<Node*>& nodes) {
    for (Node* n : nodes) delete n;
    nodes.clear();
}

// Classe modificada de Tree_Editing para mostrar as matrizes durante o cálculo
class DebugTreeEditing : public Tree_Editing {
public:
    DebugTreeEditing(Tree* t1, Tree* t2) : Tree_Editing(t1, t2) {}
    
    // Sobrescreva o método para adicionar impressão de debug    
    int comput_tree_dist(int index1, int index2) {
        cout << "Calculando distância entre subárvores enraizadas em " 
             << get_node1(index1)->label << " e " << get_node2(index2)->label << endl;
        
        // Obter os nós correspondentes aos índices
        Node* n1 = get_node1(index1);
        Node* n2 = get_node2(index2);
        
        // Obter os índices left-most leaf
        int li = n1->li;
        int lj = n2->li;
        
        cout << "Índice left-most leaf para " << n1->label << ": " << li << endl;
        cout << "Índice left-most leaf para " << n2->label << ": " << lj << endl;
        
        // Calcular os tamanhos das florestas
        int rows = interval_calc(li, index1);
        int cols = interval_calc(lj, index2);
        
        cout << "Tamanho da floresta 1: " << rows << endl;
        cout << "Tamanho da floresta 2: " << cols << endl;
        
        // Garantir que as matrizes têm o tamanho correto
        forest_dist.resize(rows + 1, vector<int>(cols + 1, 0));
        
        // Inicialização da matriz forest_dist
        forest_dist[0][0] = 0;
        for (int di = 1; di <= rows; di++) {
            forest_dist[di][0] = forest_dist[di-1][0] + remove_cost;
        }
        for (int dj = 1; dj <= cols; dj++) {
            forest_dist[0][dj] = forest_dist[0][dj-1] + add_cost;
        }        // Extrair os nós relevantes para esta subárvore
        vector<Node*> subnodes1;
        vector<Node*> subnodes2;
        
        // Preencher subnodes1 usando get_node1
        for (int i = li; i <= index1; i++) {
            subnodes1.push_back(get_node1(i));
        }
        
        // Preencher subnodes2 usando get_node2
        for (int j = lj; j <= index2; j++) {
            subnodes2.push_back(get_node2(j));
        }
        
        cout << "Matriz forest_dist inicializada:" << endl;
        print_matrix(forest_dist, subnodes1, subnodes2, "Forest Distance (Inicial)");

        // Cálculo principal da distância de edição
        for (int di = 1; di <= rows; di++) {
            for (int dj = 1; dj <= cols; dj++) {                
                int node_i_idx = li + di - 1;  // Correção: -1 para ajustar o índice
                int node_j_idx = lj + dj - 1;  // Correção: -1 para ajustar o índice

                Node* ni = get_node1(node_i_idx);
                Node* nj = get_node2(node_j_idx);
                if (ni->li == li && nj->li == lj) {
                    // Ambos são folhas mais à esquerda
                    int update_cost = (ni->label == nj->label) ? 0 : rename_cost;
                    int del_cost = forest_dist[di-1][dj] + remove_cost;      // Deleção
                    int ins_cost = forest_dist[di][dj-1] + add_cost;         // Inserção
                    int upd_cost = forest_dist[di-1][dj-1] + update_cost;    // Substituição
                    
                    // Usar min para cada par
                    forest_dist[di][dj] = std::min(del_cost, std::min(ins_cost, upd_cost));
                    
                    tree_dist[ni->walking_index+1][nj->walking_index+1] = forest_dist[di][dj]; //index +1 para pular o índice 0, correspondente ao vazio
                    
                    cout << "Nós " << ni->label << " e " << nj->label << " são folhas mais à esquerda." << endl;
                    cout << "Custo de atualização: " << update_cost << endl;
                    cout << "Custos: del=" << del_cost << ", ins=" << ins_cost << ", upd=" << upd_cost << endl;
                } else {
                    // Pelo menos um não é folha mais à esquerda
                    int del_cost = forest_dist[di-1][dj] + remove_cost;      // Deleção
                    int ins_cost = forest_dist[di][dj-1] + add_cost;         // Inserção
                    int sub_cost = forest_dist[ni->li - li][nj->li - lj] + 
                                   tree_dist[ni->walking_index+1][nj->walking_index+1];  // Operação de subárvore, 
                                                                                         //index +1 para pular o índice 0, correspondente ao vazio
                    
                    // Usar min para cada par
                    forest_dist[di][dj] = std::min(del_cost, std::min(ins_cost, sub_cost));
                    
                    cout << "Nós " << ni->label << " e " << nj->label << " não são ambos folhas mais à esquerda." << endl;
                    cout << "Usando tree_dist[" << ni->walking_index << "][" << nj->walking_index 
                         << "] = " << tree_dist[ni->walking_index][nj->walking_index] << endl;
                }
                
                cout << "forest_dist[" << di << "][" << dj << "] = " << forest_dist[di][dj] << endl;
            }
        }
        
        cout << "Matriz forest_dist final:" << endl;
        print_matrix(forest_dist, subnodes1, subnodes2, "Forest Distance (Final)");
        
        // Retorna a distância para a floresta completa
        return forest_dist[rows][cols];
    }
      int tree_dist_calc(Tree T1, Tree T2) {
        // Atualizar os vetores de nós com os das árvores recebidas
        nodes1 = T1.get_indices();
        nodes2 = T2.get_indices();
        
        // Obter os keyroots das árvores e invertê-los corretamente
        vector<Node*> keyroots1 = T1.get_LR_keyroots();
        vector<Node*> keyroots2 = T2.get_LR_keyroots();

        // Reverter os vetores de keyroots apenas uma vez aqui
        // Esta inversão é necessária pelo algoritmo de Zhang-Shasha
        reverse(keyroots1.begin(), keyroots1.end());
        reverse(keyroots2.begin(), keyroots2.end());
        
        print_keyroots(keyroots1, "\nKeyroots da Árvore 1:");
        print_keyroots(keyroots2, "\nKeyroots da Árvore 2:");

        // Preparar a matriz para a distância entre árvores
        tree_dist.resize(nodes1.size() + 1, vector<int>(nodes2.size() + 1, 0));

        // Para cada par de keyroots, calcular a distância entre as subárvores
        for (Node* n1 : keyroots1) {
            for (Node* n2 : keyroots2) {
                int i = n1->walking_index;
                int j = n2->walking_index;
                
                cout << "\n==============================================\n";
                cout << "Calculando distância entre keyroots " << n1->label 
                     << " (índice " << i << ") e " << n2->label 
                     << " (índice " << j << ")" << endl;
                
                // Calcular a distância entre as subárvores enraizadas em i e j
                int dist = comput_tree_dist(i, j);
                cout << "Distância calculada: " << dist << endl;
                cout << "==============================================\n";
            }
        }

        // Imprimir a matriz tree_dist final
        cout << "\nMatriz de distância final entre árvores:" << endl;
        print_matrix(tree_dist, nodes1, nodes2, "Distância entre Árvores");

        // Retornar a distância entre as árvores completas
        return tree_dist[nodes1.back()->walking_index][nodes2.back()->walking_index];
    }
};

// Definição da estrutura ResultadoPerformance
struct ResultadoPerformance {
    int tamanhoArvore1;
    int rootKeysArvore1;
    int tamanhoArvore2;
    int rootKeysArvore2;
    double tempoExecucaoMs;
    int distance;
};

/**
 * @brief Executa um teste de performance para um par de árvores
 */
ResultadoPerformance executarTestePerformance(int tamanho1, int tamanho2, int seed1, int seed2, bool debug = false) {
    // Criar árvores aleatórias
    Tree arvore1 = createRandomTree(tamanho1, seed1, debug);
    Tree arvore2 = createRandomTree(tamanho2, seed2, debug);

    auto inicio = std::chrono::high_resolution_clock::now();
    
    Tree_Editing ted(&arvore1, &arvore2);
    int distance = ted.tree_dist_calc(arvore1, arvore2);
    
    auto fim = std::chrono::high_resolution_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio);
    
    ResultadoPerformance resultado;
    resultado.tamanhoArvore1 = tamanho1;
    resultado.rootKeysArvore1 = arvore1.get_LR_keyroots().size();
    resultado.tamanhoArvore2 = tamanho2;
    resultado.rootKeysArvore2 = arvore2.get_LR_keyroots().size();
    resultado.tempoExecucaoMs = duracao.count() / 1000.0; // Converter para millisegundos
    resultado.distance = distance;
    
    return resultado;
}

/**
 * @brief Salva os resultados em um arquivo CSV para análise posterior
 */
void salvarResultadosCSV(const vector<ResultadoPerformance>& resultados, const string& nomeArquivo) {
    ofstream arquivo(nomeArquivo);
    
    if (!arquivo.is_open()) {
        cout << "Erro: Não foi possível criar o arquivo " << nomeArquivo << endl;
        return;
    }
    
    // Cabeçalho CSV
    arquivo << "TamanhoArvore1,RootKeysArvore1,TamanhoArvore2,RootKeysArvore2,TempoExecucaoMs,Distance\n";
    
    // Dados
    for (const auto& resultado : resultados) {
        arquivo << resultado.tamanhoArvore1 << ","
                << resultado.rootKeysArvore1 << ","
                << resultado.tamanhoArvore2 << ","
                << resultado.rootKeysArvore2 << ","
                << fixed << setprecision(4) << resultado.tempoExecucaoMs << ","
                << resultado.distance << "\n";
    }
    
    arquivo.close();
    cout << "Resultados salvos em: " << nomeArquivo << endl;
}

/**
 * @brief Função principal com menu de testes
 */
int main() {
    cout << "========================================" << endl;
    cout << "  ANÁLISE DE PERFORMANCE - ALGORITMO DE ZHANG E SHASHA" << endl;
    cout << "       Tree Edit Distance (TED)" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    vector<ResultadoPerformance> resultados;
    vector<int> tamanhos = {10, 100, 1000, 10000}; // Tamanhos das árvores a serem testadas
    
    cout << "Executando testes para diferentes tamanhos de árvores..." << endl;
    cout << "Tamanhos testados: 10, 100, 1000, 10000 nós" << endl;
    cout << string(60, '-') << endl;
    
    for (int tamanho : tamanhos) {
        cout << "Testando árvores de tamanho " << tamanho << "...";
        cout.flush();
        auto inicioTeste = std::chrono::high_resolution_clock::now();
            
        // Teste com árvores de mesmo tamanho
        ResultadoPerformance resultado = executarTestePerformance(tamanho, tamanho, 
                                                                 tamanho * 10, 
                                                                 tamanho * 20);
        resultados.push_back(resultado);
            
        auto fimTeste = std::chrono::high_resolution_clock::now();
        auto duracaoTeste = std::chrono::duration_cast<std::chrono::milliseconds>(fimTeste - inicioTeste);
            
        cout << " Concluído em " << duracaoTeste.count() << " ms" << endl;
        cout << "  Tamanho da árvore 1: " << resultado.tamanhoArvore1 
             << " (Root Keys lenght: " << resultado.rootKeysArvore1 << ")" << endl;
        cout << "  Tamanho da árvore 2: " << resultado.tamanhoArvore2 
             << " (Root Keys lenght: " << resultado.rootKeysArvore2 << ")" << endl;
        // Mostrar resultados básicos
        cout << "  Tempo algoritmo: " << fixed << setprecision(2) 
             << resultado.tempoExecucaoMs << " ms" << endl;
        cout << "  Distância de edição (TED): " << resultado.distance << endl;
        cout << endl;
        cout << endl;
    }
    
    cout << string(60, '-') << endl;
    
    // Salvar resultados em arquivo CSV
    salvarResultadosCSV(resultados, "resultados_complexidade.csv");
    
    cout << "========================================" << endl;
    cout << "           TESTES COMPLETOS" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    return 0;
}

// int main() {
//     // Criar árvores e armazenar ponteiros para liberar depois
//     vector<Node*> nodes1, nodes2;
//     Tree tree1 = create_test_tree1(nodes1);
//     Tree tree2 = create_test_tree2(nodes2);

//     Tree_Editing editor(&tree1, &tree2);
//     int distance = editor.tree_dist_calc(tree1, tree2);
//     cout << "\n==============================================\n";
//     cout << "Distância de edição entre as árvores: " << distance << endl;
//     cout << "==============================================\n";
//     free_nodes(nodes1);
//     free_nodes(nodes2);   
//     return 0;
// }

