#ifndef TESTE_H

#include <iostream>
#include <vector>

using namespace std;

class Node {
    public: 
        char label;
        int li, walking_index;
        vector<Node*> children;

        Node(char label, int li, int walk_index);

        void add_child(Node* childPointer);

};

class Tree {
    private:
        Node* root;
        vector<int> LR_keyroots;
        vector<Node*> indices;

    public:
        explicit Tree(Node* root);

        Node* get_root();

        vector<int> get_LR_keyroots();

        vector<Node*> get_indices();

        void post_order(Node* current_node, int& counter);

};




#endif  //TESTE_H