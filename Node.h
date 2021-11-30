#ifndef NODE_H
#define NODE_H

template <class Key, class Value>
class Node
{
    public:
        Key key;
        Value val;
        Node* parent;
        Node* left;
        Node* right;
        int height;
        
        Node (Key key, Value val):  key(key), val(val),
                                    parent(nullptr), left(nullptr), right(nullptr),
                                    height(0) {}   
        ~Node() = default;
};

#endif //NODE_H