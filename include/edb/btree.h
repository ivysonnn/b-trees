#pragma once

template <typename T, typename NodeT> struct node
{
    T data;
    NodeT* parent;
    NodeT* left;
    NodeT* right;

    node(const T& value) : data(value), parent(nullptr), left(nullptr), right(nullptr) {} 
};

template <typename T, typename NodeT> class BTree
{
    public:
        BTree() : root(nullptr) {}
        NodeT* getRoot() {return root;}

        virtual void insertAndFix(T value) = 0;
        virtual void removeAndFix(T value) = 0;

        NodeT *root;

      protected:
        bool insert(NodeT *value); // return true if a insertion was made
        bool remove(T value);
        NodeT* search(T value);

        void rotateLeft(NodeT *node);
        void rotateRight(NodeT *node);
        void rotateLeftLeft(NodeT *node);
        void rotateRightRight(NodeT *node);
};

// implementation

template <typename T, typename NodeT> 
bool BTree<T, NodeT>::insert(NodeT* node)
{
    if(this->root == nullptr)
    {
        this->root = node;
        return true;
    }

    NodeT* r = this->root;
    T data;
    
    while (true) 
    {
        data = node->data;
        if (data == r->data) // we're not allowing equal elements.
            return false;

        if (data > r->data) {
            if (r->right == nullptr) {
            r->right = node;
            node->parent = r;
            return true;
            }
            r = r->right;
            continue;
        }

            if (data < r->data) {
                if (r->left == nullptr) 
                {
                    r->left = node;
                    node->parent = r;
                    return true;
                }
                r = r->left;
                continue;
            }
    }
}

template <typename T, typename NodeT> bool BTree<T, NodeT>::remove(T value) {
  NodeT *q = search(value);
  if (!q)
    return false;

  auto transplant = [&](NodeT *u, NodeT *v) {
    if (!u->parent)
      this->root = v;
    else if (u == u->parent->left)
      u->parent->left = v;
    else
      u->parent->right = v;
    if (v)
      v->parent = u->parent;
  };

  if (!q->left) {
    transplant(q, q->right);
    delete q;
    return true;
  } else if (!q->right) {
    transplant(q, q->left);
    delete q;
    return true;
  } else {
    NodeT *succ = q->right;
    while (succ->left)
      succ = succ->left;

    if (succ->parent != q) {
      transplant(succ, succ->right);
      succ->right = q->right;
      if (succ->right)
        succ->right->parent = succ;
    }
    transplant(q, succ);
    succ->left = q->left;
    if (succ->left)
      succ->left->parent = succ;

    delete q;
    return true;
  }
}

template <typename T, typename NodeT> NodeT* BTree<T, NodeT>::search(T value)
{
    NodeT* r = this->root;
    while (r) 
    {
      if (value == r->data)
        return r;
      r = (value > r->data) ? r->right : r->left;
    }
    return nullptr;
}

template <typename T, typename NodeT> void BTree<T, NodeT>::rotateLeft(NodeT *q) {
    if (!q->right) // check if the rotation is valid
        return;

    NodeT *p = q->right;
    NodeT *pl = p->left;

    q->right = pl;
    if (pl) 
    {
        pl->parent = q;
    }

    NodeT *oldParent = q->parent;
    p->parent = oldParent;

    if (!oldParent) {
        this->root = p;
    } else if (q == oldParent->right) {
        oldParent->right = p;
    } else {
        oldParent->left = p;
    }

    q->parent = p;
    p->left = q;
}

template <typename T, typename NodeT> void BTree<T, NodeT>::rotateRight(NodeT *q) 
{
    if (!q->left) // check if the rotation is valid
        return; 

    NodeT *p = q->left;
    NodeT *pr = p->right;

    q->left = pr;
    if (pr) 
    {
        pr->parent = q;
    }

    NodeT *oldParent = q->parent;
    p->parent = oldParent;

    if (!oldParent) 
    {
        this->root = p;
    } 
    else if (q == oldParent->right)
    {
      oldParent->right = p;
    } 
    else 
    {
      oldParent->left = p;
    }

    q->parent = p;
    p->right = q;
}