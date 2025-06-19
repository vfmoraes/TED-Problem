#include "Tree_Editing.h"
#include <algorithm> // Para reverse() e min()

Tree_Editing::Tree_Editing(Tree* tree1, Tree* tree2)
    : t1(tree1), t2(tree2) {
    // Inicialize os vetores de nós
    nodes1 = t1->get_indices();
    nodes2 = t2->get_indices();
    
    // Prepare as matrizes para cálculo de distância
    tree_dist.resize(nodes1.size() + 1, vector<int>(nodes2.size() + 1, 0));
    forest_dist.resize(nodes1.size() + 1, vector<int>(nodes2.size() + 1, 0));
}

int Tree_Editing::interval_calc(int li, int i) {
    return i >= li ? i - li + 1 : 0; // Ensure the interval is the correct size, with the correct instances counted. if i is less than li, the interval is empty..
}


void Tree_Editing::initialize_matrix(std::vector<std::vector<int>>& matrix, int T1_size, int T2_size) {
    matrix.resize(T1_size+1);
    if (T1_size > 0 && T2_size > 0) {
        matrix[0].resize(T2_size+1);
        matrix[0][0] = 0;
        for (int i = 1; i <= T1_size; i++) {
            matrix[i].resize(T2_size+1);
            matrix[i][0] = matrix[i-1][0] + remove_cost;
        }
        for (int j = 1; j <= T2_size; j++) {
            // Corrigido: use add_cost para inserções (era incorretamente remove_cost)
            matrix[0][j] = matrix[0][j-1] + add_cost;
        }
    }
}

void Tree_Editing::build_tree_matrix(int T1_size, int T2_size) {
    initialize_matrix(tree_dist, T1_size, T2_size);
}

void Tree_Editing::build_forest_matrix(int T1_size, int T2_size) {
    initialize_matrix(forest_dist, T1_size, T2_size);
}

int Tree_Editing::comput_tree_dist(int index1, int index2) {
    // Obter os nós correspondentes aos índices
    Node* n1 = nodes1[index1];
    Node* n2 = nodes2[index2];
    
    // Obter os índices left-most leaf
    int li = n1->li;
    int lj = n2->li;
    
    // Calcular os tamanhos das florestas
    int rows = interval_calc(li, index1);
    int cols = interval_calc(lj, index2);
    
    // Garantir que as matrizes têm o tamanho correto
    forest_dist.resize(rows + 1, vector<int>(cols + 1, 0));
    
    // Inicialização da matriz forest_dist
    forest_dist[0][0] = 0;
    for (int di = 1; di <= rows; di++) {
        forest_dist[di][0] = forest_dist[di-1][0] + remove_cost;
    }
    for (int dj = 1; dj <= cols; dj++) {
        forest_dist[0][dj] = forest_dist[0][dj-1] + add_cost;
    }

    // Cálculo principal da distância de edição
    for (int di = 1; di <= rows; di++) {
        for (int dj = 1; dj <= cols; dj++) {
            int node_i_idx = li + di - 1;  // Correção: -1 para ajustar o índice
            int node_j_idx = lj + dj - 1;  // Correção: -1 para ajustar o índice

            Node* ni = nodes1[node_i_idx];
            Node* nj = nodes2[node_j_idx];

            if (ni->li == li && nj->li == lj) {
                // Ambos são folhas mais à esquerda
                int update_cost = (ni->label == nj->label) ? 0 : rename_cost;
                forest_dist[di][dj] = std::min(
                    forest_dist[di-1][dj] + remove_cost,     // Deleção
                    std::min(
                        forest_dist[di][dj-1] + add_cost,        // Inserção
                        forest_dist[di-1][dj-1] + update_cost    // Substituição/correspondência
                    )
                );
                tree_dist[ni->walking_index][nj->walking_index] = forest_dist[di][dj];
                forest_dist[di][dj] = std::min(
                    forest_dist[di-1][dj] + remove_cost,      // Deleção
                    std::min(
                        forest_dist[di][dj-1] + add_cost,         // Inserção
                        forest_dist[ni->li - li][nj->li - lj] + 
                            tree_dist[ni->walking_index][nj->walking_index]  // Operação de subárvore
                    )
                );
            }
        }
    }
    
    // Retorna a distância para a floresta completa
    return forest_dist[rows][cols];
}

int Tree_Editing::tree_dist_calc(Tree T1, Tree T2) {
    // Atualizar os vetores de nós com os das árvores recebidas
    nodes1 = T1.get_indices();
    nodes2 = T2.get_indices();
      // Obter os keyroots das árvores
    vector<Node*> keyroots1 = T1.get_LR_keyroots();
    vector<Node*> keyroots2 = T2.get_LR_keyroots();
    
    // Reverter os keyroots se necessário
    reverse(keyroots1.begin(), keyroots1.end());
    reverse(keyroots2.begin(), keyroots2.end());

    // Preparar a matriz para a distância entre árvores
    tree_dist.resize(nodes1.size() + 1, vector<int>(nodes2.size() + 1, 0));

    // Para cada par de keyroots, calcular a distância entre as subárvores
    for (Node* n1 : keyroots1) {
        for (Node* n2 : keyroots2) {
            int i = n1->walking_index;
            int j = n2->walking_index;
            
            // Calcular a distância entre as subárvores enraizadas em i e j
            comput_tree_dist(i, j);
        }
    }

    // Retornar a distância entre as árvores completas
    return tree_dist[nodes1.back()->walking_index][nodes2.back()->walking_index];
}
