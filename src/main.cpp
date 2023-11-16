#include "CustomAssert.h"
#include "Tree.h"
#include "TreeDefinitions.h"
#include "TreeDump.h"

int main () {
    PushLog (1);

    Tree::Tree <char *> tree = {};

    Tree::InitTree_    (&tree, {});

    Tree::AddNode_     (&tree, tree.root, Tree::LEFT_CHILD, {});

    Tree::TreeDump_    (&tree, {});

    Tree::DestroyTree_ (&tree);

    RETURN 0;
}
