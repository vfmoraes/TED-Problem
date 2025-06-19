#include "Tree_Editing.h"
#include <algorithm> // Para reverse() e min()
#include <iomanip> // Para setw()

// Função auxiliar para imprimir matrizes de distância
void print_tree_editing_matrix(const vector<vector<int>>& matrix, const vector<Node*>& nodes1, const vector<Node*>& nodes2, const string& title) {
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

// Função auxiliar para imprimir keyroots
void print_tree_editing_keyroots(const vector<Node*>& keyroots, const string& title) {
    cout << title << endl;
    for (Node* n : keyroots) {
        if (n)
            cout << "Nó: " << n->label << ", walking_index: " << n->walking_index 
                 << ", li: " << n->li << endl;
    }
}

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
    Node* n1 = get_node1(index1);
    Node* n2 = get_node2(index2);
    int li = n1->li;
    int lj = n2->li;
    int rows = interval_calc(li, index1);
    int cols = interval_calc(lj, index2);
    forest_dist.resize(rows + 1, vector<int>(cols + 1, 0));
    forest_dist[0][0] = 0;
    for (int di = 1; di <= rows; di++) {
        forest_dist[di][0] = forest_dist[di-1][0] + remove_cost;
    }
    for (int dj = 1; dj <= cols; dj++) {
        forest_dist[0][dj] = forest_dist[0][dj-1] + add_cost;
    }
    for (int di = 1; di <= rows; di++) {
        for (int dj = 1; dj <= cols; dj++) {
            int node_i_idx = li + di - 1;
            int node_j_idx = lj + dj - 1;
            Node* ni = get_node1(node_i_idx);
            Node* nj = get_node2(node_j_idx);
            if (ni->li == li && nj->li == lj) {
                int update_cost = (ni->label == nj->label) ? 0 : rename_cost;
                int del_cost = forest_dist[di-1][dj] + remove_cost;
                int ins_cost = forest_dist[di][dj-1] + add_cost;
                int upd_cost = forest_dist[di-1][dj-1] + update_cost;
                forest_dist[di][dj] = std::min(del_cost, std::min(ins_cost, upd_cost));
                tree_dist[ni->walking_index+1][nj->walking_index+1] = forest_dist[di][dj];
            } else {
                int del_cost = forest_dist[di-1][dj] + remove_cost;
                int ins_cost = forest_dist[di][dj-1] + add_cost;
                int sub_cost = forest_dist[ni->li - li][nj->li - lj] + 
                               tree_dist[ni->walking_index+1][nj->walking_index+1];
                forest_dist[di][dj] = std::min(del_cost, std::min(ins_cost, sub_cost));
            }
        }
    }
    return forest_dist[rows][cols];
}

int Tree_Editing::tree_dist_calc(Tree T1, Tree T2) {
    nodes1 = T1.get_indices();
    nodes2 = T2.get_indices();
    vector<Node*> keyroots1 = T1.get_LR_keyroots();
    vector<Node*> keyroots2 = T2.get_LR_keyroots();
    reverse(keyroots1.begin(), keyroots1.end());
    reverse(keyroots2.begin(), keyroots2.end());
    tree_dist.resize(nodes1.size() + 1, vector<int>(nodes2.size() + 1, 0));
    for (Node* n1 : keyroots1) {
        for (Node* n2 : keyroots2) {
            int i = n1->walking_index;
            int j = n2->walking_index;
            comput_tree_dist(i, j);
        }
    }
    return tree_dist[nodes1.back()->walking_index+1][nodes2.back()->walking_index+1];
}
