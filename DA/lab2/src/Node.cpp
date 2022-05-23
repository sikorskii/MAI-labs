
#include "Node.h"
#include "RBTree.h"


Node::Node(string key, unsigned long long value) {
    this->key = std::move(key);
    this->value = value;
    this->red = true;

    left = nullptr;
    right = nullptr;
    parent = nullptr;
}

Node::Node(string key, unsigned long long int value, bool red) {
    this->key = std::move(key);
    this->value = value;
    this->red = red;

    left = nullptr;
    right = nullptr;
    parent = nullptr;
}

void Node::leftRotate(RBTree *t) {
    Node *node = this->right;
    this->right = node->left;

    if (node->left != nullptr) {
        node->left->parent = this;
    }
    node->parent = this->parent;
    if (this->parent == nullptr) {
        t->root = node;
    } else if (this == this->parent->left) {
        this->parent->left = node;
    } else {
        this->parent->right = node;
    }
    node->left = this;
    this->parent = node;
}

void Node::rightRotate(RBTree *t) {
    Node *node = this->left;
    this->left = node->right;

    if (node->right != nullptr) {
        node->right->parent = this;
    }
    node->parent = this->parent;
    if (this->parent == nullptr) {
        t->root = node;
    } else if (this == this->parent->left) {
        this->parent->left = node;
    } else {
        this->parent->right = node;
    }
    node->right = this;
    this->parent = node;
}

void Node::insertRebalance(RBTree *t) {
    auto current = this;
    auto parentNode = current->parent;

    while (isRed(parentNode)) {
        auto dad = parentNode->parent;
        if (dad->left == parentNode) {
            auto uncle = dad->right;
            if (isRed(uncle)) {
                parentNode->red = false;
                uncle->red = false;
                dad->red = true;
                current = dad;
                parentNode = current->parent;
            } else {
                if (parentNode->right == current) {
                    parentNode->leftRotate(t);
                    swap(current, parentNode);
                }
                dad->rightRotate(t);
                dad->red = true;
                parentNode->red = false;
                break;
            }
        } else {
            auto uncle = dad->left;
            if (isRed(uncle)) {
                dad->red = true;
                parentNode->red = false;
                uncle->red = false;
                current = dad;
                parentNode = current->parent;
            } else {
                if (parentNode->left == current) {
                    parentNode->rightRotate(t);
                    swap(parentNode, current);
                }
                dad->leftRotate(t);
                parentNode->red = false;
                dad->red = true;
                break;
            }
        }
    }
    t->root->red = false;
}

void Node::deleteRebalance(RBTree *t) {
    auto current = this;
    while (current != t->root && !isRed(current)) {
        if (current == current->parent->left) {
            auto bro = current->parent->right;

            if (isRed(bro)) {
                bro->red = false;
                current->parent->red = true;
                current->parent->leftRotate(t);
                bro = current->parent->right;
            }

            if (!isRed(bro->left) && !isRed(bro->right)) {
                bro->red = true;
                current = current->parent;
            } else {
                if (!isRed(bro->right)) {
                    bro->left->red = false;
                    bro->red = true;
                    bro->rightRotate(t);
                    bro = current->parent->right;
                }
                bro->red = current->parent->red;
                current->parent->red = false;
                bro->right->red = false;
                current->parent->leftRotate(t);
                current = t->root;
            }
        } else {
            auto bro = current->parent->left;

            if (isRed(bro)) {
                bro->red = false;
                current->parent->red = true;
                current->parent->rightRotate(t);
                bro = current->parent->left;
            }

            if (!isRed(bro->right) && !isRed(bro->left)) {
                bro->red = true;
                current = current->parent;
            } else {
                if (!isRed(bro->left)) {
                    bro->right->red = false;
                    bro->red = true;
                    bro->leftRotate(t);
                    bro = current->parent->left;
                }
                bro->red = current->parent->red;
                current->parent->red = false;
                bro->left->red = false;
                current->parent->rightRotate(t);
                current = t->root;
            }
        }
    }
    current->red = false;
}



bool Node::isRed(const Node *node) {
    if (node == nullptr) {
        return false;
    }
    return node->red;
}






