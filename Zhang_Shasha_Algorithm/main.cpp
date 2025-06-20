#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
#include "Tree.h"
#include "Tree_Editing.h"
#include <unordered_set>

using namespace std;

// Print distance matrix in readable format
void printMatrix(const vector<vector<int>>& matrix, const vector<Node*>& nodes1, const vector<Node*>& nodes2, const string& title) {
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

// Print tree nodes with their indices and leftmost leaf values
void printTreeNodes(Tree& tree, const string& title) {
    cout << title << endl;
    for (Node* node : tree.get_indices()) {
        cout << "Node: " << node->label 
             << ", walking_index: " << node->walking_index
             << ", li: " << node->li << endl;
    }
}

// Print keyroots nodes
void printKeyroots(const vector<Node*>& keyroots, const string& title) {    cout << title << endl;
    for (Node* n : keyroots) {
        if (n)
            cout << "Node: " << n->label << ", walking_index: " << n->walking_index 
                 << ", li: " << n->li << endl;
    }
}

// Create test tree 1
Tree createTestTree1(vector<Node*>& nodes) {
    Node* a1 = new Node('a', -1, -1);
    Node* b1 = new Node('b', -1, -1);
    Node* c1 = new Node('c', -1, -1);
    Node* d1 = new Node('d', -1, -1);
    Node* e1 = new Node('e', -1, -1);
    c1->add_child(d1);
    c1->add_child(e1);
    a1->add_child(b1);
    a1->add_child(c1);
    nodes = {a1, b1, c1, d1, e1};
    Tree tree1(a1);

    // Perform post-order traversal to assign indices and Li values
    int counter1 = 0;
    tree1.post_order(tree1.get_root(), counter1);
    // printTreeNodes(tree1, "Tree 1 (post-order):");
    
    // Find keyroots of tree 1
    int last_li1 = -1;
    tree1.find_keyroots(tree1.get_root(), last_li1);
    vector<Node*> keyroots1 = tree1.get_LR_keyroots();
    reverse(keyroots1.begin(), keyroots1.end());
    // printKeyroots(keyroots1, "\nTree 1 Keyroots (after reversal):");

    return tree1;
}

// Create test tree 2
Tree createTestTree2(vector<Node*>& nodes) {
    Node* a2 = new Node('a', -1, -1);
    Node* b2 = new Node('b', -1, -1);
    Node* f2 = new Node('f', -1, -1);
    a2->add_child(b2);
    a2->add_child(f2);
    nodes = {a2, b2, f2};

    Tree tree2(a2);

    // Perform post-order traversal to assign indices and Li values
    int counter2 = 0;
    tree2.post_order(tree2.get_root(), counter2);
    // printTreeNodes(tree2, "\nTree 2 (post-order):");

    // Find keyroots of tree 2
    int last_li2 = -1;
    tree2.find_keyroots(tree2.get_root(), last_li2);
    vector<Node*> keyroots2 = tree2.get_LR_keyroots();
    reverse(keyroots2.begin(), keyroots2.end());
    // printKeyroots(keyroots2, "\nTree 2 Keyroots (after reversal):");
    return tree2;
}


// Utility function to create a new Node*
Node* createNode(const char& label) {
    return new Node(label, -1, -1);
}

// Create random tree with numNodes nodes and given seed
Tree createRandomTree(int numNodes, int seed, bool debug) {
    if (numNodes <= 0) {
        cerr << "Number of nodes must be greater than zero." << endl;
        return Tree(nullptr); // Return empty tree
    }
    char nodeChar = 'a'; // Start with 'a' for node labels
    srand(seed);
    vector<Node*> availableNodes;
    // Create root node
    Node* root = createNode(nodeChar);
    nodeChar++;
    availableNodes.push_back(root);
    // Add remaining nodes
    for (int i = 1; i < numNodes; ++i) {
        Node* newNode = createNode(nodeChar);
        nodeChar = nodeChar < 'z' ? nodeChar + 1 : 'a'; // Cycle from 'z' to 'a'
        // Choose random parent
        Node* parent = availableNodes[rand() % availableNodes.size()];
        parent->add_child(newNode);
        // Add new node to pool
        availableNodes.push_back(newNode);
    }
    
    Tree tree(root);
    // Perform post-order traversal to assign indices and Li values
    int counter = 0;
    tree.post_order(tree.get_root(), counter);

    if (debug) {
        printTreeNodes(tree, "Random Tree (post-order):");
    }
    
    // Find keyroots of tree
    int last_li2 = -1;
    tree.find_keyroots(tree.get_root(), last_li2);
    vector<Node*> keyroots2 = tree.get_LR_keyroots();
    reverse(keyroots2.begin(), keyroots2.end());
    if (debug) {
        printKeyroots(keyroots2, "\nRandom Tree Keyroots (after reversal):");
    }
    return tree;
}

// Create worst-case tree structure (linear chain - maximizes keyroots and computation)
Tree createWorstCaseTree(int numNodes, bool leftChain = true, bool debug = false) {
    if (numNodes <= 0) {
        cerr << "Number of nodes must be greater than zero." << endl;
        return Tree(nullptr);
    }
    
    char nodeChar = 'a';
    Node* root = createNode(nodeChar);
    nodeChar++;
    
    Node* current = root;
    
    // Create a linear chain structure (worst case for Zhang-Shasha)
    for (int i = 1; i < numNodes; ++i) {
        Node* newNode = createNode(nodeChar);
        nodeChar = nodeChar < 'z' ? nodeChar + 1 : 'a';
        
        if (leftChain) {
            // Create left-heavy chain (each node has only left child)
            current->add_child(newNode);
        } else {
            // Create right-heavy chain by adding as rightmost child
            current->add_child(newNode);
        }
        current = newNode;
    }
    
    Tree tree(root);
    
    // Perform post-order traversal
    int counter = 0;
    tree.post_order(tree.get_root(), counter);
    
    if (debug) {
        printTreeNodes(tree, leftChain ? "Worst Case Tree - Left Chain (post-order):" : "Worst Case Tree - Right Chain (post-order):");
    }
    
    // Find keyroots
    int last_li = -1;
    tree.find_keyroots(tree.get_root(), last_li);
    vector<Node*> keyroots = tree.get_LR_keyroots();
    reverse(keyroots.begin(), keyroots.end());
    
    if (debug) {
        printKeyroots(keyroots, leftChain ? "\nWorst Case Tree Keyroots - Left Chain:" : "\nWorst Case Tree Keyroots - Right Chain:");
    }
    
    return tree;
}

// Create balanced tree (best case scenario)
Tree createBestCaseTree(int numNodes, bool debug = false) {
    if (numNodes <= 0) {
        cerr << "Number of nodes must be greater than zero." << endl;
        return Tree(nullptr);
    }
    
    char nodeChar = 'a';
    vector<Node*> currentLevel;
    Node* root = createNode(nodeChar);
    nodeChar++;
    currentLevel.push_back(root);
    
    int nodesCreated = 1;
    
    // Create a root node with n-1 children
    while (nodesCreated < numNodes) {
        vector<Node*> nextLevel;
        for (Node* parent : currentLevel) {
            // Create two children for each parent if possible
            if (nodesCreated < numNodes) {
                Node* leftChild = createNode(nodeChar);
                nodeChar = nodeChar < 'z' ? nodeChar + 1 : 'a';
                parent->add_child(leftChild);
                nextLevel.push_back(leftChild);
                nodesCreated++;
            }
            if (nodesCreated < numNodes) {
                Node* rightChild = createNode(nodeChar);
                nodeChar = nodeChar < 'z' ? nodeChar + 1 : 'a';
                parent->add_child(rightChild);
                nextLevel.push_back(rightChild);
                nodesCreated++;
            }
        }
        currentLevel = nextLevel;
    }
    
    Tree tree(root);
    
    // Perform post-order traversal
    int counter = 0;
    tree.post_order(tree.get_root(), counter);
    
    if (debug) {
        printTreeNodes(tree, "Best Case Tree - Balanced (post-order):");
    }
    
    // Find keyroots
    int last_li = -1;
    tree.find_keyroots(tree.get_root(), last_li);
    vector<Node*> keyroots = tree.get_LR_keyroots();
    reverse(keyroots.begin(), keyroots.end());
    
    if (debug) {
        printKeyroots(keyroots, "\nBest Case Tree Keyroots - Balanced:");
    }
    
    return tree;
}

// Free memory of nodes
void freeNodes(vector<Node*>& nodes) {
    for (Node* n : nodes) delete n;
    nodes.clear();
}

// Modified Tree_Editing class to show matrices during calculation
class DebugTreeEditing : public Tree_Editing {
public:
    DebugTreeEditing(Tree* t1, Tree* t2) : Tree_Editing(t1, t2) {}
    
    // Override method to add debug printing    
    int computeTreeDistance(int index1, int index2) {
        cout << "Computing distance between subtrees rooted at " 
             << get_node1(index1)->label << " and " << get_node2(index2)->label << endl;
        
        // Get nodes corresponding to indices
        Node* n1 = get_node1(index1);
        Node* n2 = get_node2(index2);
        
        // Get left-most leaf indices
        int li = n1->li;
        int lj = n2->li;
        
        cout << "Left-most leaf index for " << n1->label << ": " << li << endl;
        cout << "Left-most leaf index for " << n2->label << ": " << lj << endl;
        
        // Calculate forest sizes
        int rows = interval_calc(li, index1);
        int cols = interval_calc(lj, index2);
        
        cout << "Forest 1 size: " << rows << endl;
        cout << "Forest 2 size: " << cols << endl;
        
        // Ensure matrices have correct size
        forest_dist.resize(rows + 1, vector<int>(cols + 1, 0));
        
        // Initialize forest_dist matrix
        forest_dist[0][0] = 0;
        for (int di = 1; di <= rows; di++) {
            forest_dist[di][0] = forest_dist[di-1][0] + remove_cost;
        }
        for (int dj = 1; dj <= cols; dj++) {
            forest_dist[0][dj] = forest_dist[0][dj-1] + add_cost;
        }        // Extract relevant nodes for this subtree
        vector<Node*> subnodes1;
        vector<Node*> subnodes2;
        
        // Fill subnodes1 using get_node1
        for (int i = li; i <= index1; i++) {
            subnodes1.push_back(get_node1(i));
        }
        
        // Fill subnodes2 using get_node2
        for (int j = lj; j <= index2; j++) {
            subnodes2.push_back(get_node2(j));
        }
        
        cout << "Initial forest_dist matrix:" << endl;
        printMatrix(forest_dist, subnodes1, subnodes2, "Forest Distance (Initial)");

        // Main edit distance calculation
        for (int di = 1; di <= rows; di++) {
            for (int dj = 1; dj <= cols; dj++) {                
                int node_i_idx = li + di - 1;  // Correction: -1 to adjust index
                int node_j_idx = lj + dj - 1;  // Correction: -1 to adjust index

                Node* ni = get_node1(node_i_idx);
                Node* nj = get_node2(node_j_idx);
                if (ni->li == li && nj->li == lj) {
                    // Both are left-most leaves
                    int update_cost = (ni->label == nj->label) ? 0 : rename_cost;
                    int del_cost = forest_dist[di-1][dj] + remove_cost;      // Deletion
                    int ins_cost = forest_dist[di][dj-1] + add_cost;         // Insertion
                    int upd_cost = forest_dist[di-1][dj-1] + update_cost;    // Substitution
                    
                    // Use min for each pair
                    forest_dist[di][dj] = std::min(del_cost, std::min(ins_cost, upd_cost));
                    
                    tree_dist[ni->walking_index+1][nj->walking_index+1] = forest_dist[di][dj]; //index +1 to skip index 0, corresponding to empty                    
                    cout << "Nodes " << ni->label << " and " << nj->label << " are left-most leaves." << endl;
                    cout << "Update cost: " << update_cost << endl;
                    cout << "Costs: del=" << del_cost << ", ins=" << ins_cost << ", upd=" << upd_cost << endl;
                } else {
                    // At least one is not left-most leaf
                    int del_cost = forest_dist[di-1][dj] + remove_cost;      // Deletion
                    int ins_cost = forest_dist[di][dj-1] + add_cost;         // Insertion
                    int sub_cost = forest_dist[ni->li - li][nj->li - lj] + 
                                   tree_dist[ni->walking_index+1][nj->walking_index+1];  // Subtree operation, 
                                                                                         //index +1 to skip index 0, corresponding to empty
                    
                    // Use min for each pair
                    forest_dist[di][dj] = std::min(del_cost, std::min(ins_cost, sub_cost));
                    
                    cout << "Nodes " << ni->label << " and " << nj->label << " are not both left-most leaves." << endl;
                    cout << "Using tree_dist[" << ni->walking_index << "][" << nj->walking_index 
                         << "] = " << tree_dist[ni->walking_index][nj->walking_index] << endl;
                }
                
                cout << "forest_dist[" << di << "][" << dj << "] = " << forest_dist[di][dj] << endl;
            }
        }
        
        cout << "Final forest_dist matrix:" << endl;
        printMatrix(forest_dist, subnodes1, subnodes2, "Forest Distance (Final)");
        
        // Return distance for complete forest
        return forest_dist[rows][cols];
    }
      int treeEditDistance(Tree T1, Tree T2) {
        // Update node vectors with those from received trees
        nodes1 = T1.get_indices();
        nodes2 = T2.get_indices();
        
        // Get keyroots of trees and reverse them correctly
        vector<Node*> keyroots1 = T1.get_LR_keyroots();
        vector<Node*> keyroots2 = T2.get_LR_keyroots();

        // Reverse keyroot vectors only once here
        // This reversal is necessary for Zhang-Shasha algorithm
        reverse(keyroots1.begin(), keyroots1.end());
        reverse(keyroots2.begin(), keyroots2.end());
        
        printKeyroots(keyroots1, "\nTree 1 Keyroots:");
        printKeyroots(keyroots2, "\nTree 2 Keyroots:");

        // Prepare matrix for tree distance
        tree_dist.resize(nodes1.size() + 1, vector<int>(nodes2.size() + 1, 0));

        // For each pair of keyroots, calculate distance between subtrees
        for (Node* n1 : keyroots1) {
            for (Node* n2 : keyroots2) {
                int i = n1->walking_index;
                int j = n2->walking_index;
                
                cout << "\n==============================================\n";
                cout << "Computing distance between keyroots " << n1->label 
                     << " (index " << i << ") and " << n2->label 
                     << " (index " << j << ")" << endl;
                
                // Calculate distance between subtrees rooted at i and j
                int dist = computeTreeDistance(i, j);
                cout << "Computed distance: " << dist << endl;
                cout << "==============================================\n";
            }
        }

        // Print final tree_dist matrix
        cout << "\nFinal tree distance matrix:" << endl;
        printMatrix(tree_dist, nodes1, nodes2, "Tree Distance");

        // Return distance between complete trees
        return tree_dist[nodes1.back()->walking_index][nodes2.back()->walking_index];
    }
};

// Performance result structure
struct PerformanceResult {
    int tree1Size;
    int tree1RootKeys;
    int tree2Size;
    int tree2RootKeys;
    double executionTimeMs;
    int distance;
    double memoryKB; // Adicionado para armazenar o uso médio de memória
};

/**
 * @brief Run worst-case performance test
 */
PerformanceResult runBestCaseTest(int size, bool debug = false) {
    Tree tree1 = createWorstCaseTree(size, true, debug);   // Left chain
    Tree tree2 = createWorstCaseTree(size, false, debug);  // Right chain

    // Cálculo estimado de memória (duas tabelas |T1| x |T2| de int)
    size_t mem_bytes = 2ULL * tree1.get_indices().size() * tree2.get_indices().size() * sizeof(int);
    double mem_kb = mem_bytes / (1024.0 * 1024.0);

    auto start = std::chrono::high_resolution_clock::now();

    Tree_Editing ted(&tree1, &tree2);
    int distance = ted.treeEditDistance(tree1, tree2);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    PerformanceResult result;
    result.tree1Size = size;
    result.tree1RootKeys = tree1.get_LR_keyroots().size();
    result.tree2Size = size;
    result.tree2RootKeys = tree2.get_LR_keyroots().size();
    result.executionTimeMs = duration.count() / 1000.0;
    result.distance = distance;
    result.memoryKB = mem_kb; // Salva o valor estimado de memória

    return result;
}

/**
 * @brief Run best-case performance test
 */
PerformanceResult runWorstCaseTest(int size, bool debug = false) {
    Tree tree1 = createBestCaseTree(size, debug);
    Tree tree2 = createBestCaseTree(size, debug);

    // Cálculo estimado de memória (duas tabelas |T1| x |T2| de int)
    size_t mem_bytes = 2ULL * tree1.get_indices().size() * tree2.get_indices().size() * sizeof(int);
    double mem_kb = mem_bytes / (1024.0 * 1024.0);

    auto start = std::chrono::high_resolution_clock::now();

    Tree_Editing ted(&tree1, &tree2);
    int distance = ted.treeEditDistance(tree1, tree2);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    PerformanceResult result;
    result.tree1Size = size;
    result.tree1RootKeys = tree1.get_LR_keyroots().size();
    result.tree2Size = size;
    result.tree2RootKeys = tree2.get_LR_keyroots().size();
    result.executionTimeMs = duration.count() / 1000.0;
    result.distance = distance;
    result.memoryKB = mem_kb; // Salva o valor estimado de memória

    return result;
}

/**
 * @brief Run performance test for a pair of trees
 */
PerformanceResult runPerformanceTest(int size1, int size2, int seed1, int seed2, bool debug = false) {
    Tree tree1 = createRandomTree(size1, seed1, debug);
    Tree tree2 = createRandomTree(size2, seed2, debug);

    // Cálculo estimado de memória (duas tabelas |T1| x |T2| de int)
    size_t mem_bytes = 2ULL * tree1.get_indices().size() * tree2.get_indices().size() * sizeof(int);
    mem_bytes += tree1.get_indices().size() * sizeof(Node*) + tree2.get_indices().size() * sizeof(Node*); // Adiciona o tamanho dos vetores de nós
    double mem_kb = mem_bytes / (1024.0);

    auto start = std::chrono::high_resolution_clock::now();

    Tree_Editing ted(&tree1, &tree2);
    int distance = ted.treeEditDistance(tree1, tree2);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    PerformanceResult result;
    result.tree1Size = size1;
    result.tree1RootKeys = tree1.get_LR_keyroots().size();
    result.tree2Size = size2;
    result.tree2RootKeys = tree2.get_LR_keyroots().size();
    result.executionTimeMs = duration.count() / 1000.0;
    result.distance = distance;
    result.memoryKB = mem_kb; // Salva o valor estimado de memória

    return result;
}

/**
 * @brief Save results to CSV file for later analysis
 */
void saveResultsToCSV(const vector<PerformanceResult>& results, const string& filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not create file " << filename << endl;
        return;
    }

    // CSV header
    file << "Tree1Size,Tree1RootKeys,Tree2Size,Tree2RootKeys,ExecutionTimeMs,Distance,MemoryKB\n";

    // Data
    for (const auto& result : results) {
        file << result.tree1Size << ","
                << result.tree1RootKeys << ","
                << result.tree2Size << ","
                << result.tree2RootKeys << ","
                << fixed << setprecision(4) << result.executionTimeMs << ","
                << result.distance << ","
                << fixed << setprecision(2) << result.memoryKB << "\n";
    }

    file.close();
    cout << "Results saved to: " << filename << endl;
}

