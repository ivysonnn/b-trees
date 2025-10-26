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

        template <typename T> void handle_input(edb::RBTree<T> *tree);
    }
}

template <typename NodeT> void edb::utils::drawNode(NodeT *node, int x, int y, int offsetX) {
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

    int nextOffset = offsetX > 40 ? offsetX / 2 : 40; // 40px minimum
    if (node->left)
        drawNode(node->left, x - offsetX, y + 80, offsetX / 2);
    if (node->right)
        drawNode(node->right, x + offsetX, y + 80, offsetX / 2);
}

template <typename TreeT> void edb::utils::draw(TreeT *tree) {
    ClearBackground(RAYWHITE);

    if (tree->root)
        drawNode(tree->root, GetScreenWidth() / 2, 60, 200);
}

template <typename T> void edb::utils::handle_input(edb::RBTree<T> *tree) {
  static bool removing = false;
  static std::string input = "";

  if (IsKeyPressed(KEY_SPACE)) {
    int n = 1000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, n - 1);

    int e = distrib(gen);

    tree->insertAndFix(e);
  }

  if (IsKeyPressed(KEY_R) && !removing) { // input for the remove value
    removing = true;
    input.clear();
  }

  if (removing) {
    int key = GetCharPressed();

    while (key > 0) {
      if (key >= 48 && key <= 57)
        input.push_back((char)key);

      key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE) && !input.empty()) {
      input.pop_back();
    }

    if (IsKeyPressed(KEY_ENTER)) {
      if (!input.empty()) {
        int value = std::stoi(input);
        tree->removeAndFix(value);
      }
      removing = false;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
      removing = false;
    }

    DrawText(("Remove: " + input).c_str(), 50, 50, 20, RED);
  }
}