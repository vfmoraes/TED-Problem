#include "Tree.h"

#include <iostream>
#include <algorithm>
#include <unordered_set>

using namespace std;

// Utility function to print tree nodes
void printTreeNodes(const vector<Node*>& nodes, const string& title) {
    cout << title << endl;
    for (const Node* node : nodes) {
        cout << "Node: " << node->label 
             << ", walking_index: " << node->walking_index
             << ", li: " << node->li << endl;
    }
}

// Utility function to print keyroots
void printTreeKeyroots(const vector<Node*>& keyroots, const string& title) {
    cout << title << endl;
    for (const Node* n : keyroots) {
        if (n)
            cout << "Node: " << n->label << ", walking_index: " << n->walking_index 
                 << ", li: " << n->li << endl;
    }
}

// =================== Node class implementation ===================

/**
* @brief Constructs a Node object.
* @param label The character label of the node.
* @param li The Li index of the node.
* @param walking_index The post-order traversal index of the node.
*/
Node::Node(char label, int li, int walking_index) : label(label), li(li), walking_index(walking_index) {};

/**
* @brief Adds a child node to this node.
* @param child_pointer Pointer to the child node to be added.
*/
void Node::add_child(Node *child_pointer) {
   children.push_back(child_pointer);
}

// =================== Tree class implementation ===================

/**
* @brief Constructs a Tree object with a given root node.
* @param root Pointer to the root node of the tree.
*/
Tree::Tree(Node *root) : root(root) {};

/**
* @brief Returns the pointer to the root node of the tree.
* @return Pointer to the root node.
*/
Node *Tree::get_root() {
   return root;
}

/**
* @brief Returns the vector of nodes indexed in post-order.
* @return Vector of pointers to the nodes.
*/
vector<Node *> Tree::get_indices() {
   return indices;
}

/**
* @brief Returns a node by its index with bounds checking.
* @param index The index of the node to retrieve.
* @return Pointer to the node, or nullptr if index is out of bounds.
*/
Node* Tree::get_node(int index) {
   if (index < 0 || index >= indices.size()) {
       cerr << "Index out of bounds: " << index << endl;
       return nullptr; // or throw an exception
   }
   return indices[index];
}

/**
* @brief Returns the vector of keyroots in the tree.
* @return Vector of pointers to the keyroot nodes.
*/
vector<Node*> Tree::get_LR_keyroots() { 
   return LR_keyroots;
}

/**
* @brief Performs post-order traversal, assigning indices and Li values to nodes.
* @param current_node Pointer to the current node being visited.
* @param counter Reference to the counter used for assigning traversal indices.
*/
void Tree::post_order(Node *current_node, int &counter) {
   if (current_node == nullptr)
       return;

   int li = -1;

   for (size_t i = 0; i < current_node->children.size(); ++i)
   {
       Node *child = current_node->children[i];
       post_order(child, counter);
       if (i == 0 && child != nullptr)
       {
           li = child->li;
       }
   }

   current_node->walking_index = counter++;

   current_node->li = (current_node->children.empty()) ? current_node->li = current_node->walking_index : current_node->li = li;

   indices.push_back(current_node);
}

/**
* @brief Finds the keyroots in the tree and stores them in LR_keyroots.
* @param current_node Pointer to the current node being visited.
* @param last_li Reference to the last Li value found.
*/
void Tree::find_keyroots(Node *current_node, int &last_li) {
   if (current_node == nullptr) return;

   if (current_node->li != last_li) {
       LR_keyroots.push_back(current_node);
       last_li = current_node->li;
   }

   for ( Node* child : current_node->children) {
       find_keyroots(child, last_li);
   }
   
   // Reversal should not be done here within recursion
   // It should be done once after all recursive calls
}