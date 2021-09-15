//
// Created by djent on 09.04.2021.
//

#include <stdio.h>
#include "cstack.h"
#include "ctree24.h"

signed main() {
    struct stack numbers, exitStack;
    struct tree t;
    createTree(&t);
    create(&numbers);
    create(&exitStack);
    bool a = fill(&numbers, &exitStack);
    if (a) {
        insert(&t, &exitStack);
        printf("Tree: ");
        printTree(t.root);
        if (t.root->lTree)
            execute(&t, t.root->lTree);
        printf("\n");
        printf("Result: ");
        printSign();
        printf("%c", '(');
        printFinal(t.root, &t);
        printOne();
        printf("%c", ')');
        nclear(&t);
    }
}

