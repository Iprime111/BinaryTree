#include <stdlib.h>

#include "CustomAssert.h"
#include "Logger.h"
#include "TreeDefinitions.h"
#include "TreePrints.h"
#include "TreeDump.h"

#ifndef TREE_H_
#define TREE_H_

namespace Tree {
    #define TreeVerification(tree)                                      \
                do {                                                    \
                    TreeError verificationResult_ = VerifyTree_ (tree); \
                    if (verificationResult_ != NO_TREE_ERRORS) {        \
                        RETURN verificationResult_;                     \
                    }                                                   \
                }while (0)
    
    #define WriteError(tree, error)  (tree)->errors = (TreeError) (error | (tree)->errors)
    #define ReturnError(tree, error)                \
                do {                                \
                    if (error != NO_TREE_ERRORS) {  \
                        WriteError (tree, error);   \
                        RETURN error;               \
                    }                               \
                }while (0)
    
    template <typename T>
    static TreeError RecursiveTreeVerification (Tree <T> *tree, Node <T> *node);
    
    template <typename T>
    static TreeError InternalPrint_ (Node <T> *node, PrintType printType, FILE *stream);
    
    template <typename T>
    TreeError VerifyTree_ (Tree <T> *tree) {
        PushLog (3);
    
        if (!tree) {
            RETURN NULL_TREE_POINTER;
        }
    
        if (!tree->root) {
            ReturnError (tree, NULL_ROOT_POINTER);
        }
    
        RecursiveTreeVerification (tree, tree->root);
    
        RETURN tree->errors;
    }
    
    template <typename T>
    static TreeError RecursiveTreeVerification (Tree <T> *tree, Node <T> *node) {
        PushLog (4);
    
        custom_assert (node, pointer_is_null, NULL_NODE_POINTER);
    
        TreeError errors = NO_TREE_ERRORS;
    
        #define VerifyChild_(child) errors = (TreeError) (VerifyNode_ (tree, node->child) | errors); 
    
        if (node->left)
            VerifyChild_ (left);
    
        if (node->left)
            VerifyChild_(right);
    
        #undef VerifyChild_
    
        RETURN errors;
    }
    
    template <typename T>
    TreeError VerifyNode_ (Tree <T> *tree, Node <T> *node) {
        PushLog (3);
    
        if (!tree) {
            RETURN NULL_TREE_POINTER;
        }
    
        if (!node) {
            ReturnError (tree, NULL_NODE_POINTER);
        }
    
        RETURN tree->errors;
    }
    
    template <typename T>
    TreeError InitTree_ (Tree <T> *tree, CallData callData) {
        PushLog (3);
        
        custom_assert (tree, pointer_is_null, NULL_TREE_POINTER);
    
        if (InitNode_ (&tree->root, callData) != NO_TREE_ERRORS) {
            RETURN NULL_ROOT_POINTER;
        }
        
        tree->root->left  = NULL;
        tree->root->right = NULL;
    
        TreeVerification (tree);
    
        RETURN NO_TREE_ERRORS;
    }
    
    template <typename T>
    TreeError InitNode_ (Node <T> **node, CallData callData) {
        PushLog (3);
    
        *node = (Node <T> *) calloc (1, sizeof (Node <T>));
      
        if (!(*node)) {
            RETURN NULL_NODE_POINTER;
        }
    
        RETURN NO_TREE_ERRORS;
    }
    
    template <typename T>
    TreeError DestroyTree_ (Tree <T> *tree) {
        PushLog (3);
        
        custom_assert (tree, pointer_is_null, NULL_TREE_POINTER);
        
        TreeError errors = DestroySubtreeNode_ (tree, tree->root);
    
        tree->root = NULL;
        tree->creationData = {};
    
        RETURN errors;
    }
    
