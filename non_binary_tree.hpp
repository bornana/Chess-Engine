#pragma once
#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <memory>
#include <unordered_map>

#include "magic_numbers.hpp"
class Tree : public std::enable_shared_from_this<Tree>
{
public:
    using NodePtr = std::shared_ptr<Tree>;
    Tree(const GameState& x, NodePtr parent = nullptr);
    
    NodePtr AddLeaf(const GameState& leafstate);

    bool Finished_Branch(int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables);

    const GameState& get_state() const;

    const std::vector<NodePtr>& get_children() const;

    NodePtr get_parent() const;

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
