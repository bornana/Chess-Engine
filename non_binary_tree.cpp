#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include "magic_numbers.hpp"
#include "non_binary_tree.hpp"



    using NodePtr = std::shared_ptr<Tree>;//allow multiple pointers to reference the same object of type Tree
    GameState state;
    NodePtr parent = nullptr;
    int visits, value;
    Tree::Tree(const GameState& x, NodePtr parent)//constructor, state holds the current board state, parent the parent node, visits the amount of visits to this node ,value the sum of outcomes from every branch(1 for win, 0 for draw and -1 for loss)
    : state(state), parent(parent), visits(0), value(0){}//visits and value started at 0 because we are creating new leaf
    
    NodePtr Tree::AddLeaf(const GameState& leafstate){
        NodePtr leaf = std::make_shared<Tree>(leafstate, shared_from_this());
        leafs.push_back(leaf);
        return leaf;
        
    }

    bool Tree::Finished_Branch(int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables) {
        return leafs.size() == state.get_legal_moves(side, Rmagics, Bmagics, ratt_tables, batt_tables).size();//if the amount of leafs in the branch are equal to the amount of legal moves in the position, we have went over every move and the branch is complete
    }

    const GameState& Tree::get_state() const{
        return state;
    }

    const std::vector<NodePtr>& Tree::get_children() const {
        return leafs;
    }

    NodePtr Tree::get_parent() const {
        return parent;
    }

    int Tree::get_visits() {
        return visits;
    }
    
    double Tree::get_value() {
        return value;
    }

    void Tree::add_visit(){
        visits++;
    }

    void Tree::add_value(double val){
        value += val;
    }
    std::unordered_map<std::string, int> position_history;

