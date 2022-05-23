#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>


using namespace std;

class TRBTree;

class TNode {
public:
    string Key;
    unsigned long long Value;
    bool Red;
    TNode* Left;
    TNode* Right;
    TNode* Parent;

    TNode(string key, unsigned long long value);

    void LeftRotate(TRBTree* t);
    void RightRotate(TRBTree* t);
    void InsertRebalance(TRBTree* t);
    void DeleteRebalance(TRBTree* t);

    static bool IsRed(const TNode* node);
};

class TRBTree {
public:
    TNode *Root;

    void Insert(const string &key, unsigned long long value, bool fromFile) {
        if (Root != nullptr) {
            if (Root->Key == key) {
                cout << "Exist" << endl;
                return;
            }
            auto newNode = new TNode(key, value);
            Insert(newNode, this->Root, fromFile);
        } else {
            auto newNode = new TNode(key, value);
            Root = newNode;
            Root->Red = false;
            if (!fromFile) {
                cout << "OK" << endl;
            }
        }
    }

    TNode *Find(const string &key) {
        auto currentNode = this->Root;
        while (currentNode != nullptr) {
            int compRes = key.compare(currentNode->Key);
            if (compRes < 0) {
                currentNode = currentNode->Left;
            } else if (compRes > 0) {
                currentNode = currentNode->Right;
            } else {
                return currentNode;
            }
        }
        return nullptr;
    }

    void FindNode(const string& key) {
        TNode* node = Find(key);
        if (node == nullptr) {
            cout << "NoSuchWord" << endl;
            return;
        }
        cout << "OK: " << node->Value << endl;
    }

    void DeleteNode(const string &key) {
        TNode* z = Find(key);
        if (z == nullptr) {
            cout << "NoSuchWord" << endl;
            return;
        }
        DeleteNode(z);
    }

    void WriteTreeAlter(string& path) {
        ofstream file(path, ios::trunc | ios::binary);
        WriteBinary(this->Root, file);
        file.close();
    }

    void WriteBinary(TNode* node, ofstream& file) {
        if (node == nullptr) {
            return;
        }
        size_t size = node->Key.length();
        file.write(reinterpret_cast<char*>(&size), sizeof(size));
        file.write(node->Key.c_str(), size);
        file.write(reinterpret_cast<char*>(&node->Value), sizeof(node->Value));
        file.flush();
        WriteBinary(node->Left, file);
        WriteBinary(node->Right, file);
    }

    static TRBTree* ReadTreeAlter(string& source) {
        ifstream file(source, ios::binary);
        size_t size;
        auto tree = new TRBTree();
        while (file.read(reinterpret_cast<char*>(&size), sizeof(size))) {
            string key;
            unsigned long long value;
            key.resize(size);
            file.read(key.data(), size);
            file.read(reinterpret_cast<char*>(&value), sizeof(value));
            tree->Insert(key, value, true);
        }
        file.close();
        return tree;
    }

    void Clean(TNode* t) {
        if (t == nullptr) {
            return;
        }
        Clean(t->Left);
        Clean(t->Right);
        delete t;
    }

private:
    void Insert(TNode *newNode, TNode *currentNode, bool fromFile) {
        int compRes = newNode->Key.compare(currentNode->Key);
        if (compRes > 0) {
            if (currentNode->Right != nullptr) {
                Insert(newNode, currentNode->Right, fromFile);
            } else {
                newNode->Parent = currentNode;
                currentNode->Right = newNode;
                newNode->InsertRebalance(this);
                if (!fromFile) {
                    cout << "OK" << endl;
                }
            }
        } else if (compRes < 0) {
            if (currentNode->Left != nullptr) {
                Insert(newNode, currentNode->Left, fromFile);
            } else {
                newNode->Parent = currentNode;
                currentNode->Left = newNode;
                newNode->InsertRebalance(this);
                if (!fromFile) {
                    cout << "OK" << endl;
                }
            }
        }
        else {
            std::cout << "Exist" << endl;
        }
    }

