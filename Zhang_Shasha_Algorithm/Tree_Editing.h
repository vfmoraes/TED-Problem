#ifndef TREE_EDITING

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

        Tree_Editing(vector<vector<int>> tree_dist, vector<vector<int>> forest_dist);
         
        int tree_dist_calc(Tree T1, Tree T2);
        int comput_tree_dist(int index1, int index2);
        

        int interval_calc(int li, int i);
        void build_tree_matrix(int T1_size, int T2_size);
        void build_forest_matrix(int T1_size, int T2_size);
        void Tree_Editing::initialize_matrix(std::vector<std::vector<int>>& matrix, int T1_size, int T2_size);
};

#endif //TREE_EDITING