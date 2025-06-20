#include "Tree_Editing.h"
#include <algorithm> // For reverse() and min()
#include <iomanip> // For setw()

// Utility function to print distance matrices
void printTreeEditingMatrix(const vector<vector<int>>& matrix, const vector<Node*>& nodes1, const vector<Node*>& nodes2, const string& title) {
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

Tree_Editing::Tree_Editing(Tree* tree1, Tree* tree2)
    : t1(tree1), t2(tree2) {
    // Initialize node vectors
    nodes1 = t1->get_indices();
    nodes2 = t2->get_indices();
    
    // Prepare matrices for distance calculation
    tree_dist.resize(nodes1.size() + 1, vector<int>(nodes2.size() + 1, 0));
    forest_dist.resize(nodes1.size() + 1, vector<int>(nodes2.size() + 1, 0));
}

int Tree_Editing::interval_calc(int li, int i) {
    return i >= li ? i - li + 1 : 0; // Ensure interval has correct size, with correct instances counted. If i < li, interval is empty.
}

// Primary implementation for tree distance computation
int Tree_Editing::computeTreeDistance(int index1, int index2) {
    Node* n1 = get_node1(index1);
    Node* n2 = get_node2(index2);
    int li = n1->li;
    int lj = n2->li;
    int rows = interval_calc(li, index1);
    int cols = interval_calc(lj, index2);
    
    // Clear and resize forest distance matrix
    forest_dist.clear();
    forest_dist.resize(rows + 1, vector<int>(cols + 1, 0));
    
    // Initialize forest distance matrix
    forest_dist[0][0] = 0;
    for (int di = 1; di <= rows; di++) {
        forest_dist[di][0] = forest_dist[di-1][0] + remove_cost;
    }
    for (int dj = 1; dj <= cols; dj++) {
        forest_dist[0][dj] = forest_dist[0][dj-1] + add_cost;
    }
    
    // Main dynamic programming computation
    for (int di = 1; di <= rows; di++) {
        for (int dj = 1; dj <= cols; dj++) {
            int node_i_idx = li + di - 1;
            int node_j_idx = lj + dj - 1;
            
            Node* ni = get_node1(node_i_idx);
            Node* nj = get_node2(node_j_idx);
            
            if (ni->li == li && nj->li == lj) {
                // Both nodes are leftmost leaves in their respective forests
                int update_cost = (ni->label == nj->label) ? 0 : rename_cost;
                int del_cost = forest_dist[di-1][dj] + remove_cost;
                int ins_cost = forest_dist[di][dj-1] + add_cost;
                int upd_cost = forest_dist[di-1][dj-1] + update_cost;
                
                forest_dist[di][dj] = std::min(del_cost, std::min(ins_cost, upd_cost));
                
                int ti = ni->walking_index+1;
                int tj = nj->walking_index+1;
                tree_dist[ti][tj] = forest_dist[di][dj];
            } else {
                // At least one node is not a leftmost leaf
                int del_cost = forest_dist[di-1][dj] + remove_cost;
                int ins_cost = forest_dist[di][dj-1] + add_cost;
                int sub_di = ni->li - li;
                int sub_dj = nj->li - lj;
                
                int ti = ni->walking_index+1;
                int tj = nj->walking_index+1;
                int sub_cost = forest_dist[sub_di][sub_dj] + tree_dist[ti][tj];
                
                forest_dist[di][dj] = std::min(del_cost, std::min(ins_cost, sub_cost));
            }
        }
    }
    return forest_dist[rows][cols];
}

// Legacy method name for backward compatibility
int Tree_Editing::comput_tree_dist(int index1, int index2) {
    return computeTreeDistance(index1, index2);
}

// Primary tree edit distance calculation
int Tree_Editing::treeEditDistance(Tree T1, Tree T2) {
    nodes1 = T1.get_indices();
    nodes2 = T2.get_indices();
    
    vector<Node*> keyroots1 = T1.get_LR_keyroots();
    vector<Node*> keyroots2 = T2.get_LR_keyroots();
    
    // Reverse keyroots for Zhang-Shasha algorithm
    reverse(keyroots1.begin(), keyroots1.end());
    reverse(keyroots2.begin(), keyroots2.end());
    
    // Initialize tree distance matrix
    tree_dist.resize(nodes1.size() + 1, vector<int>(nodes2.size() + 1, 0));
    
    // Initialize first row and column
    for (int i = 1; i <= nodes1.size(); ++i) {
        tree_dist[i][0] = tree_dist[i-1][0] + remove_cost;
    }
    for (int j = 1; j <= nodes2.size(); ++j) {
        tree_dist[0][j] = tree_dist[0][j-1] + add_cost;
    }
    
    // Compute distance for each pair of keyroots
    for (Node* n1 : keyroots1) {
        for (Node* n2 : keyroots2) {
            int i = n1->walking_index;
            int j = n2->walking_index;
            computeTreeDistance(i, j);
        }
    }
    
    return tree_dist[nodes1.back()->walking_index+1][nodes2.back()->walking_index+1];
}

// Legacy method name for backward compatibility
int Tree_Editing::tree_dist_calc(Tree T1, Tree T2) {
    return treeEditDistance(T1, T2);
}
