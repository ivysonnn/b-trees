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
    if (q == nullptr ||
        q == this->root) // no such element to remove or can't remove the root
        return false;

    // CASE 3 that needs to come before 1 and 2
    if (q->left != nullptr && q->right != nullptr) 
    {
        // Find the successor
        NodeT *succ = q->right;
        while (succ->left != nullptr)
            succ = succ->left;

        q->data = succ->data;

        q = succ;
    }

    // CASE 1
    if (q->right == nullptr && q->left == nullptr) 
    {
        if (q->parent->left == q)
            q->parent->left = nullptr;
        else if (q->parent->right == q)
            q->parent->right = nullptr;

        goto remove; // GOTO
    }

    // CASE 2.1
    else if (q->right) 
    {
        if (q->parent->right == q)
            q->parent->right = q->right;
        else if (q->parent->left == q)
            q->parent->left = q->right;

        if (q->right)
            q->right->parent = q->parent;

        goto remove;
    }

    // CASE 2.2
    else if (q->left) 
    {
        if (q->parent->right == q)
            q->parent->right = q->left;
        else if (q->parent->left == q)
            q->parent->left = q->left;

        if (q->left)
            q->left->parent = q->parent;

        goto remove;
    }

    remove:
        delete q;
        return true;
}

template <typename T, typename NodeT> NodeT* BTree<T, NodeT>::search(T value)
{
    NodeT* r = this->root;
    while(!r)
    {
        if(value == r->data)
            return r;
        else if(value > r->data)
            r = r->right;
        else
            r = r->left;
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