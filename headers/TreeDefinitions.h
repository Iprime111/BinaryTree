#ifndef TREE_DEFINITIONS_H_
#define TREE_DEFINITIONS_H_

#include <bits/types/FILE.h>
#include <stddef.h>

namespace Tree {

    struct CallData {
        const char *file;
        int line;
        const char *function;
    };

    enum TreeError {
        NO_TREE_ERRORS    = 0,
        NULL_TREE_POINTER = 1 << 0,
        NULL_NODE_POINTER = 1 << 1,
        NULL_ROOT_POINTER = 1 << 2,
        NODE_ALREADY_USED = 1 << 3,
        OUTPUT_ERROR      = 1 << 4,
        LOG_ERROR         = 1 << 5,
    };

    enum TreeEdge {
        NO_EDGE     = 0,
        LEFT_CHILD  = 1 << 0,
        RIGHT_CHILD = 1 << 1,
        PARENT_NODE = 1 << 2,
    };

    enum PrintType {
        PREFIX_PRINT  = 0,
        INFIX_PRINT   = 1,
        POSTFIX_PRINT = 2,
    };
    
    template <typename T>
    struct Node {
        Node <T> *left   = NULL;
        Node <T> *right  = NULL;
        Node <T> *parent = NULL; 

        T nodeData = {};
    };

    template <typename T>
    struct Tree {
        Node <T> *root = NULL;

        TreeError errors = NO_TREE_ERRORS;
        CallData creationData;
    };
    
    template <typename T>
    TreeError NodeDataDestructor (Node <T> *node);
    
    template <typename T>
    TreeError VerifyTree  (Tree <T> *tree);
    template <typename T>
    TreeError VerifyNode  (Tree <T> *tree, Node <T> *node);

    template <typename T>
    TreeError InitTree_    (Tree <T> *tree,  CallData callData);
    template <typename T>
    TreeError InitNode_    (Node <T> **node, CallData callData);

    template <typename T>
    TreeError DestroyTree        (Tree <T> *tree);
    template <typename T>
    TreeError DestroySingleNode  (Node <T> *node);
    template <typename T> 
    TreeError DestroySubtreeNode (Tree <T> *tree, Node <T> *node);

    template <typename T>
    TreeError AddNode_    (Tree <T> *tree, Node <T> *leaf, TreeEdge direction, CallData callData);
    template <typename T>
    TreeError DeleteNode_ (Tree <T> *tree, Node <T> *nodePointer, CallData callData);
    
    template <typename T>
    TreeError PrintTree_  (Tree <T> *tree, PrintType printType, FILE *stream, CallData callData);

    template <typename T>
    TreeError PrintNodeValue   (Node <T> *node, FILE *stream);
    template <typename T>
    TreeError SnprintNodeValue (Node <T> *node, char *buffer, size_t maxLength);

    template <typename T>
    TreeError TreeDump_ (Tree <T> *tree, CallData callData);

    template <typename T>
    Node <T> *NextNode (Node <T> *node, TreeEdge direction);

    #define CreateCallData {__FILE__, __LINE__, __PRETTY_FUNCTION__}

    #define InitTree(tree) InitTree_ (tree, CreateCallData)
    #define InitNode(node) InitNode_ (node, CreateCallData)

    #define AddNode(tree, leaf, direction) AddNode_    (tree, leaf, direction, CreateCallData)
    #define DeleteNode(tree, nodePointer)  DeleteNode_ (tree, nodePointer,     CreateCallData)

    #define PrintTree(tree, printType, stream) PrintTree_ (tree, printType, stream, CreateCallData)

    #define TreeDump(tree) TreeDump_ (tree, CreateCallData);
}
#endif
