#include <cstdio>
#include <cstring>
#include <edb/rbtree.h>
#include <edb/avltree.h>
#include <edb/btree.h>
#include <edb/utils.h>
#include <raylib.h>

template <typename TreeT> int main_loop(TreeT *tree) 
{
    InitWindow(1280, 720, "Binary Tree");

    while (!WindowShouldClose()) 
    {
        edb::utils::handle_input(tree);
        BeginDrawing();
        edb::utils::draw(tree);
        EndDrawing();
    }
    return 0;
}

void usage(char **argv)
{
    printf("Usage:\n");
    printf("%s -<arg>\n", argv[0]);
    printf("args:\n    'rb' for red black tree\n    'avl' for avl tree");
}

int main(int argc, char** argv) 
{
    if(argc < 2)
    {
        usage(argv);
        return 1;
    }
    else if(std::strcmp(argv[1], "rb") == 0)
    {
        edb::RBTree<int> *tree = new edb::RBTree<int>();
        return main_loop(tree);
    } else if (std::strcmp(argv[1], "avl") == 0) 
    {
      edb::AVLTree<int> *tree = (edb::AVLTree<int> *)new edb::AVLTree<int>();
      return main_loop(tree);
    } else 
    {
      usage(argv);
      return 1;
    }
}