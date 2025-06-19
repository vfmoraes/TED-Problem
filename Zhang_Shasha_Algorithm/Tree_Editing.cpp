#include "Tree.h"
#include "Tree_Editing.h"

#include <iostream>
#include <algorithm>

using namespace std;

// =================== Tree Editing class implementation ===================
Tree_Editing::Tree_Editing(vector<vector<int>> tree_dist, vector<vector<int>> forest_dist) : tree_dist(tree_dist), forest_dist(forest_dist) {};

void Tree_Editing::initialize_matrix(std::vector<std::vector<int>>& matrix, int T1_size, int T2_size) {
    matrix.resize(T1_size);
    if (T1_size > 0 && T2_size > 0) {
        matrix[0].resize(T2_size);
        matrix[0][0] = 0;
        for (int i = 1; i < T1_size; i++) {
            matrix[i].resize(T2_size);
            matrix[i][0] = matrix[i-1][0] + remove_cost;
        }
        for (int j = 1; j < T2_size; j++) {
            matrix[0][j] = matrix[0][j-1] + remove_cost;
        }
    }
}

void Tree_Editing::build_tree_matrix(int T1_size, int T2_size) {
    initialize_matrix(tree_dist, T1_size, T2_size);
}

void Tree_Editing::build_forest_matrix(int T1_size, int T2_size){
    initialize_matrix(forest_dist, T1_size, T2_size);
}