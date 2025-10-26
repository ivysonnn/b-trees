#include <cstdio>
#include <edb/rbtree.h>
#include <edb/utils.h>
#include <raylib.h>

int main(int argc, char** argv) 
{
    if(argc < 2)
    {
        printf("Usage:\n");
        printf("%s -<arg>\n", argv[0]);
        printf("args:\n    'rb' for red black tree\n    'avl' for avl tree");
        return 1;
    }
    if(argv[1] == "-rb")
        edb::RBTree<int> *tree = new edb::RBTree<int>();
    else if(argv[1] == "avl")
        edb::AVLTree<int> *tree = new edb::AVLTree<int>();
    
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