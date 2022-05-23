#include <iostream>
#include <algorithm>

#include "RBTree.h"



using namespace std;

int main() {
    auto tree = new RBTree();
    string key;
    unsigned long long value;

    string command;

    while (cin >> command) {
        switch (command[0]) {
            case '+':
                cin >> key;
                transform(key.begin(), key.end(), key.begin(),
                          [](unsigned char c){ return std::tolower(c); });
                cin >> value;
                tree->insert(key, value);;
                break;
            case '-':
                cin >> key;
                transform(key.begin(), key.end(), key.begin(),
                          [](unsigned char c){ return std::tolower(c); });
                tree->deleteNode(key);
                break;
            case '!':
                cin >> command;
                if (command == "Save"){
                    string path;
                    cin >> path;
                    tree->writeTree(path);
                }
                if (command == "Load") {
                    string path;
                    cin >> path;
                    tree->clean(tree->root);
                    delete tree;
                    tree = RBTree::readTree(path);
                }
                break;
            default:
                transform(command.begin(), command.end(), command.begin(),
                          [](unsigned char c){ return std::tolower(c); });
                tree->findNode(command);
                break;
        }

    }
    tree->clean(tree->root);
    delete tree;
    return 0;
}
