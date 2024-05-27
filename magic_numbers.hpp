#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
typedef unsigned long long uint64;
//
//
//
//SLIDING PIECES(ROOK, BISHOP, QUEEN)
//
//
//
uint64 random_bitboard();

uint64 random_fewbits_bitboard();

int count_1s(uint64 b);

int pop_1st_bit(uint64* bb);

uint64 index_to_uint64(int index, int bits, uint64 m);

uint64 rmask(int sq);

uint64 bmask(int sq);

uint64 ratt(int sq, uint64 block);

uint64 batt(int sq, uint64 block);


int transform(uint64 b, uint64 magic, int bits);

uint64 find_magic(int sq, int m, int bishop);


// print bitboard
void print_bitboard(uint64 bitboard);
//
//
//
//PAWN LEGAL MOVES
//
//
//
uint64 pawn_move_mask(int square, int side, uint64 blockers);

uint64 pawn_capture_mask(int square, int side, uint64 enemies);

uint64 all_pawn_moves_mask(int square, int side, uint64 enemies, uint64 blockers);
//
//
//
//KNIGHT LEGAL MOVES
//
//
//
uint64 all_knight_moves_mask(int square, uint64 blockers);
//
//
//
//KING LEGAL MOVES
//
//
//
uint64 all_king_moves_mask(int square, uint64 blockers);



class GameState{
private:
    std::vector<uint64> board;
    int whiteP, whiteN, whiteB, whiteR, whiteQ, whiteK, blackP, blackN, blackB, blackR, blackQ, blackK, fifty_move_counter;
public:
    GameState();
    uint64 all_white_pieces();
    uint64 all_black_pieces();
    bool is_checked(int side, GameState move);
    std::vector<uint64> get_board();
    int num_of_pieces(int side);
    void update_board(uint64 b, int index);
    void move_piece(int start, int destination, GameState state, int index, int side);
    std::vector<GameState> get_pseudo_legal_moves(int side);
    std::vector<GameState> get_legal_moves(int side) const;
    bool is_terminal() const;
    double get_result(int side) const;
    std::string hash_position() const;
    bool operator ==(const GameState& other)const;
};