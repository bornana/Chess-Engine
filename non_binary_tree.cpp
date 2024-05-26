#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <memory>
#include "magic_numbers.hpp"
#include "non_binary_tree.hpp"


class Tree : public std::enable_shared_from_this<Tree> {
public:
    using NodePtr = std::shared_ptr<Tree>;//allow multiple pointers to reference the same object of type Tree
    GameState state;
    NodePtr parent = nullptr;
    int visits, value;
    Tree(const GameState& x, NodePtr parent = nullptr)//constructor, state holds the current board state, parent the parent node, visits the amount of visits to this node ,value the sum of outcomes from every branch(1 for win, 0 for draw and -1 for loss)
    : state(state), parent(parent), visits(0), value(0){}//visits and value started at 0 because we are creating new leaf
    
    NodePtr AddLeaf(const GameState& leafstate){
        NodePtr leaf = std::make_shared(leafstate, shared_from_this());
        leafs.push_back(leaf);
        return leaf;
        
    }

    bool Finished_Branch() const {
        return leafs.size() == state.get_legal_moves().size();//if the amount of leafs in the branch are equal to the amount of legal moves in the position, we have went over every move and the branch is complete
    }

    const GameState& get_state() const{
        return state;
    }

    const std::vector<NodePtr>& get_children() const {
        return children;
    }

    NodePtr get_parent() const {
        return parent;
    }

    int get_visits() {
        return visits;
    }
    
    double get_value() {
        return value;
    }

    void add_visit(){
        visits++;
    }

    void add_value(double val){
        value += val;
    }
    

private:
    GameState state;
    NodePtr parent;
    int visits; 
    double value;
    std::vector<NodePtr> leafs;
};

