#ifndef TREE_EDITING
#define TREE_EDITING

#include <iostream>
#include <vector>
#include "Tree.h"

using namespace std;

class Tree_Editing {
public:
    vector<vector<int>> tree_dist;
    vector<vector<int>> forest_dist;
    const int remove_cost = 1;
    const int add_cost = 1;
    const int rename_cost = 1;

    Tree* t1;
    Tree* t2;
    vector<Node*> nodes1;
    vector<Node*> nodes2;

    Tree_Editing(Tree* t1, Tree* t2);

    int tree_dist_calc(Tree T1, Tree T2);
    int comput_tree_dist(int index1, int index2);

    Node* get_node1(int index) {
        if (index < 0 || index >= nodes1.size()) {
            cerr << "Index out of bounds for nodes1: " << index << endl;
            return nullptr;
        }
        return nodes1[index];
    }
    Node* get_node2(int index) {
        if (index < 0 || index >= nodes2.size()) {
            cerr << "Index out of bounds for nodes2: " << index << endl;
            return nullptr;
        }
        return nodes2[index];
    }

    void initialize_matrix(std::vector<std::vector<int>>& matrix, int T1_size, int T2_size);
    int interval_calc(int li, int i);
    void build_tree_matrix(int T1_size, int T2_size);
    void build_forest_matrix(int T1_size, int T2_size);
};

// Funções auxiliares para impressão (após definição de Node)
void print_tree_editing_matrix(const vector<vector<int>>& matrix, const vector<Node*>& nodes1, const vector<Node*>& nodes2, const string& title);
void print_tree_editing_keyroots(const vector<Node*>& keyroots, const string& title);

#endif //TREE_EDITING