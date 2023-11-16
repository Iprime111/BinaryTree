#ifndef TREE_DUMP_H_
#define TREE_DUMP_H_

#include "stdio.h"

#include "CustomAssert.h"
#include "Logger.h"
#include "TreeDefinitions.h"
#include "Buffer.h"
#include "TreeDumpColors.h"

#define WriteDataWithErrorCheck(buffer, data)                                                   \
            do {                                                                                \
                if (WriteStringToBuffer (buffer, data) != BufferErrorCode::NO_BUFFER_ERRORS) {  \
                    DestroyBuffer (buffer);                                                     \
                    RETURN LOG_ERROR;                                                           \
                }                                                                               \
            } while (0)

namespace Tree {
    const size_t MAX_TREE_NODE_INDEX_SIZE = 32;
    const size_t MAX_TREE_NODE_DATA_SIZE  = 256;

    template <typename T>
    static TreeError DumpNodeConnection    (Node <T> *node, TreeEdge direction, char *color,  Buffer <char> *graphvizBuffer);
    template <typename T>
    static TreeError DumpTreeNodeRecursive (Tree <T> *tree, Node <T> *node, Buffer <char> *graphvizBuffer);
    template <typename T>
    static TreeError DumpTreeHeader (Tree <T> *tree, Buffer <char> *graphvizBuffer);

    template <typename T>
    TreeError TreeDump_ (Tree <T> *tree, CallData callData) {
        PushLog (3);

        custom_assert (tree, pointer_is_null, NULL_TREE_POINTER);

        Buffer <char> graphvizBuffer = {};
        InitBuffer (&graphvizBuffer, 1);

        DumpTreeHeader (tree, &graphvizBuffer);
        
        if (tree->root)
            DumpTreeNodeRecursive (tree, tree->root, &graphvizBuffer);

        WriteDataWithErrorCheck (&graphvizBuffer, "}");

        FILE *dumpFile = fopen ("dump.dot", "w");
        fwrite (graphvizBuffer.data, sizeof (char), graphvizBuffer.currentIndex, dumpFile);
        fclose (dumpFile);

        DestroyBuffer (&graphvizBuffer);

        RETURN NO_TREE_ERRORS;
    }

    template <typename T>
    static TreeError DumpTreeHeader (Tree <T> *tree, Buffer <char> *graphvizBuffer) {
        PushLog (3);
        
        custom_assert (graphvizBuffer, pointer_is_null, LOG_ERROR);
        custom_assert (tree, pointer_is_null, NULL_TREE_POINTER);

        WriteDataWithErrorCheck (graphvizBuffer, "digraph {\n\tbgcolor=\"" DUMP_BACKGROUND_COLOR "\"\n");

        RETURN NO_TREE_ERRORS;
    }

    template <typename T>
    static TreeError DumpTreeNodeRecursive (Tree <T> *tree, Node <T> *node, Buffer <char> *graphvizBuffer) {
        PushLog (3);

        custom_assert (tree,           pointer_is_null, NULL_TREE_POINTER);
        custom_assert (node,           pointer_is_null, NULL_NODE_POINTER);
        custom_assert (graphvizBuffer, pointer_is_null, LOG_ERROR);

        char nodeIndexBuffer [MAX_TREE_NODE_INDEX_SIZE] = "";
        snprintf (nodeIndexBuffer, MAX_TREE_NODE_INDEX_SIZE, "%lu", (unsigned long) node);

        WriteDataWithErrorCheck (graphvizBuffer, "\t");
        WriteDataWithErrorCheck (graphvizBuffer, nodeIndexBuffer);
        WriteDataWithErrorCheck (graphvizBuffer, " [style=\"filled,rounded\" fillcolor=\"" DUMP_NODE_COLOR "\" shape=\"record\" color=\"" DUMP_NODE_OUTLINE_COLOR "\" label=\"{");

        char nodeDataBuffer [MAX_TREE_NODE_DATA_SIZE] = "";
        SnprintNodeValue (node, nodeDataBuffer, MAX_TREE_NODE_DATA_SIZE);

        WriteDataWithErrorCheck (graphvizBuffer, nodeDataBuffer);
        WriteDataWithErrorCheck (graphvizBuffer, " | {<left> left | <right> right}}\"];\n");

        if (node->left) {
            DumpNodeConnection    (node, LEFT_CHILD, DUMP_NEXT_CONNECTION_COLOR, graphvizBuffer);
            DumpTreeNodeRecursive (tree, node->left, graphvizBuffer);
        }

        if (node->right) {
            DumpNodeConnection    (node, RIGHT_CHILD, DUMP_NEXT_CONNECTION_COLOR, graphvizBuffer);
            DumpTreeNodeRecursive (tree, node->right,  graphvizBuffer);
        }

        RETURN tree->errors;
    }
    
    template <typename T>
    static TreeError DumpNodeConnection (Node <T> *node, TreeEdge direction, char *color, Buffer <char> *graphvizBuffer) {
        PushLog (3);

        custom_assert (node,           pointer_is_null, NULL_NODE_POINTER);
        custom_assert (graphvizBuffer, pointer_is_null, LOG_ERROR);

        char nodeIndexBuffer [MAX_TREE_NODE_INDEX_SIZE] = "";

        WriteDataWithErrorCheck (graphvizBuffer, "\t");
    
        snprintf (nodeIndexBuffer, MAX_TREE_NODE_INDEX_SIZE, "%lu", (unsigned long) node);
        WriteDataWithErrorCheck (graphvizBuffer, nodeIndexBuffer);

        Node <T> *nextNode = NULL;

        if (direction == LEFT_CHILD) {
            WriteDataWithErrorCheck (graphvizBuffer, ":left");
            nextNode = node->left;
        } else if (direction == RIGHT_CHILD) {
            WriteDataWithErrorCheck (graphvizBuffer, ":right");
            nextNode = node->right;
        } else if (direction == PARENT_NODE) {
            nextNode = node->parent;
        }

        WriteDataWithErrorCheck (graphvizBuffer, " -> ");
    
        snprintf (nodeIndexBuffer, MAX_TREE_NODE_INDEX_SIZE, "%lu", (unsigned long) nextNode);
        WriteDataWithErrorCheck (graphvizBuffer, nodeIndexBuffer);

        WriteDataWithErrorCheck (graphvizBuffer, " [color=\"");
        WriteDataWithErrorCheck (graphvizBuffer, color);
        WriteDataWithErrorCheck (graphvizBuffer, "\"];\n");

        RETURN NO_TREE_ERRORS;        
    }
}

#endif
