#pragma once

#include <edb/btree.h>

namespace edb
{
    enum Color
    {
        R,
        B,
    };

    template <typename T> struct RBNode : public node<T, RBNode<T>> 
    {
        Color color;
        RBNode(const T &value) : node<T, RBNode<T>>(value), color(R) {}
    };
                            // VASCO DA GAMA >>>>>
    template <typename T> class RBTree : public BTree<T, RBNode<T>>
    {
        public:
            RBTree() : BTree<T, RBNode<T>>() {}

            void insertAndFix(T value) override;
            void removeAndFix(T value) override;

        private:
            void insertionCases(RBNode<T>* node);
            RBNode<T>* remotionCases(RBNode<T>* node);
    };
}

// implementation

template <typename T> void edb::RBTree<T>::insertionCases(RBNode<T> *node) {
    RBNode<T> *parent = node->parent;
    RBNode<T> *grandparent = parent->parent;
    if (!grandparent)
    {
        node = parent;
        return;
    }
    RBNode<T> *uncle =
        (parent == grandparent->right) ? grandparent->left : grandparent->right;

    if (uncle &&
        (parent->color == R && uncle->color == R)) // CASE 1
    {
        parent->color = B;
        uncle->color = B;
        node = grandparent;
    } 
    else if (parent->color == R) // WILL HAVE ALL THE OTHER CASES
    {
        if (parent == grandparent->left && node == parent->left) // CASE 2
        {
            this->rotateRight(grandparent);
            parent->color = B;
            grandparent->color = R;
            node = parent;
        }
        else if (parent == grandparent->left && node == parent->right) // CASE 3
        {
            this->rotateLeft(parent);
            this->rotateRight(grandparent);

            parent->color = B;
            grandparent->color = R;
            
            node = parent;
        }
        else if(parent == grandparent->right && node == parent->right) // CASE 4
        {
            this->rotateLeft(grandparent);
            parent->color = B;
            grandparent->color = R;
            node = parent;
        }
        else if(parent == grandparent->right && node == parent->left) // CASE 5
        {
          this->rotateRight(parent);

          this->rotateLeft(grandparent);

          parent->color = B;
          grandparent->color = R;
          node->color = B;

          node = parent;
        }
    }

    node->color = (node == this->root) ? B : R;
}

template <typename T>
edb::RBNode<T> *edb::RBTree<T>::remotionCases(RBNode<T> *node) 
{
    RBNode<T> *parent = node->parent;

    while (node != this->root && node->color == B) 
    {
        if (parent->left == node) {
        RBNode<T> *sibling = parent->right;

        if (sibling && sibling->color == R) // CASE 1
        {
            sibling->color = B;
            parent->color = R;
            if (parent->left == node)
                this->rotateLeft(parent);
            else
                this->rotateRight(parent);
            sibling = (parent->left == node) ? parent->right : parent->left;
        }

        if ((!sibling->left || sibling->left->color == B) &&
            (!sibling->right || sibling->right->color == B)) // CASE 2
        {
            if (sibling)
                sibling->color = R;
            node = parent;
            parent = node->parent;
        } 
        else 
        {
            if (!sibling->right || sibling->right->color == B) // CASE 3
            {
                if (sibling->left)
                    sibling->left->color = B;
                sibling->color = R;
                this->rotateRight(sibling);
                sibling = parent->right;
            }

            sibling->color = parent->color; // CASE 4
            parent->color = B;
            if (sibling->right)
                sibling->right->color = B;
            this->rotateLeft(parent);
            node = this->root;
        }
        } else 
        {
        RBNode<T> *sibling = parent->left;

        if (sibling && sibling->color == R) // CASE 1 mirror
        {
            sibling->color = B;
            parent->color = R;
            this->rotateRight(parent);
            sibling = parent->left;
        }

        if ((!sibling->left || sibling->left->color == B) &&
            (!sibling->right || sibling->right->color == B)) // CASE 2 mirror
        {
            if (sibling)
                sibling->color = R;
            node = parent;
            parent = node->parent;
        } else {
            if (!sibling->left || sibling->left->color == B) // CASE 3 mirror
            {
            if (sibling->right)
                sibling->right->color = B;
            sibling->color = R;
            this->rotateLeft(sibling);
            sibling = parent->left;
            }

            sibling->color = parent->color; // CASE 4 mirror
            parent->color = B;
            if (sibling->left)
              sibling->left->color = B;
            this->rotateRight(parent);
            node = this->root;
        }
        }
    }

    if (node)
        node->color = B;
    return node;
}

template <typename T> void edb::RBTree<T>::insertAndFix(T value) 
{
    RBNode<T> *node = new RBNode<T>(value);
    if (this->insert(node) == false) // will return true if a insertion was made
    {                   
        delete node;             //  or if the new node equals the root
        return;
    }

    if(node == this->root)
    {
        this->root->color = B;
        return;
    }
    // balances
    while (node != this->root && node->parent && node->parent->color == R)
        insertionCases(node);

    this->root->color = B;
}

template <typename T> void edb::RBTree<T>::removeAndFix(T value) 
{
    RBNode<T> *node = this->search(value);
    if (!node)
        return;

    if (node == this->root) 
    {
        return;
    }

    if (!this->remove(value))
        return;

    // balances
    RBNode<T> *fixNode = node->parent ? node->parent : this->root;
    while (fixNode && fixNode != this->root && fixNode->color == B)
        fixNode = remotionCases(fixNode);

    if (this->root)
        this->root->color = B;
}
