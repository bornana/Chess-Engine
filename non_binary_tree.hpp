#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <memory>
#include <unordered_map>

#include "magic_numbers.hpp"
class Tree
{
public:
    using NodePtr = std::shared_ptr<Tree>;
    Tree(const GameState& x, NodePtr parent = nullptr);
    
    NodePtr AddLeaf(const GameState& leafstate);

    bool Finished_Branch();

    const GameState& get_state();

    const std::vector<NodePtr>& get_children();

    NodePtr get_parent();

    int get_visits();
    
    double get_value();
    void add_visit();

    void add_value(double val);

private:
    GameState state;
    NodePtr parent;
    int visits; 
    double value;
    std::vector<NodePtr> leafs;
};
