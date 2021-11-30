#ifndef AVL_TREE_WRAP_H
#define AVL_TREE_WRAP_H

#include "AvlTree.h"
#include "ModelGradeInfo.h"

class AvlTreeWrap 
{
    public:
        AVLTree<ModelGradeInfo, int> sub_zero_tree;
        Node<ModelGradeInfo, int>* minimal_zero_model_node;

        AvlTreeWrap() : sub_zero_tree(), minimal_zero_model_node(nullptr) {}
        ~AvlTreeWrap() = default;
};

#endif //AVL_TREE_WRAP_H