#pragma once

#include <edb/btree.h>
#include <utility>

namespace edb
{
    enum CColor
    {
        R,
        B,
    };

    template <typename T> struct RBNode : public node<T, RBNode<T>> 
    {
        CColor color;
        RBNode(const T &value) : node<T, RBNode<T>>(value), color(R) {}
    };
                            // VASCO DA GAMA >>>>>
    template <typename T> class RBTree : public BTree<T, RBNode<T>>
    {
        public:
            RBTree() : BTree<T, RBNode<T>>() {}

            void insertAndFix(T value) override;
            void removeAndFix(T value) override;
    };
}

// implementation

template <typename T> void edb::RBTree<T>::insertAndFix(T value) 
{
    RBNode<T> *node = new RBNode<T>(value);
    if (this->insert(node) == false) {
        delete node;
        return;
    }

    if (node == this->root) {
        node->color = B;
        return;
    }

    while (node != this->root) { // conitue the loop until node == root
        RBNode<T> *parent = node->parent;
        
        if (parent->color == B) // dont need balance
            break;

        RBNode<T> *grandparent = parent->parent;
        if (!grandparent) { 
             break;
        }
        
        bool parentOnLeft = (grandparent->left == parent);
        RBNode<T> *uncle = parentOnLeft ? grandparent->right : grandparent->left;

        // CASE 1: UNCLE AND PARENT RED
        if (uncle && uncle->color == R) 
        {
            parent->color = B;
            uncle->color = B;
            grandparent->color = R;
            node = grandparent; 
            continue;
        }

        if (parentOnLeft) 
        {
            // CASE 3: Left-Right
            if (node == parent->right) 
            {
                this->rotateLeft(parent);
                std::swap(node, parent); 
            }
            
            // CASE 2: Left-Left
            parent->color = B;
            grandparent->color = R;
            this->rotateRight(grandparent);
        } else {
            // CASE 5: Right-Left
            if (node == parent->left) 
            {
                this->rotateRight(parent);
                std::swap(node, parent); 
            }
            
            parent->color = B;
            grandparent->color = R;
            this->rotateLeft(grandparent);
        }
    }

    this->root->color = B;
}

template <typename T> void edb::RBTree<T>::removeAndFix(T value) {
  RBNode<T> *x = this->search(value);
  if(x)
  {
    this->remove(value);
  }
  else 
  {
    return;
  }

    if (x->color == B) { 
        while (x != this->root && x->color == B) {
        RBNode<T> *p = x->parent;

        if (!p)
            break;

        bool xIsLeftChild = (x == p->left);
        RBNode<T> *s = xIsLeftChild ? p->right : p->left;

        if (s && s->color == R) {
            s->color = B;
            p->color = R;
            if (xIsLeftChild) {
            this->rotateLeft(p);
            s = p->right;
            } else {
            this->rotateRight(p);
            s = p->left; // O novo irmão
            }
        }
        // CASE 2
        if (s && (!s->left || s->left->color == B) &&
            (!s->right || s->right->color == B))
        {
            s->color = R;
            x = p;
        }
        else 
        {
            if (xIsLeftChild) {
            // CASE 3
            if (s->left && s->left->color == R &&
                (!s->right || s->right->color == B)) {
                s->left->color = B;
                s->color = R;
                this->rotateRight(s);
                s = p->right;
            }

            // CASE 3
            s->color = p->color;
            p->color = B;
            if (s->right)
                s->right->color = B;
            this->rotateLeft(p);
            }

            else {
            // CASE 3 MIRROR
            if (s->right && s->right->color == R &&
                (!s->left || s->left->color == B)) {
                s->right->color = B;
                s->color = R;
                this->rotateLeft(s);
                s = p->left; 
            }

            // CASE 4 MIRROR
            s->color = p->color;
            p->color = B;
            if (s->left)
                s->left->color = B;
            this->rotateRight(p);
            }

            x = this->root;
        }
        }
    }
    if (x)
        x->color = B;
    if (this->root)
        this->root->color = B;
}
