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
  RBNode<T> *z = this->search(value);
  if (!z)
    return;

  RBNode<T> *y = z;
  CColor yOriginalColor = y->color;
  RBNode<T> *x = nullptr;

  auto transplant = [&](RBNode<T> *u, RBNode<T> *v) {
    if (!u->parent)
      this->root = v;
    else if (u == u->parent->left)
      u->parent->left = v;
    else
      u->parent->right = v;
    if (v)
      v->parent = u->parent;
  };

  auto subtree_minimum = [&](RBNode<T> *n) -> RBNode<T> * {
    while (n && n->left)
      n = n->left;
    return n;
  };

  if (!z->left) {
    x = z->right;
    transplant(z, z->right);
  } else if (!z->right) {
    x = z->left;
    transplant(z, z->left);
  } else {
    y = subtree_minimum(z->right);
    yOriginalColor = y->color;
    x = y->right;
    if (y->parent == z) {
      if (x)
        x->parent = y;
    } else {
      transplant(y, y->right);
      y->right = z->right;
      if (y->right)
        y->right->parent = y;
    }
    transplant(z, y);
    y->left = z->left;
    if (y->left)
      y->left->parent = y;
    y->color = z->color;
  }

  // não delete z até terminar de determinar/fixar x; mas z já está fora da
  // árvore após transplant
  delete z;

  if (yOriginalColor == B) {
    while (x != this->root && (!x || x->color == B)) {
      RBNode<T> *p = x ? x->parent : nullptr;
      if (!p)
        break;
      bool xIsLeft = (x == p->left);
      RBNode<T> *s = xIsLeft ? p->right : p->left;

      if (s && s->color == R) {
        s->color = B;
        p->color = R;
        if (xIsLeft) {
          this->rotateLeft(p);
          s = p->right;
        } else {
          this->rotateRight(p);
          s = p->left;
        }
      }

      if (!s || ((!s->left || s->left->color == B) &&
                 (!s->right || s->right->color == B))) {
        if (s)
          s->color = R;
        x = p;
      } else {
        if (xIsLeft) {
          if (s->right && s->right->color == R) {
            s->color = p->color;
            p->color = B;
            s->right->color = B;
            this->rotateLeft(p);
            x = this->root;
          } else {
            if (s->left && s->left->color == R) {
              s->left->color = B;
              s->color = R;
              this->rotateRight(s);
              s = p->right;
            }
            s->color = p->color;
            p->color = B;
            if (s->right)
              s->right->color = B;
            this->rotateLeft(p);
            x = this->root;
          }
        } else {
          if (s->left && s->left->color == R) {
            s->color = p->color;
            p->color = B;
            s->left->color = B;
            this->rotateRight(p);
            x = this->root;
          } else {
            if (s->right && s->right->color == R) {
              s->right->color = B;
              s->color = R;
              this->rotateLeft(s);
              s = p->left;
            }
            s->color = p->color;
            p->color = B;
            if (s->left)
              s->left->color = B;
            this->rotateRight(p);
            x = this->root;
          }
        }
      }
    }
    if (x)
      x->color = B;
  }
  if (this->root)
    this->root->color = B;
}
