#include <iostream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include "non_binary_tree.hpp"
#include "magic_numbers.hpp"
using NodePtr = std::shared_ptr<Tree>;

enum {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1,
};

enum { white, black };

Tree::NodePtr select(Tree::NodePtr root){
    NodePtr node = root;
    while (!node->get_children().empty())
    {
        node = *std::max_element(node -> get_children().begin(), node -> get_children().end(),
        [](const NodePtr& a, const NodePtr& b){
            double ucb1_a = a -> get_value() / a -> get_visits() + std::sqrt(2*std::log(a -> get_parent() -> get_visits() / a -> get_visits()));
            double ucb1_b = b -> get_value() / b -> get_visits() + std::sqrt(2*std::log(b -> get_parent() -> get_visits() / b -> get_visits()));
            return ucb1_a < ucb1_b;//ucb1 equation, look in project book for explanation on the equation
        });

    }
    return node;
}

Tree::NodePtr expand(Tree::NodePtr node, int side){
    std::vector<GameState> legal_moves = node -> get_state().get_legal_moves(side);
    for (const GameState& move : legal_moves)
    {
        bool move_exists = false;
        for (const auto& child : node -> get_children())
        {
            if(child -> get_state() == move){
                move_exists = true;
                break;
            }
        }
        if(!move_exists){
            return node -> AddLeaf(move);
        }
    }
    return node;
}

double simulate(Tree::NodePtr node, int side){
    GameState state = node->get_state();
    std::unordered_map<std::string, int> local_position_history;
    std::string initial_hash = state.hash_position();
    local_position_history[initial_hash] = 1;

    int turn = side;//even number means white turn and odd black turn
    while(!state.is_terminal()){
        std::vector<GameState> legal_moves = state.get_legal_moves(turn % 2);
        state = legal_moves[std::rand() % legal_moves.size()];//choose random move to play
        std::string position_hash = state.hash_position();
        local_position_history[position_hash]++;

        if(local_position_history[position_hash] >= 3) return 0;//threefold repetition is a draw
        turn++;
    }
    return state.get_result(side);
}

void backpropogate(Tree::NodePtr node, double result){
    while(node != nullptr){
        node -> add_visit();
        node -> add_value(result);
        node = node -> get_parent();
    }
}

void mcts(Tree::NodePtr root, int num_simulations, int side){
    for (int i = 0; i < num_simulations; i++)
    {
        Tree::NodePtr node = select(root);
        if(node -> Finished_Branch()){
            node = expand(node, side);
        }
        double result = simulate(node, side);
        backpropogate(node, result);
    }
    
}
//tasks:
//castling?
//find NN
//connect with main project
//add firebase
//learn for math bagrut?
int main() {
    std::srand(std::time(0));

    int computer_player = white;
    GameState initial_state;

    Tree::NodePtr root = std::make_shared<Tree>(initial_state);

    int num_simulations = 1000;
    mcts(root, num_simulations, computer_player);

    Tree::NodePtr best_move = *std::max_element(root -> get_children().begin(), root -> get_children().end(),
    [](const Tree::NodePtr& a, const Tree::NodePtr& b){
        return a -> get_visits() < b -> get_visits();
    });


    
    std::cout << "Best move found after" << num_simulations << "simulations" << std::endl;
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