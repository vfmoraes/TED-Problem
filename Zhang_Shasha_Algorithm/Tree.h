#ifndef TREE_H
#define TREE_H

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

// Utility functions for printing (after Node definition)
void printTreeNodes(const vector<Node*>& nodes, const string& title);
void printTreeKeyroots(const vector<Node*>& keyroots, const string& title);

class Tree {
private:
    Node* root;
    vector<Node*> LR_keyroots;
    vector<Node*> indices;

public:
    explicit Tree(Node* root);
    Node* get_root();
    vector<Node*> get_LR_keyroots();
    vector<Node*> get_indices();
    Node* get_node(int index);
    void post_order(Node* current_node, int& counter);
    void find_keyroots(Node* current_node, int& last_li);
};

#endif  // TREE_H