    void DeleteNode(TNode *node) {
        if (node->Left != nullptr && node->Right != nullptr) {
            TNode* s = FindNext(node);
            node->Key = s->Key;
            node->Value = s->Value;
            node = s;
        }
        TNode* replace;
        if (node->Left == nullptr) {
            replace = node->Right;
        } else {
            replace = node->Left;
        }

        if (replace != nullptr) {
            replace->Parent = node->Parent;
            if (node->Parent == nullptr) {
                this->Root = replace;
            } else if (node == node->Parent->Left) {
                node->Parent->Left = replace;
            } else {
                node->Parent->Right = replace;
            }

            node->Parent = nullptr;
            node->Left = nullptr;
            node->Right = nullptr;

            if (!TNode::IsRed(node)) {
                replace->DeleteRebalance(this);
            }
        } else if (node->Parent == nullptr) {
            this->Root = nullptr;
        } else {
            if (!TNode::IsRed(node)) {
                node->DeleteRebalance(this);
            }

            if (node->Parent != nullptr) {
                if (node == node->Parent->Left) {
                    node->Parent->Left = nullptr;
                } else if (node == node->Parent->Right) {
                    node->Parent->Right = nullptr;
                }
                node->Parent = nullptr;
            }
        }
        delete node;
        cout << "OK" << endl;
    }

    TNode *FindNext(TNode *node) {
        if (node == nullptr) {
            return node;
        } else if (node->Right != nullptr) {
            auto child = node->Right;
            while (child->Left != nullptr) {
                child = child->Left;
            }
            return child;
        } else {
            auto parent = node->Parent;
            auto currentChild = node;
            while (parent != nullptr && currentChild == parent->Right) {
                currentChild = parent;
                parent = parent->Parent;
            }
            return parent;
        }
    }

};

TNode::TNode(string key, unsigned long long value) {
    this->Key = std::move(key);
    this->Value = value;
    this->Red = true;

    Left = nullptr;
    Right = nullptr;
    Parent = nullptr;
}

void TNode::LeftRotate(TRBTree *t) {
    TNode *node = this->Right;
    this->Right = node->Left;

    if (node->Left != nullptr) {
        node->Left->Parent = this;
    }
    node->Parent = this->Parent;
    if (this->Parent == nullptr) {
        t->Root = node;
    } else if (this == this->Parent->Left) {
        this->Parent->Left = node;
    } else {
        this->Parent->Right = node;
    }
    node->Left = this;
    this->Parent = node;
}

void TNode::RightRotate(TRBTree *t) {
    TNode *node = this->Left;
    this->Left = node->Right;

    if (node->Right != nullptr) {
        node->Right->Parent = this;
    }
    node->Parent = this->Parent;
    if (this->Parent == nullptr) {
        t->Root = node;
    } else if (this == this->Parent->Left) {
        this->Parent->Left = node;
    } else {
        this->Parent->Right = node;
    }
    node->Right = this;
    this->Parent = node;
}

void TNode::InsertRebalance(TRBTree *t) {
    auto current = this;
    auto parentNode = current->Parent;

    while (IsRed(parentNode)) {
        auto dad = parentNode->Parent;
        if (dad->Left == parentNode) {
            auto uncle = dad->Right;
            if (IsRed(uncle)) {
                parentNode->Red = false;
                uncle->Red = false;
                dad->Red = true;
                current = dad;
                parentNode = current->Parent;
            } else {
                if (parentNode->Right == current) {
                    parentNode->LeftRotate(t);
                    swap(current, parentNode);
                }
                dad->RightRotate(t);
                dad->Red = true;
                parentNode->Red = false;
                break;
            }
        } else {
            auto uncle = dad->Left;
            if (IsRed(uncle)) {
                dad->Red = true;
                parentNode->Red = false;
                uncle->Red = false;
                current = dad;
                parentNode = current->Parent;
            } else {
                if (parentNode->Left == current) {
                    parentNode->RightRotate(t);
                    swap(parentNode, current);
                }
                dad->LeftRotate(t);
                parentNode->Red = false;
                dad->Red = true;
                break;
            }
        }
    }
    t->Root->Red = false;
}



