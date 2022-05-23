
#pragma once

#include <iostream>
#include <fstream>
#include "Node.h"

using namespace std;

class Node;

class RBTree {
public:
    Node *root;

    void insert(const string &key, unsigned long long value) {
        if (root != nullptr) {
            if (root->key == key) {
                cout << "Exist" << endl;
                return;
            }
            auto newNode = new Node(key, value);
            insert(newNode, this->root);
        } else {
            auto newNode = new Node(key, value);
            root = newNode;
            root->red = false;
            cout << "OK" << endl;
        }
    }

    Node *Find(const string &key) {
        auto currentNode = this->root;
        while (currentNode != nullptr) {
            int compRes = key.compare(currentNode->key);
            if (compRes < 0) {
                currentNode = currentNode->left;
            } else if (compRes > 0) {
                currentNode = currentNode->right;
            } else {
                return currentNode;
            }
        }
        return nullptr;
    }

    void FindNode(const string& key) {
        auto node = find(key);
        if (node == nullptr) {
            cout << "NoSuchWord" << endl;
            return;
        }
        cout << "OK: " << node->value << endl;
    }

    void DeleteNode(const string &key) {
        auto z = find(key);
        if (z == nullptr) {
            cout << "NoSuchWord" << endl;
            return;
        }
        deleteNode(z);
    }

    void print() {
        print(root, 0);
    }

   void writeTree(string& path) {
        string tree;
        ofstream file(path, std::ios_base::trunc);
        writeTree(file, this->root);
        file << '\n';
        file.close();
    }

    static RBTree* readTree(string& source) {
        ifstream file(source);
        string s;
        s.append("#");
        cerr << s << endl;
        getline(file, s);
        auto tree = new RBTree();
        int index = 0;
        if (s.length() != 0) {
            auto node = new Node(getKey(index, s), getValue(index, s), getIsRed(index, s));
            tree->root = node;
        }
        auto currentNode = tree->root;
        while (index <= s.length()) {
            ++index;
            char path = s[index];
            ++index;
            switch (path) {
                case '\n':
                    file.close();
                    return tree;
                case 'P':
                    --index;
                    currentNode = currentNode->parent;
                    break;
                case 'L':
                    currentNode->left = new Node(getKey(index, s), getValue(index, s), getIsRed(index, s));
                    currentNode->left->parent = currentNode;
                    currentNode = currentNode->left;
                    break;
                case 'R':
                    currentNode->right = new Node(getKey(index, s), getValue(index, s), getIsRed(index, s));
                    currentNode->right->parent = currentNode;
                    currentNode = currentNode->right;
                    break;
            }
        }
        file.close();
        return tree;
    }

    void clean(Node* t) {
        if (t == nullptr) {
            return;
        }
        clean (t->left);
        clean (t->right);
        delete t;
    }

private:
    static string getKey(int& i, string& file) {
        string key;
        key.reserve(258);
        while (file[i] != '$') {
            key.append({file[i]});
            ++i;
        }
        ++i;
        return key;
    }

    static unsigned long long getValue(int& i, string& file) {
        string value;
        value.reserve(25);
        while (file[i] != '$') {
            value.append({file[i]});
            ++i;
        }
        ++i;
        return stoull(value);
    }

    static bool getIsRed(int& i, string& file) {
        ++i;
        return file[i - 1] == 'R';
    }

    void writeTree(ofstream& file, Node *node) {
        if (node == nullptr) {
            return;
        }
        if (node->parent != nullptr && node == node->parent->left) {
            file << "L";
        } else if (node->parent != nullptr && node == node->parent->right) {
            file << "R";
        }
        file << node->key;
        file << "$";
        file << node->value;
        file << "$";
        file << (node->red ? "R" : "B");
        file << "$";
        //file.flush();
        writeTree(file, node->left);
        writeTree(file, node->right);
        file << "P";
        //file.flush();
    }

    void insert(Node *newNode, Node *currentNode) {
        int compRes = newNode->key.compare(currentNode->key);
        if (compRes > 0) {
            if (currentNode->right != nullptr) {
                insert(newNode, currentNode->right);
            } else {
                newNode->parent = currentNode;
                currentNode->right = newNode;
                newNode->insertRebalance(this);
                cout << "OK" << endl;
            }
        } else if (compRes < 0) {
            if (currentNode->left != nullptr) {
                insert(newNode, currentNode->left);
            } else {
                newNode->parent = currentNode;
                currentNode->left = newNode;
                newNode->insertRebalance(this);
                cout << "OK" << endl;
            }
        }
        else {
            std::cout << "Exist" << endl;
        }
    }

    void print(Node *node, int height) {
        if (node == nullptr) {
            return;
        }
        if (node->right != nullptr) {
            print(node->right, height + 2);
        }
        for (int i = 0; i < height; i++) {
            cout << "-";
        }
        printf("{%s, %llu, %s}\n", node->key.c_str(), node->value, node->red ? "R" : "B");
        if (node->left != nullptr) {
            print(node->left, height + 2);
        }
    }

    void DeleteNode(Node *node) {
        if (node->left != nullptr && node->right != nullptr) {
            auto s = findNext(node);
            node->key = s->key;
            node->value = s->value;
            node = s;
        }
        Node* replace;
        if (node->left == nullptr) {
            replace = node->right;
        } else {
            replace = node->left;
        }

        if (replace != nullptr) {
            replace->parent = node->parent;
            if (node->parent == nullptr) {
                this->root = replace;
            } else if (node == node->parent->left) {
                node->parent->left = replace;
            } else {
                node->parent->right = replace;
            }

            node->parent = nullptr;
            node->left = nullptr;
            node->right = nullptr;

            if (!Node::isRed(node)) {
                replace->deleteRebalance(this);
            }
        } else if (node->parent == nullptr) {
            this->root = nullptr;
        } else {
            if (!Node::isRed(node)) {
                node->deleteRebalance(this);
            }

            if (node->parent != nullptr) {
                if (node == node->parent->left) {
                    node->parent->left = nullptr;
                } else if (node == node->parent->right) {
                    node->parent->right = nullptr;
                }
                node->parent = nullptr;
            }
        }
        delete node;
        cout << "OK" << endl;
    }

    Node *FindNext(Node *node) {
        if (node == nullptr) {
            return node;
        } else if (node->right != nullptr) {
            auto child = node->right;
            while (child->left != nullptr) {
                child = child->left;
            }
            return child;
        } else {
            auto parent = node->parent;
            auto currentChild = node;
            while (parent != nullptr && currentChild == parent->right) {
                currentChild = parent;
                parent = parent->parent;
            }
            return parent;
        }
    }

};

