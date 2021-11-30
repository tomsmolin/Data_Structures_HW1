#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "Node.h"

template <class Key, class Value>
class AVLTree
{
    public:
        Node <Key, Value>* root;

        AVLTree () : root(nullptr) {}
        ~AVLTree() = default;

        Node <Key, Value>* insert(Key key, Value val);
        void remove(Key key);
        void deleteTree (Node <Key, Value>* root);

        Node <Key, Value>* find(Key key);
        Node <Key, Value>* getMin();
        Node <Key, Value>* getMax();

        static int max (int left_height, int right_height);
        static void updateHeights(Node <Key, Value>* node);

    private:
    
        void removeRoot(Node <Key, Value>* node);
        void removeRootWithoutSons(Node <Key, Value>* node);
        void removeRootWithRightSon(Node <Key, Value>* node);
        void removeRootWithLeftSon(Node <Key, Value>* node);
        void removeRootWithTwoSons(Node <Key, Value>* node);

        void removeLeaf(Node <Key, Value>* node, bool is_right_son);
        void removeNodeWithRightSon(Node <Key, Value>* node, bool is_right_son);
        void removeNodeWithLeftSon(Node <Key, Value>* node, bool is_right_son);
        void removeNodeWithTwoSons(Node <Key, Value>* node, bool is_right_son);

        Node <Key, Value>* findSuccessor(Node <Key, Value>* node);
        int calculateBalanceFactor(Node <Key, Value>* node);

        void balanceInsert (Node <Key, Value>* node);
        void balanceRemove(Node <Key, Value>* node);
        void rightRotation (Node <Key, Value>* node);
        void leftRotation (Node <Key, Value>* node);
        void LRRotation(Node <Key, Value>* node);
        void RLRotation(Node <Key, Value>* node);
};

template <class Key, class Value>
Node <Key, Value>* AVLTree<Key, Value>::insert(Key key, Value val)
{
    Node <Key, Value>* new_node = new Node<Key, Value> (key, val);
    if (new_node == nullptr)
    {
        return nullptr;
    }

    if (root == nullptr)
    {
        root = new_node;
    }

    else 
    {
        Node <Key, Value>* sub_tree_root = root;

        while (sub_tree_root != nullptr)
        {
            if (key < sub_tree_root->key)
            {
                if (!sub_tree_root->left)
                {
                    sub_tree_root->left = new_node;
                    new_node->parent = sub_tree_root;
                    break;
                }   
                sub_tree_root = sub_tree_root->left; 
            }
            else
            {
                if (!sub_tree_root->right)
                {
                    sub_tree_root->right = new_node;
                    new_node->parent = sub_tree_root;
                    break;
                }  
                sub_tree_root = sub_tree_root->right; 
            }
        }
        balanceInsert(new_node);
    }
    return new_node; 
}

