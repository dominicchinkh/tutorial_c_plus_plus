#pragma once

template <typename Object>
struct TreeNode
{
    // Keep the children of each node in a linked list of tree nodes
    Object element;
    TreeNode *firstChild;
    TreeNode *nextSibling;
};