void TNode::DeleteRebalance(TRBTree *t) {
    auto current = this;
    while (current != t->Root && !IsRed(current)) {
        if (current == current->Parent->Left) {
            auto bro = current->Parent->Right;

            if (IsRed(bro)) {
                bro->Red = false;
                current->Parent->Red = true;
                current->Parent->LeftRotate(t);
                bro = current->Parent->Right;
            }

            if (!IsRed(bro->Left) && !IsRed(bro->Right)) {
                bro->Red = true;
                current = current->Parent;
            } else {
                if (!IsRed(bro->Right)) {
                    bro->Left->Red = false;
                    bro->Red = true;
                    bro->RightRotate(t);
                    bro = current->Parent->Right;
                }
                bro->Red = current->Parent->Red;
                current->Parent->Red = false;
                bro->Right->Red = false;
                current->Parent->LeftRotate(t);
                current = t->Root;
            }
        } else {
            auto bro = current->Parent->Left;

            if (IsRed(bro)) {
                bro->Red = false;
                current->Parent->Red = true;
                current->Parent->RightRotate(t);
                bro = current->Parent->Left;
            }

            if (!IsRed(bro->Right) && !IsRed(bro->Left)) {
                bro->Red = true;
                current = current->Parent;
            } else {
                if (!IsRed(bro->Left)) {
                    bro->Right->Red = false;
                    bro->Red = true;
                    bro->LeftRotate(t);
                    bro = current->Parent->Left;
                }
                bro->Red = current->Parent->Red;
                current->Parent->Red = false;
                bro->Left->Red = false;
                current->Parent->RightRotate(t);
                current = t->Root;
            }
        }
    }
    current->Red = false;
}

bool TNode::IsRed(const TNode *node) {
    if (node == nullptr) {
        return false;
    }
    return node->Red;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    auto tree = new TRBTree();
    string key;
    unsigned long long value;

    string command;

    unsigned long long addTime = 0;
    unsigned long long removeTime = 0;
    unsigned long long findTime = 0;

    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point finish;

    while (cin >> command) {
        switch (command[0]) {
            case '+':
                cin >> key;
                transform(key.begin(), key.end(), key.begin(),
                          [](unsigned char c){ return std::tolower(c); });
                cin >> value;
                start = std::chrono::steady_clock::now();
                tree->Insert(key, value, false);
                finish = std::chrono::steady_clock::now();
                addTime += std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
                break;
            case '-':
                cin >> key;
                transform(key.begin(), key.end(), key.begin(),
                          [](unsigned char c){ return std::tolower(c); });
                start = std::chrono::steady_clock::now();
                tree->DeleteNode(key);
                finish = std::chrono::steady_clock::now();
                removeTime += std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
                break;
            case '!':
                cin >> command;
                if (command == "Save"){
                    string path;
                    cin >> path;
                    tree->WriteTreeAlter(path);
                    cout << "OK" << endl;
                }
                if (command == "Load") {
                    string path;
                    cin >> path;
                    tree->Clean(tree->Root);
                    delete tree;
                    tree = TRBTree::ReadTreeAlter(path);
                    cout << "OK" << endl;
                }
                break;
            default:
                transform(command.begin(), command.end(), command.begin(),
                          [](unsigned char c){ return std::tolower(c); });
                start = std::chrono::steady_clock::now();
                tree->FindNode(command);
                finish = std::chrono::steady_clock::now();
                findTime += std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
                break;
        }

    }
    tree->Clean(tree->Root);
    cout << "find " << findTime << "\n";
    cout << "add " << addTime << "\n";
    cout << "delete " << removeTime << "\n";
    delete tree;
}

