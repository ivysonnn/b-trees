#pragma once

#include <random>
#include <raylib.h>
#include <edb/rbtree.h>

namespace edb
{
    namespace utils
    {
        template <typename NodeT> ::Color getNodeColor(NodeT* node) {return BLACK;}

        template<typename T> ::Color getNodeColor(RBNode<T>* node) {return (node->color == R) ? RED : BLACK;}

        template <typename NodeT> void drawNode(NodeT* node, int x, int y, int offsetX);

        template <typename TreeT> void draw(TreeT *tree);

        template <typename TreeT> void handle_input(TreeT *tree);
    }
}

template <typename NodeT> void edb::utils::drawNode(NodeT *node, int x, int y, int offsetX) 
{
    if (!node)
        return;

    if (node->left)
        DrawLine(x, y, x - offsetX, y + 80, BLACK);
    if (node->right)
        DrawLine(x, y, x + offsetX, y + 80, BLACK);

    Color c = getNodeColor(node);

    DrawCircle(x, y, 20, c);
    DrawCircleLines(x, y, 20, WHITE);

    // Draw value
    DrawText(TextFormat("%d", node->data), x - 10, y - 8, 18, WHITE);

    int nextOffset = offsetX > 50 ? offsetX / 2 : 50; // 50px minimum
    if (node->left)
        drawNode(node->left, x - offsetX, y + 80, offsetX / 2);
    if (node->right)
        drawNode(node->right, x + offsetX, y + 80, offsetX / 2);
}

template <typename TreeT> void edb::utils::draw(TreeT *tree) 
{
    ClearBackground(RAYWHITE);

    if (tree->root)
        drawNode(tree->root, GetScreenWidth() / 2, 60, 300);
}

template <typename TreeT> void edb::utils::handle_input(TreeT* tree) 
{
    static bool removing = false;
    static bool inserting = false;
    static std::string input = "";

    if (IsKeyPressed(KEY_SPACE)) 
    {
        int n = 1000;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(1, n - 1);

        int e = distrib(gen);

        tree->insertAndFix(e);
    }

    if (IsKeyPressed(KEY_R) && !removing) 
    { // clear the input to begin removing mode
      removing = true;
      input.clear();
    }

    if (IsKeyPressed(KEY_A) && !inserting) 
    { // clear the input to begin insert mode
      inserting = true;
      input.clear();
    }

    int key = GetCharPressed();

    if (removing) 
    {
        if(key > 0)
        {
            if (key >= 48 && key <= 57)
                input.push_back((char)key);
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !input.empty()) 
        {
            input.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER)) 
        {
        if (!input.empty()) {
            int value = std::stoi(input);
            tree->removeAndFix(value);
        }
        removing = false;
        }

        if (IsKeyPressed(KEY_ESCAPE)) 
        {
            removing = false;
        }

        DrawText(("Remove: " + input).c_str(), 50, 50, 20, RED);
    }

    if (inserting) 
    {
        if (key > 0) 
        {
            if (key >= 48 && key <= 57)
            input.push_back((char)key);
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !input.empty()) 
        {
            input.pop_back();
        }

        if (IsKeyPressed(KEY_ENTER)) 
        {
            if (!input.empty()) 
            {
                int value = std::stoi(input);
                tree->insertAndFix(value);
            }
            inserting = false;
        }

        if (IsKeyPressed(KEY_ESCAPE)) 
        {
            inserting = false;
        }

        DrawText(("Insert: " + input).c_str(), 50, 50, 20, RED);
    }
}