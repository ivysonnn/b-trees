#pragma once

#include "btree.h"

namespace edb
{
    // Nó
    template <typename T> struct AVLNode : public node<T, AVLNode<T>> 
    {
        int height;
        AVLNode(const T &value) : node<T, AVLNode<T>>(value), height(1) {}
    };

    // Classe
    template <typename T> class AVLTree : public BTree<T, AVLNode<T>>
    {
    public:
        AVLTree() : BTree<T, AVLNode<T>>() {}

        void insertAndFix(T value) override;
        void removeAndFix(T value) override;

        int getHeight(AVLNode<T> *node){
            return (node == nullptr) ? 0 : node->height;
        }

        int getBF(AVLNode<T> *node)
        {
            return getHeight(node->left) - getHeight(node->right);
        }

        void updateHeight(AVLNode<T> *node) {
            if (node != nullptr) {
                node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
            }
        }

    };
    
    template <typename T>
    void AVLTree<T>::insertAndFix(T value) {
        AVLNode<T> *newNode = new AVLNode<T>(value);

        if (!this->insert(newNode)) {
            // O valor já existe na árvore
            delete newNode; 
            return;
        }

        // Sobe na árvore a partir do nó inserido, atualizando alturas e balanceando
        AVLNode<T> *z = newNode;
        while (z != nullptr) {
            updateHeight(z);
            int balance = getBF(z);

            // Left-Left
            if (balance > 1 && value < z->left->data) {
                this->rotateRight(z);
                updateHeight(z->parent);
                updateHeight(z);
            }
            // Right-Right
            else if (balance < -1 && value > z->right->data) {
                this->rotateLeft(z);
                updateHeight(z->parent);
                updateHeight(z);
            }
            // Left-Right
            else if (balance > 1 && value > z->left->data) {
                this->rotateLeft(z->left);
                this->rotateRight(z);
                updateHeight(z->parent->left);
                updateHeight(z->parent->right);
                updateHeight(z->parent);
            }
            // Right-Left
            else if (balance < -1 && value < z->right->data) {
                this->rotateRight(z->right);
                this->rotateLeft(z);
                updateHeight(z->parent->left);
                updateHeight(z->parent->right);
                updateHeight(z->parent);
            }
            z = z->parent;
        }
    }

    template <typename T>
    void AVLTree<T>::removeAndFix(T value) {
        // Encontrar o nó a ser removido
        AVLNode<T>* node_to_remove = this->search(value);

        if (node_to_remove == nullptr) {
            // O valor não existe na árvore
            return;
        }

        // Encontrar o nó que será fisicamente removido e o ponto de partida para o rebalanceamento
        AVLNode<T>* physically_removed_node;
        AVLNode<T>* child;

        // Identifica o nó que será fisicamente removido
        if (node_to_remove->left == nullptr || node_to_remove->right == nullptr) {
            physically_removed_node = node_to_remove;
        } else {
            // Nó com dois filhos: encontra o sucessor (menor da subárvore direita)
            physically_removed_node = node_to_remove->right;
            while (physically_removed_node->left != nullptr) {
                physically_removed_node = physically_removed_node->left;
            }
            // Copia o dado do sucessor para o nó que queríamos remover
            node_to_remove->data = physically_removed_node->data;
        }

        // O nó fisicamente removido tem no máximo um filho
        child = (physically_removed_node->left != nullptr) ? physically_removed_node->left : physically_removed_node->right;

        // Relinka o filho (se existir) ao avô
        if (child != nullptr) {
            child->parent = physically_removed_node->parent;
        }

        // Ponto de partida para o rebalanceamento é o pai do nó fisicamente removido
        AVLNode<T>* z = physically_removed_node->parent;

        // Remove o nó da árvore
        if (physically_removed_node->parent == nullptr) {
            this->root = child;
        } else {
            if (physically_removed_node == physically_removed_node->parent->left) {
                physically_removed_node->parent->left = child;
            } else {
                physically_removed_node->parent->right = child;
            }
        }

        delete physically_removed_node;

        // Sobe na árvore a partir do ponto de partida, atualizando alturas e balanceando
        while (z != nullptr) {
            updateHeight(z);
            int balance = getBF(z);

            // Se o nó 'z' está desbalanceado, identifica o caso e aplica as rotações

            // Left-Heavy (BF > 1)
            if (balance > 1) {
                // Left-Right Case
                if (getBF(z->left) < 0) {
                    this->rotateLeft(z->left);
                }
                // Left-Left Case (ou após a correção do Left-Right)
                this->rotateRight(z);

                // As alturas são atualizadas após a rotação da árvore
                
                // z é o antigo nó desbalanceado, agora filho
                updateHeight(z);
                // z->parent é o novo topo da subárvore
                updateHeight(z->parent);
            }
            
            // Right-Heavy (BF < -1)
            else if (balance < -1) {
                // Right-Left Case
                if (getBF(z->right) > 0) {
                    this->rotateRight(z->right);
                    // Atualiza o nó que desceu
                    updateHeight(z->right->right);
                    // Atualiza o nó que subiu
                    updateHeight(z->right);
                }
                // Right-Right Case (ou após a correção do Right-Left)
                this->rotateLeft(z);

                // Atualiza alturas após rotações.
                updateHeight(z);
                updateHeight(z->parent);
            }

            z = z->parent;
        }
    }
}