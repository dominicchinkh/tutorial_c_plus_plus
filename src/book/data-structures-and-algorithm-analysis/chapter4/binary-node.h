#ifndef BINARY_NODE_H
#define BINARY_NODE_H

template <typename Object>
struct BinaryNode
{
    Object element;
    BinaryNode *left;
    BinaryNode *right;
};

#endif
