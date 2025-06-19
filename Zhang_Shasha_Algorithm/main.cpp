#include "Tree.h"

#include <iostream>

using namespace std;

// Node class implementation
Node::Node(char label, int li, int walking_index) : label(label), li(li), walking_index(walking_index) {};

void Node::add_child(Node *child_pointer)
{
    children.push_back(child_pointer);
}

// Tree class implementation
Tree::Tree(Node *root) : root(root) {};

Node *Tree::get_root()
{
    return root;
}

vector<Node *> Tree::get_indices()
{
    return indices;
}

void Tree::post_order(Node *current_node, int &counter)
{
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

    if (current_node->children.empty())
    {
        current_node->li = current_node->walking_index;
    }
    else
    {
        current_node->li = li;
    }

    indices.push_back(current_node);
}

void print_postorder(Node* node)
{
    if (!node)
        return;
    for (Node* child : node->children)
        print_postorder(child);
    cout << "Nó: " << node->label << ", walk_index: " << node->walking_index << ", Li: " << node->li << endl;
}

// Função para imprimir os nós em pós-ordem
void print_indices(const vector<Node *> &indices)
{
    cout << "Índices pós-ordem salvos na árvore:" << endl;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        const Node *node = indices[i];
        if (node)
            cout << "Posição " << i << ": Nó " << node->label
                 << ", walk_index: " << node->walking_index
                 << ", Li: " << node->li << endl;
    }
}

int main()
{
    // Criação dos nós (labels, Li e walk_index serão definidos depois)
    Node node_a = Node('a', -1, -1);
    Node node_b = Node('b', -1, -1);
    Node node_c = Node('c', -1, -1);
    Node node_f = Node('f', -1, -1);
    Node node_e = Node('e', -1, -1);
    Node node_d = Node('d', -1, -1);

    // Montagem da subárvore c
    node_c.add_child(&node_a); // a à esquerda de c
    node_c.add_child(&node_b); // b à direita de c

    // Montagem da subárvore e
    node_e.add_child(&node_d); // f à esquerda de e

    // Montagem da raiz d
    node_f.add_child(&node_c); // c à esquerda de d
    node_f.add_child(&node_e); // e à direita de d

    // Criação da árvore
    Tree tree(&node_f);

    // Pós-ordem para atribuir walk_index e Li
    int counter = 1;
    tree.post_order(tree.get_root(), counter);

    // Impressão dos nós em pós-ordem
    std::cout << "Nós em pós-ordem:" << std::endl;
    print_postorder(tree.get_root());
    print_indices(tree.get_indices());

    return 0;
}