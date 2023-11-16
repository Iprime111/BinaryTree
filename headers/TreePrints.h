#ifndef TREE_PRINTS_H_
#define TREE_PRINTS_H_

#include <stdio.h>

#include "TreeDefinitions.h"

namespace Tree {

    template <typename T>
    TreeError PrintNodeValue (Node <T> *node, FILE *stream) {
        fprintf (stream, "%x", node->nodeData);

        return NO_TREE_ERRORS;
    }

    template <typename T>
    TreeError SnprintNodeValue (Node <T> *node, char *buffer, size_t maxLength) {
        snprintf (buffer, maxLength, "%x", node->nodeData);

        return NO_TREE_ERRORS;
    }
}
#endif
