#include <iostream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "non_binary_tree.hpp"
#include "magic_numbers.hpp"
using NodePtr = std::shared_ptr<Tree>;

Tree::NodePtr select(Tree::NodePtr root){
    Tree::NodePtr node = root;
    while (!node->get_children().empty())
    {
        node = *std::max_element(node -> get_children().begin(), node -> get_children().end(),
        [](const Tree::NodePtr& a, const Tree::NodePtr& b){
            double ucb1_a = a -> get_value() / a -> get_visits() + std::sqrt(2*std::log(a -> get_pa))
        })
    }
    
}

int main() {
    int square;
    GameState state = GameState();
    uint64 x = 5ULL;
    uint64 y = 4ULL;

    for (int i = 0; i < Rmagics.size(); i++)
    {
        std::cout << Rmagics[i] << std::endl;
    }
    
    std::cout << "1" << std::endl;
    /*printf("const uint64 RMagic[64] = {\n");
    for (square = 0; square < 64; square++)
        printf("  0x%llxULL,\n", find_magic(square, RBits[square], 0));
    printf("};\n\n");

    printf("const uint64 BMagic[64] = {\n");
    for (square = 0; square < 64; square++)
        printf("  0x%llxULL,\n", find_magic(square, BBits[square], 1));
    printf("};\n\n");*/

    return 0;
}