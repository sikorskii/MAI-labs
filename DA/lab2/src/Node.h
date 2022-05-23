

#pragma once

#include <string>
#include <utility>

using namespace std;

class RBTree;

class Node {
public:
    string key;
    unsigned long long value;
    bool red;
    Node* left;
    Node* right;
    Node* parent;

    Node(string key, unsigned long long value);
    Node(string key, unsigned long long value, bool red);

    void LeftRotate(RBTree* t);
    void RightRotate(RBTree* t);
    void InsertRebalance(RBTree* t);
    void DeleteRebalance(RBTree* t);

    static bool IsRed(const Node* node);
};
