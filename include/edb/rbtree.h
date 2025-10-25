#pragma once

#include <edb/btree.h>
#include <utility>

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
            RBNode<T>* remotionCases(RBNode<T>* node);
    };
}

// implementation

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

template <typename T> 
void edb::RBTree<T>::insertAndFix(T value) 
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

    // Loop de balanceamento: Continua enquanto o nó não for a raiz
    while (node != this->root) {
        RBNode<T> *parent = node->parent;
        
        // C.1: Fim do balanceamento se o pai é preto (ou nó atingiu a raiz)
        if (parent->color == B)
            break;

        // O avô deve existir se o pai for vermelho (pois a raiz é preta).
        RBNode<T> *grandparent = parent->parent;
        if (!grandparent) { // Caso o parent seja a raiz e tenha sido pintado de R em algum Caso 1
             break;
        }
        
        bool parentOnLeft = (grandparent->left == parent);
        RBNode<T> *uncle = parentOnLeft ? grandparent->right : grandparent->left;

        // CASO 1: Recoloração quando tio é vermelho
        if (uncle && uncle->color == R) {
            parent->color = B;
            uncle->color = B;
            grandparent->color = R;
            node = grandparent; // Continua a correção a partir do avô
            continue;
        }

        // CASO 2, 3, 4 e 5: Rotações quando tio é preto (ou NIL)
        if (parentOnLeft) {
            // CASO 3: Left-Right (Rotação Dupla - Transforma em Left-Left)
            if (node == parent->right) {
                this->rotateLeft(parent);
                // CORREÇÃO ESSENCIAL: O antigo 'node' agora é o 'parent' e vice-versa.
                // O swap garante que a próxima etapa (Left-Left) use os ponteiros corretos.
                std::swap(node, parent); 
            }
            
            // CASO 2: Left-Left (Rotação Simples) ou Caso 3 transformado
            parent->color = B;
            grandparent->color = R;
            this->rotateRight(grandparent);
        } else {
            // CASO 5: Right-Left (Rotação Dupla - Transforma em Right-Right)
            if (node == parent->left) {
                this->rotateRight(parent);
                // CORREÇÃO ESSENCIAL: O swap re-alinha para o Case Right-Right
                std::swap(node, parent); 
            }
            
            // CASO 4: Right-Right (Rotação Simples) ou Caso 5 transformado
            parent->color = B;
            grandparent->color = R;
            this->rotateLeft(grandparent);
        }

        // Se uma rotação ocorreu (Casos 2, 3, 4, 5), a violação foi resolvida localmente.
        // O loop irá terminar na próxima iteração, pois o novo pai será Preto.
    }

    // Garante raiz sempre preta
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