/**
 * @brief Main function with test menu
 */
int best_worst_case_tests() {
    cout << "========================================" << endl;
    cout << "  PERFORMANCE ANALYSIS - ZHANG-SHASHA ALGORITHM" << endl;
    cout << "       Tree Edit Distance (TED)" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    vector<PerformanceResult> results;
    vector<int> sizes = {10000}; // Adjusted sizes for worst-case analysis
    
    cout << "Running comprehensive performance tests..." << endl;
    cout << "Test scenarios: Random, Best Case (Balanced), Worst Case (Linear)" << endl;
    cout << "Sizes tested: 10000 nodes" << endl;
    cout << string(70, '=') << endl;
    
    for (int size : sizes) {
        cout << "\n" << string(70, '-') << endl;
        cout << "TESTING TREES OF SIZE " << size << endl;
        cout << string(70, '-') << endl;

        cout << "\n1. BEST CASE TEST (Balanced Trees):" << endl;
        cout << "Testing balanced trees of size " << size << "...";
        cout.flush();
        auto testStart = std::chrono::high_resolution_clock::now();
        
        PerformanceResult bestResult = runBestCaseTest(size);
        results.push_back(bestResult);
        
        auto testEnd = std::chrono::high_resolution_clock::now();
        auto testDuration = std::chrono::duration_cast<std::chrono::milliseconds>(testEnd - testStart);
        
        cout << " Completed in " << testDuration.count() << " ms" << endl;
        cout << "  Tree 1 size: " << bestResult.tree1Size 
             << " (Root Keys: " << bestResult.tree1RootKeys << ")" << endl;
        cout << "  Tree 2 size: " << bestResult.tree2Size 
             << " (Root Keys: " << bestResult.tree2RootKeys << ")" << endl;
        cout << "  Algorithm time: " << fixed << setprecision(2) 
             << bestResult.executionTimeMs << " ms" << endl;
        cout << "  Edit distance: " << bestResult.distance << endl;
        cout << "  Estimated memory usage: " << fixed << setprecision(2) 
             << bestResult.memoryKB << " KB" << endl;
        
        // 2. Worst Case Test
        cout << "\n2. WORST CASE TEST (Linear Chains):" << endl;
        cout << "Testing linear chain trees of size " << size << "...";
        cout.flush();
        testStart = std::chrono::high_resolution_clock::now();
        
        PerformanceResult worstResult = runWorstCaseTest(size);
        results.push_back(worstResult);
        
        testEnd = std::chrono::high_resolution_clock::now();
        testDuration = std::chrono::duration_cast<std::chrono::milliseconds>(testEnd - testStart);
        
        cout << " Completed in " << testDuration.count() << " ms" << endl;
        cout << "  Tree 1 size: " << worstResult.tree1Size 
             << " (Root Keys: " << worstResult.tree1RootKeys << ")" << endl;
        cout << "  Tree 2 size: " << worstResult.tree2Size 
             << " (Root Keys: " << worstResult.tree2RootKeys << ")" << endl;
        cout << "  Algorithm time: " << fixed << setprecision(2) 
             << worstResult.executionTimeMs << " ms" << endl;
        cout << "  Edit distance: " << worstResult.distance << endl;
        
        // Performance comparison
        cout << "\n3. PERFORMANCE COMPARISON:" << endl;
        cout << "  Worst vs Best:   " << fixed << setprecision(2) 
             << (worstResult.executionTimeMs / bestResult.executionTimeMs) << "x slower" << endl;
    }
    
    cout << "\n" << string(70, '=') << endl;
    
    // Save results to CSV file
    saveResultsToCSV(results, "BW_complexity_results.csv");
    
    cout << "\n========================================" << endl;
    cout << "      COMPREHENSIVE TESTS COMPLETED" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    return 0;
}