template <class Key, class Value>
void AVLTree<Key, Value>::remove(Key key)
{
    Node <Key, Value>* node = find(key);
    if (!node)
    {
        return;
    }
    
    if (node == root)
    {
        removeRoot(node);
        return;
    }

    bool is_right_son = false;
    if (node != root && node->parent->right == node)
    {
        is_right_son = true;
    }
    
    if (node->height == 0)
    {
        removeLeaf(node, is_right_son);
        return;
    }

    if (node->right && !node->left)
    {
        removeNodeWithRightSon(node, is_right_son);
        return;
    }

    if (!node->right && node->left)
    {
        removeNodeWithLeftSon(node, is_right_son);
        return;
    }
    else 
    {
        removeNodeWithTwoSons(node, is_right_son);
        return;
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::deleteTree(Node <Key, Value>* node)
{
    if (!node)
    {
        return;
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        node->parent = nullptr;
        delete node;
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);
    
    node->parent = nullptr;
    node->right = nullptr;
    node->left = nullptr;

    delete node;
    return;
}

template <class Key, class Value>
Node <Key, Value>* AVLTree<Key, Value>::find(Key key)
{
    Node <Key, Value>* iterator = root;
    while(iterator)
    {
        if (iterator->key == key)
        {
            return iterator;
        }

        if (key < iterator->key)
        {
            iterator = iterator->left;
            continue;
        }

        if (iterator->key < key)
        {
            iterator = iterator->right;
        }
    }
    return iterator;
}

template <class Key, class Value>
Node <Key, Value>* AVLTree<Key, Value>::getMin()
{
    Node <Key, Value>* node = root;
    while (node)
    {
        if (node->left == nullptr)
        {
            return node;
        }
        node = node->left;
    }

    return nullptr;
}

template <class Key, class Value>
Node <Key, Value>* AVLTree<Key, Value>::getMax()
{
    Node <Key, Value>* node = root;
    while (node)
    {
        if (node->right == nullptr)
        {
            return node;
        }
        node = node->right;
    }

    return nullptr;
}

template <class Key, class Value>
int AVLTree<Key, Value>::max (int left_height, int right_height)
{
    return left_height >= right_height ? left_height : right_height;
}

template <class Key, class Value>
void AVLTree<Key, Value>::updateHeights(Node <Key, Value>* node)
{
    if (node == nullptr)
    {
        return;
    }
    
    int left_height = -1;
    int right_height = -1;
    if (node->left != nullptr)
    {
        left_height = node->left->height;
    }
    if (node->right != nullptr)
    {
        right_height = node->right->height;
    }
    
    node->height = max(left_height, right_height) + 1;
    if (node->height == -1)
    {
        node->height = 0;
    }
    return;
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeRoot(Node <Key, Value>* node)
{
    if (node->height == 0)
    {
        removeRootWithoutSons(node);
        return;
    }

    if (node->right && !node->left)
    {
        removeRootWithRightSon(node);
        return;

    }
    if (!node->right && node->left)
    {
        removeRootWithLeftSon(node);
        return;
    }
    else
    {
        removeRootWithTwoSons(node);
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeRootWithoutSons(Node <Key, Value>* node)
 {
    delete root;
    root = nullptr;
    return;
 }

template <class Key, class Value>
void AVLTree<Key, Value>::removeRootWithRightSon(Node <Key, Value>* node)
{
    root = node->right;
    root->parent = nullptr;
    node->right = nullptr;
    delete node;
    return;
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeRootWithLeftSon(Node <Key, Value>* node)
{
    root = node->left;
    root->parent = nullptr;
    node->left = nullptr;
    delete node;
    return;
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeRootWithTwoSons(Node <Key, Value>* node)
{
    Node <Key, Value>* successor = findSuccessor(node);
    Node <Key, Value>* successor_right_son = successor->right;
    Node <Key, Value>* successor_parent = successor->parent;

    if (successor_parent == root)
    {
        successor->left = node->left;
        successor->left->parent = successor;
        
        successor->parent = nullptr;
        
    }
    else
    {
        successor->parent = nullptr;

        successor->left = node->left;
        successor->left->parent = successor;

        successor->right = node->right;
        successor->right->parent = successor;

        successor_parent->left = successor_right_son;
        if (successor_right_son)
        {
            successor_right_son->parent = successor_parent;
        }
        
    }
    root = successor;
    updateHeights(successor_parent);
    updateHeights(successor);

    node->right = nullptr;
    node->left = nullptr;
    delete node;
    return;
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeLeaf(Node <Key, Value>* node, bool is_right_son)
{
    if (is_right_son)
    {
        node->parent->right = nullptr;
    }
    if (!is_right_son)
    {
        node->parent->left = nullptr;
    }

    Node <Key, Value>* iterator = node->parent;
    while (iterator)
    {
        updateHeights(iterator);
        iterator = iterator->parent;  
    }
    balanceRemove(node);
    node->parent = nullptr;
    node->right = nullptr;
    node->left = nullptr;
    delete node;
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeNodeWithRightSon(Node <Key, Value>* node, bool is_right_son)
{
    if (is_right_son)
    {
        node->parent->right = node->right;
    }
    else
    {
        node->parent->left = node->right;  
    }
    node->right->parent = node->parent;

    Node <Key, Value>* iterator = node->parent;
    while (iterator)
    {
        updateHeights(iterator);
        iterator = iterator->parent;  
    }
    balanceRemove(node);
    node->parent = nullptr;
    node->right = nullptr;
    node->left = nullptr;
    delete node;
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeNodeWithLeftSon(Node <Key, Value>* node, bool is_right_son)
{
    if (is_right_son)
    {
        node->parent->right = node->left;
    }
    else
    {
        node->parent->left = node->left;
    }
    node->left->parent = node->parent;

    Node <Key, Value>* iterator = node->parent;
    while (iterator)
    {
        updateHeights(iterator);
        iterator = iterator->parent;  
    }
    balanceRemove(node);
    node->parent = nullptr;
    node->right = nullptr;
    node->left = nullptr;
    delete node;
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeNodeWithTwoSons(Node <Key, Value>* node, bool is_right_son)
{
    Node <Key, Value>* successor = findSuccessor(node);
    Node <Key, Value>* successor_right_son = successor->right;
    Node <Key, Value>* successor_parent = successor->parent;

    if (successor_parent == node)
    {
        successor->left = node->left;
        successor->left->parent = successor;

        successor->parent = node->parent;
        if (is_right_son)
        {
            node->parent->right = successor;
        }
        else
        {
            node->parent->left = successor;
        }
    }
    else
    {
        successor->parent = node->parent;
        if (is_right_son)
        {
            node->parent->right = successor;
        }
        else
        {
            node->parent->left = successor;
        }
        successor->left = node->left;
        successor->left->parent = successor;
        
        successor->right = node->right; 
        successor->right->parent = successor;

        successor_parent->left = successor_right_son;
        if (successor_right_son)
        {
            successor_right_son->parent = successor_parent; 
        }
    }
    updateHeights(successor_parent);
    updateHeights(successor);

    Node <Key, Value>* iterator = node->parent;
    while (iterator)
    {
        updateHeights(iterator);
        iterator = iterator->parent;  
    }
    balanceRemove(node);
    node->parent = nullptr;
    node->right = nullptr;
    node->left = nullptr;
    delete node;
}

template <class Key, class Value>
Node <Key, Value>* AVLTree<Key, Value>::findSuccessor(Node <Key, Value>* node)
{
    Node <Key, Value>* iterator = nullptr;
    if (node->right)
    {
        iterator = node->right;
        while (iterator->left)
        {
            iterator = iterator->left;
        }
        return iterator;
    }

    if (node->parent)
    {
        iterator = node;
        while (iterator)
        {
            if (iterator == root)
            {
                return nullptr;
            }
            if (iterator->parent->left == iterator)
            {
                return iterator->parent;
            }

            iterator = iterator->parent;
        }
    }
    return iterator;
}

template <class Key, class Value>
int AVLTree<Key, Value>::calculateBalanceFactor(Node <Key, Value>* node)
{
    int left_height = -1;
    int right_height = -1;
    if (node->left != nullptr)
    {
        left_height = node->left->height;
    }
    if (node->right != nullptr)
    {
        right_height = node->right->height;
    }
    int balance_factor = left_height - right_height;
    return balance_factor;
}

template <class Key, class Value>
void AVLTree<Key, Value>::balanceInsert(Node <Key, Value>* node)
{
    Node <Key, Value>* parent = nullptr;
    while (node != nullptr)
    {
        parent = node->parent;
        if (!parent)
        {
            return;
        }
        if (parent->height >= node->height + 1)
        {
            return;
        }  
       parent->height = node->height + 1;
        
        int balance_factor = calculateBalanceFactor(parent);
        if (balance_factor < -1 || 1 < balance_factor)
        {
            if (balance_factor == 2)
            {
                int balance_factor_left_v = calculateBalanceFactor(parent->left);
                if (balance_factor_left_v >= 0)
                {
                    // LL Rotation
                    rightRotation(parent);
                    return;
                }
                if (balance_factor_left_v == -1)
                {
                    LRRotation(parent);
                    return;
                } 
            }

            if (balance_factor == -2)
            {
                int balance_factor_right_v = calculateBalanceFactor(parent->right);
                if (balance_factor_right_v <= 0)
                {
                    // RR Rotation
                    leftRotation(parent);
                    return;
                }

                if (balance_factor_right_v == 1)
                {
                    RLRotation(parent);
                    return;
                }
            }
        }
        node = parent;
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::balanceRemove(Node <Key, Value>* node)
{
    while (node != nullptr)
    {
        if (!node->parent)
        {
            return;
        }
        int balance_factor = calculateBalanceFactor(node->parent);
        int old_height = node->parent->height;
        if (balance_factor < -1 || 1 < balance_factor)
        {
            if (balance_factor == 2)
            {
                int balance_factor_left_v = calculateBalanceFactor(node->parent->left);
                if (balance_factor_left_v >= 0)
                {
                    // LL Rotation
                    rightRotation(node->parent);
                }
                if (balance_factor_left_v == -1)
                {
                    LRRotation(node->parent);
                } 
            }
        
            if (balance_factor == -2)
            {
                int balance_factor_right_v = calculateBalanceFactor(node->parent->right);
                if (balance_factor_right_v <= 0)
                {
                    // RR Rotation
                    leftRotation(node->parent);
                }

                if (balance_factor_right_v == 1)
                {
                    RLRotation(node->parent);
                }
            }
            if(node->parent && old_height == node->parent->height)
            {
                return;
            }
        }
        node = node->parent;
    }
    updateHeights(root);
}

/* LL - rotation to right through un-balanced node*/
template <class Key, class Value>
void AVLTree<Key, Value>::rightRotation (Node <Key, Value>* x)
{
    Node<Key, Value>* temp = x->parent;
    Node<Key, Value>* y = x->left;
    x->parent = y;
    x->left = y->right;
    
    if (x->left != nullptr)
    {
        x->left->parent = x;
    }
    
    y->right = x;
    y->parent = temp;
    updateHeights(x);
    updateHeights(y);

    // In case x is root
    if (temp == nullptr)
    {
        root = y;
    }
    else if (temp->right == x)
    {
        temp->right = y;
    }
    else
    {
        temp->left = y;
    }
    return;
}

/* RR - rotation to left through un-balanced node*/
template <class Key, class Value>
void AVLTree<Key, Value>::leftRotation (Node <Key, Value>* x)
{
    Node<Key, Value>* temp = x->parent;
    Node<Key, Value>* y = x->right;
    x->parent = y;
    x->right = y->left;

    if (x->right != nullptr)
    {
        x->right->parent = x;
    }
    y->left = x;
    y->parent = temp;
    
    updateHeights(x);
    updateHeights(y);
    // In case x is root
    if (temp == nullptr)
    {
        root = y;
    }
    else if (temp->right == x)
    {
        temp->right = y;
    }
    else
    {
        temp->left = y;
    }
    return;   
}

/*
** LR - first using leftRotation through left-son-of-un-balanced node,
**      completing LR with regular rightRotation through un-balanced node.
*/
template <class Key, class Value>
void AVLTree<Key, Value>::LRRotation(Node <Key, Value>* node)
{
    leftRotation(node->left);
    rightRotation(node);
}

/* 
** RL - first using rightRotation through right-son-of-un-balanced node,
**      completing RL with regular leftRotation through un-balanced node.
*/
template <class Key, class Value>
void AVLTree<Key, Value>::RLRotation(Node <Key, Value>* node)
{
    rightRotation(node->right);
    leftRotation(node);
}

#endif //AVL_Tree_H