    template <typename T>
    TreeError DestroySubtreeNode_ (Tree <T> *tree, Node <T> *node) {
        PushLog (3);
        
        custom_assert (tree, pointer_is_null, NULL_TREE_POINTER);
        custom_assert (node, pointer_is_null, NULL_NODE_POINTER);
    
        if (node->left)
            WriteError (tree, DestroySubtreeNode_ (tree, node->left));
    
        if (node->right)
            WriteError (tree, DestroySubtreeNode_ (tree, node->right));
        
        WriteError (tree, DestroySingleNode_ (node));
    
        RETURN tree->errors;
    }
    
    template <typename T>
    TreeError DestroySingleNode_ (Node <T> *node) {
        PushLog (3);
    
        if (!node) {
            RETURN NULL_ROOT_POINTER;
        }
    
        free (node);
    
        RETURN NO_TREE_ERRORS;
    }
    
    template <typename T>
    TreeError AddNode_ (Tree <T> *tree, Node <T> *leaf, TreeEdge direction, CallData callData) {
        PushLog (3);
        
        custom_assert (tree,   pointer_is_null, NULL_TREE_POINTER);
        custom_assert (leaf,   pointer_is_null, NULL_NODE_POINTER);

        TreeVerification (tree);
    
        Node <T> **child = NULL;
    
        switch (direction) {
            case LEFT_CHILD:
                child = &(leaf->left);
                break;
    
            case RIGHT_CHILD:
                child = &(leaf->right);
                break;
    
            case PARENT_NODE:
            case NO_EDGE:
            default:
                RETURN NO_TREE_ERRORS;
                break;
        }
    
        if (*child) {
            RETURN NODE_ALREADY_USED;
        }
    
        Node <T> *newNode = {};
        WriteError (tree, InitNode_ (&newNode, callData));
    
        newNode->parent = leaf;
        newNode->left   = NULL;
        newNode->right  = NULL;
        *child          = newNode;
    
        RETURN tree->errors;
    }
    
    template <typename T>
    TreeError DeleteNode_ (Tree <T> *tree, Node <T> *nodePointer, CallData callData) {
        PushLog (3);
        
        custom_assert (tree,        pointer_is_null, NULL_TREE_POINTER);
        custom_assert (nodePointer, pointer_is_null, NULL_NODE_POINTER);
    
        if (nodePointer->parent) {
            if (nodePointer == nodePointer->parent->left) {
                nodePointer->parent->left = NULL;
    
            }else if (nodePointer == nodePointer->parent->right) {
                nodePointer->parent->right = NULL;
            }
        }
    
        WriteError (tree, DestroySubtreeNode_ (tree, nodePointer));
        RETURN tree->errors;
    }
    
    template <typename T>
    TreeError PrintTree_ (Tree <T> *tree, PrintType printType, FILE *stream, CallData callData) {
        PushLog (3);
    
        TreeVerification (tree);
    
        custom_assert (tree,   pointer_is_null, NULL_TREE_POINTER);
        custom_assert (stream, pointer_is_null, OUTPUT_ERROR);
    
        InternalPrint_ (tree->root, printType, stream);           
    
        RETURN tree->errors;
    }
    
    template <typename T>
    static TreeError InternalPrint_ (Tree <T> *tree, Node <T> *node, PrintType printType, FILE *stream) {
        PushLog (3);
        
        custom_assert (node,   pointer_is_null, NULL_NODE_POINTER);
        custom_assert (stream, pointer_is_null, OUTPUT_ERROR);
    
        fprintf (stream, "( ");
    
        if (printType == PREFIX_PRINT)
            WriteError (tree, PrintNodeValue (node));
        
        if (node->left)
            InternalPrint_ (node, printType, stream);
    
        if (printType == INFIX_PRINT)
            WriteError (tree, PrintNodeValue (node));
    
        if (node->right)
            InternalPrint_ (node, printType, stream);
    
        if (printType == POSTFIX_PRINT)
            WriteError (tree, PrintNodeValue (node));
        
        fprintf (stream, " )");

        RETURN NO_TREE_ERRORS;
    }
    
    
    #undef WriteError
    #undef ReturnError

}

#endif