void random_tests(){
    cout << "========================================" << endl;
    cout << "  PERFORMANCE ANALYSIS - ZHANG-SHASHA ALGORITHM" << endl;
    cout << "       Tree Edit Distance (TED)" << endl;
    cout << "========================================" << endl;
    cout << endl;
    
    vector<PerformanceResult> results;
    vector<int> sizes = {10, 100, 1000, 10000}; // Tree sizes to test
    
    cout << "Running tests for different tree sizes..." << endl;
    cout << "Sizes tested: 10, 100, 1000, 10000 nodes" << endl;
    cout << string(60, '-') << endl;
    
    for (int size : sizes) {
        cout << "Testing trees of size " << size << "...";
        cout.flush();
        auto testStart = std::chrono::high_resolution_clock::now();
            
        // Test with trees of same size
        PerformanceResult result = runPerformanceTest(size, size, 
                                                     size * 10, 
                                                     size * 20);
        results.push_back(result);
            
        auto testEnd = std::chrono::high_resolution_clock::now();
        auto testDuration = std::chrono::duration_cast<std::chrono::milliseconds>(testEnd - testStart);
            
        cout << " Completed in " << testDuration.count() << " ms" << endl;
        cout << "  Tree 1 size: " << result.tree1Size 
             << " (Root Keys length: " << result.tree1RootKeys << ")" << endl;
        cout << "  Tree 2 size: " << result.tree2Size 
             << " (Root Keys length: " << result.tree2RootKeys << ")" << endl;
        // Show basic results
        cout << "  Algorithm time: " << fixed << setprecision(2) 
             << result.executionTimeMs << " ms" << endl;
        cout << "  Edit distance (TED): " << result.distance << endl;
        cout << "  Estimated memory usage: " << fixed << setprecision(2) 
             << result.memoryKB << " KB" << endl;
        cout << endl;
        cout << endl;
    }
    
    cout << string(60, '-') << endl;
    
    // Save results to CSV file
    saveResultsToCSV(results, "RAND_complexity_results.csv");
    
    cout << "========================================" << endl;
    cout << "           TESTS COMPLETED" << endl;
    cout << "========================================" << endl;
    cout << endl;
}

/**
 * @brief Main function with test menu
 */
int main() {
    
    random_tests();
    best_worst_case_tests();
    
    return 0;
}


// Deprecated manual test code - kept for tests
// int main() {
//     // Create trees and store pointers to free later
//     vector<Node*> nodes1, nodes2;
//     Tree tree1 = createTestTree1(nodes1);
//     Tree tree2 = createTestTree2(nodes2);

//     DebugTreeEditing editor(&tree1, &tree2);
//     int distance = editor.treeEditDistance(tree1, tree2);
//     cout << "\n==============================================\n";
//     cout << "Edit distance between trees: " << distance << endl;
//     cout << "==============================================\n";
//     freeNodes(nodes1);
//     freeNodes(nodes2);   
//     return 0;
// }

