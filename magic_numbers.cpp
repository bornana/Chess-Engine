#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "magic_numbers.hpp"
#define USE_32_BIT_MULTIPLICATIONS

typedef unsigned long long uint64;
const uint64 starting_position = 18446462598732906495ULL;//unsigned long long
const uint64 not_a_file = 18374403900871474942ULL;
const uint64 not_ab_files = 18229723555195321596ULL;
const uint64 not_h_file = 9187201950435737471ULL;
const uint64 not_gh_files = 4557430888798830399ULL;




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

enum { rook, bishop };

// get/set/pop macros
#define set_bit(bitboard, square) (bitboard |= (1ULL << square))
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define pop_bit(bitboard, square) (get_bit(bitboard, square) ? (bitboard ^= (1ULL << square)) : 0)
//
//
//
//SLIDING PIECES(ROOK, BISHOP, QUEEN)
//
//
//
uint64 random_bitboard() {
    uint64 u1, u2, u3, u4;
    u1 = (uint64)(rand()) & 0xFFFF; u2 = (uint64)(rand()) & 0xFFFF;
    u3 = (uint64)(rand()) & 0xFFFF; u4 = (uint64)(rand()) & 0xFFFF;
    return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
}

uint64 random_fewbits_bitboard() {
    return random_bitboard() & random_bitboard() & random_bitboard();
}

int count_1s(uint64 b) {
    int r;
    for (r = 0; b; r++, b &= b - 1);
    return r;
}

const int BitTable[64] = {
  63, 30, 3, 32, 25, 41, 22, 33,
  15, 50, 42, 13, 11, 53, 19, 34,
  61, 29, 2, 51, 21, 43, 45, 10,
  18, 47, 1, 54, 9, 57, 0, 35,
  62, 31, 40, 4, 49, 5, 52, 26,
  60, 6, 23, 44, 46, 27, 56, 16,
  7, 39, 48, 24, 59, 14, 12, 55,
  38, 28,58, 20, 37, 17, 36, 8
};

int pop_1st_bit(uint64* bb) {
    uint64 b = *bb ^ (*bb - 1);
    unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32));
    *bb &= (*bb - 1);
    return BitTable[(fold * 0x783a9b23) >> 26];
}

uint64 index_to_uint64(int index, int bits, uint64 num) {
    int i, j;
    uint64 result = 0ULL;
    for (i = 0; i < bits; i++) {
        j = pop_1st_bit(&num);
        if (index & (1 << i)) result |= (1ULL << j);
    }
    return result;
}

uint64 rmask(int sq) {
    uint64 result = 0ULL;
    int rk = sq / 8, fl = sq % 8, r, f;
    for (r = rk + 1; r <= 6; r++) result |= (1ULL << (fl + r * 8));
    for (r = rk - 1; r >= 1; r--) result |= (1ULL << (fl + r * 8));
    for (f = fl + 1; f <= 6; f++) result |= (1ULL << (f + rk * 8));
    for (f = fl - 1; f >= 1; f--) result |= (1ULL << (f + rk * 8));
    return result;
}

uint64 bmask(int sq) {
    uint64 result = 0ULL;
    int rk = sq / 8, fl = sq % 8, r, f;
    for (r = rk + 1, f = fl + 1; r <= 6 && f <= 6; r++, f++) result |= (1ULL << (f + r * 8));
    for (r = rk + 1, f = fl - 1; r <= 6 && f >= 1; r++, f--) result |= (1ULL << (f + r * 8));
    for (r = rk - 1, f = fl + 1; r >= 1 && f <= 6; r--, f++) result |= (1ULL << (f + r * 8));
    for (r = rk - 1, f = fl - 1; r >= 1 && f >= 1; r--, f--) result |= (1ULL << (f + r * 8));
    return result;
}

uint64 ratt(int sq, uint64 block) {
    uint64 result = 0ULL;
    int rk = sq / 8, fl = sq % 8, r, f;
    for (r = rk + 1; r <= 7; r++) {
        result |= (1ULL << (fl + r * 8));
        if (block & (1ULL << (fl + r * 8))) break;
    }
    for (r = rk - 1; r >= 0; r--) {
        result |= (1ULL << (fl + r * 8));
        if (block & (1ULL << (fl + r * 8))) break;
    }
    for (f = fl + 1; f <= 7; f++) {
        result |= (1ULL << (f + rk * 8));
        if (block & (1ULL << (f + rk * 8))) break;
    }
    for (f = fl - 1; f >= 0; f--) {
        result |= (1ULL << (f + rk * 8));
        if (block & (1ULL << (f + rk * 8))) break;
    }
    return result;
}

uint64 batt(int sq, uint64 block) {
    uint64 result = 0ULL;
    int rk = sq / 8, fl = sq % 8, r, f;
    for (r = rk + 1, f = fl + 1; r <= 7 && f <= 7; r++, f++) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    for (r = rk + 1, f = fl - 1; r <= 7 && f >= 0; r++, f--) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    for (r = rk - 1, f = fl + 1; r >= 0 && f <= 7; r--, f++) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    for (r = rk - 1, f = fl - 1; r >= 0 && f >= 0; r--, f--) {
        result |= (1ULL << (f + r * 8));
        if (block & (1ULL << (f + r * 8))) break;
    }
    return result;
}


int transform(uint64 b, uint64 magic, int bits) {
#if defined(USE_32_BIT_MULTIPLICATIONS)
    return
        (unsigned)((int)b * (int)magic ^ (int)(b >> 32) * (int)(magic >> 32)) >> (32 - bits);
#else
    return (int)((b * magic) >> (64 - bits));
#endif
}

uint64 find_magic(int sq, int num_of_moves, int bishop) {
    uint64 mask, b[4096], a[4096], used[4096], magic;
    int i, j, k, n, fail;

    mask = bishop ? bmask(sq) : rmask(sq);
    n = count_1s(mask);

    for (i = 0; i < (1 << n); i++) {
        b[i] = index_to_uint64(i, n, mask);//b holds all possible formations of blocking pieces
        a[i] = bishop ? batt(sq, b[i]) : ratt(sq, b[i]);//a holds all possible blocker patterns with the available moves accordingly
    }
    for (k = 0; k < 100000000; k++) {
        magic = random_fewbits_bitboard();//randomly make bitboards 100 millions times(number doesnt matter as long as it is big)
        if (count_1s((mask * magic) & 0xFF00000000000000ULL) < 6) continue;
        for (i = 0; i < 4096; i++) used[i] = 0ULL;
        for (i = 0, fail = 0; !fail && i < (1 << n); i++) {
            j = transform(b[i], magic, num_of_moves);//j is the index for the possible moves with the given blockers that should be given if the magic number is correct
            if (used[j] == 0ULL) used[j] = a[i];//fill used with a
            else if (used[j] != a[i]) fail = 1;//used[j] should represent a[i], if it doesnt then the index is wrong and so is the magic number. in that case stop the loop and find new magic
        }
        if (!fail) return magic;//if the loop wasnt forcefully stopped then the magic is good and return it
    }
    printf("**Failed**\n");//if no magic number was found print fail and return empty bitboard;
    return 0ULL;
}

int RBits[64] = {
  12, 11, 11, 11, 11, 11, 11, 12,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  11, 10, 10, 10, 10, 10, 10, 11,
  12, 11, 11, 11, 11, 11, 11, 12
};

int BBits[64] = {
  6, 5, 5, 5, 5, 5, 5, 6,
  5, 5, 5, 5, 5, 5, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 9, 9, 7, 5, 5,
  5, 5, 7, 7, 7, 7, 5, 5,
  5, 5, 5, 5, 5, 5, 5, 5,
  6, 5, 5, 5, 5, 5, 5, 6
};

// print bitboard
void print_bitboard(uint64 bitboard)
{
	printf("\n");
	for (int rank = 0; rank < 8; ++rank)
	{
		// print row
		printf("  %d ", 8 - rank);
		for (int file = 0; file < 8; ++file)
		{
			int square_index = rank * 8 + file;

			// print board state (1- occupied, 0- free)
			printf(" %d", get_bit(bitboard, square_index) ? 1 : 0);
		}
		printf("\n");
	}
	printf("\n     a b c d e f g h\n\n");
	printf("     Bitboard value: %llu\n\n", bitboard);
}
//
//
//
//PAWN LEGAL MOVES
//
//
//
uint64 pawn_move_mask(int square, int side, uint64 blockers){
    uint64 bitboard = 0ULL;
    set_bit(bitboard, square);
    bool doublemove = false;
    uint64 moves = 0ULL;
    int temp;

    // white pawns
    if(!side)
    {
        moves |= bitboard >> 8;
        if (square > 47) {
            moves |= (bitboard >> 16);//if pawn didnt move he can also choose to move two spaces
            doublemove = true;
        }
    }
	// black pawns
    else
    {
        moves |= bitboard << 8;
        if(square < 16) {
            moves |= (bitboard << 16);
            doublemove = true;
        }
    }

    blockers = blockers & moves;//only keep relevant blockers
    if(doublemove && count_1s(blockers) == 1){
        if(blockers == (bitboard >> 8))  moves ^= (bitboard >> 16);
        else if (blockers == (bitboard << 8)) moves ^= (bitboard << 16);
    }
    moves = moves ^ blockers;
    /*
    if(doublemove && count_1s(blockers) ){
        if(!side){
            moves ^= (bitboard >> 16);
        } 
        else {
            moves ^= (bitboard << 16);
        }   
    }*/

    return moves;//return move mask
}
uint64 pawn_capture_mask(int square, int side, uint64 enemies){
	// set piece bitboard
	uint64 bitboard = 0ULL;
	set_bit(bitboard, square);

	// set attack bitboard
	uint64 attacks = 0ULL;

	if (!side)
	{
		if ((bitboard >> 7) & not_a_file) attacks |= (bitboard >> 7);
		if ((bitboard >> 9) & not_h_file) attacks |= (bitboard >> 9);
	}

	else
	{
		if ((bitboard << 7) & not_h_file) attacks |= (bitboard << 7);
		if ((bitboard << 9) & not_a_file) attacks |= (bitboard << 9);
	}
    attacks &= enemies;
	return attacks;// return attack mask
}
uint64 all_pawn_moves_mask(int square, int side, uint64 enemies, uint64 blockers){
    uint64 allmoves = 0ULL;
    allmoves |= pawn_capture_mask(square, side, enemies);
    allmoves |= pawn_move_mask(square, side, blockers);
    return allmoves;
}
//
//
//
//KNIGHT LEGAL MOVES
//
//
//
uint64 all_knight_moves_mask(int square, uint64 blockers){
uint64 attacks, knights = 0ULL;

// place knight on board
(knights) |= (1ULL << square);
// add attacks
attacks = (((knights >> 6)  | (knights << 10)) & not_ab_files) |
          (((knights >> 10) | (knights << 6))  & not_gh_files) |
          (((knights >> 15) | (knights << 17)) & not_a_file)  |
          (((knights >> 17) | (knights << 15)) & not_h_file);
attacks ^= blockers;
return attacks;
}
//
//
//
//KING LEGAL MOVES
//
//
//
uint64 all_king_moves_mask(int square, uint64 blockers){
    uint64 bitboard = 0ULL;
    set_bit(bitboard, square);

    uint64 moves = 0ULL;
    moves = (((bitboard >> 7) | (bitboard << 1) | (bitboard << 9)) & not_a_file) |
            (((bitboard >> 9) | (bitboard >> 1) | (bitboard << 7)) & not_h_file) |
            (bitboard << 8) | (bitboard >> 8);
    moves ^= blockers;
    return moves;
}



    GameState::GameState(){
        fifty_move_counter = 0;
        whiteP = 0;
        whiteN = 1;
        whiteB = 2;
        whiteR = 3;
        whiteQ = 4;
        whiteK = 5;
        blackP = 6;
        blackN = 7;
        blackB = 8;
        blackR = 9;
        blackQ = 10;
        blackK = 11;
        board = std::vector<uint64>(12);
        board[whiteP] = 71776119061217280ULL;
        board[whiteN] = 4755801206503243776ULL;
        board[whiteB] = 2594073385365405696ULL;
        board[whiteR] = 9295429630892703744ULL;
        board[whiteQ] = 1152921504606846976ULL;
        board[whiteK] = 576460752303423488;
        board[blackP] = 65280ULL;
        board[blackN] = 66ULL;
        board[blackB] = 36ULL;
        board[blackR] = 129ULL;
        board[blackQ] = 8ULL;
        board[blackK] = 16ULL;
    };
    uint64 GameState::all_white_pieces(){
        uint64 bitboard = 0ULL;
        return (bitboard | board[whiteP] | board[whiteN] | board[whiteB] | board[whiteR] | board[whiteQ] | board[whiteK]);
    };
    uint64 GameState::all_black_pieces(){
        uint64 bitboard = 0ULL;
        return bitboard | board[blackP] | board[blackN] | board[blackB] | board[blackR] | board[blackQ] | board[blackK];
    };
    bool GameState::is_checked(int side, GameState move){
        uint64 king, state;
        std::vector<GameState> moves;
        if(!side){
            king = board[whiteK];
            if(king & (move.all_black_pieces())) return true;
        }
        else{
            king = board[blackK];
            if(king & (move.all_white_pieces())) return true;
        }
        return false;
    };
    std::vector<uint64> GameState::get_board(){
        return board;
    };
    int GameState::num_of_pieces(int side){
        int sum_pieces = 0;
        for(int i = 5 * side; i < (5*side) + 5; i++){
            sum_pieces += count_1s(board[i]);
        }
        return sum_pieces;
    };
    void GameState::update_board(uint64 b, int index){
        board[index] = b;
    };
    void GameState::move_piece(int start, int destination, GameState state, int index, int side){
        uint64 piece = 1ULL << start;
        uint64 piece_dest = 1ULL << destination;
        int prev_num_of_enemies;
        if(!side) prev_num_of_enemies = count_1s(all_black_pieces);
        else prev_num_of_enemies = count_1s(all_white_pieces);
        uint64 moved = state.get_board()[index] ^ (piece_dest);// moved now has all the pieces of the type being moved with an extra bit which is the destination of one of the pieces
        state.update_board((moved ^ piece), index);//remove the piece's previous spot and update the board with the piece moved
        if(!side){
            if(index != whiteP && count_1s(all_black_pieces()) != prev_num_of_enemies) fifty_move_counter++;
            else if (index != blackP && count_1s(all_white_pieces()) != prev_num_of_enemies) fifty_move_counter++;//if there wan no pawn move or take, start counting for fifty move rule
            else fifty_move_counter = 0;
        }
    };
    std::vector<GameState> GameState::get_pseudo_legal_moves(int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables){
        std::vector<GameState> moves;
        GameState help = GameState();
        uint64 piecemoves, B_attacks, R_attacks;
        int index = 0, individual_move;
        int piecesquare;
        std::vector<uint64> temp = board;
        if(!side){//for white pieces we will push each move from each piece into the moves vector
            for(int i = 0; i < count_1s(board[whiteP]); i++){//for each pawn
                piecesquare = pop_1st_bit(&temp[whiteP]);//piecesquare is current pawn's location
                piecemoves = all_pawn_moves_mask(piecesquare, side, all_black_pieces(), all_white_pieces());//piecemoves is bitboard with all pawn moves
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(piecemoves); j++){
                    individual_move = pop_1st_bit(&piecemoves);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, whiteP, side);
                    moves.push_back(help);
                    help.update_board(board[whiteP], whiteP);//reset help to original state, because for each gamestate in moves we want an individual move
                }
            }
            for(int i = 0; i < count_1s(board[whiteN]); i++){//for each knight
                piecesquare = pop_1st_bit(&temp[whiteN]);//piecesquare is current knight's location
                piecemoves = all_knight_moves_mask(piecesquare, all_white_pieces());//piecemoves is bitboard with all knight moves
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(piecemoves); j++){
                    individual_move = pop_1st_bit(&piecemoves);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, whiteN, side);
                    moves.push_back(help);
                    help.update_board(board[whiteN], whiteN);//reset help to original state, because for each gamestate in moves we want an individual move
                }
            }
            for(int i = 0; i < count_1s(board[whiteB]); i++){//for each bishop
                piecesquare = pop_1st_bit(&temp[whiteB]);//piecesquare is current bishop's location
                B_attacks = batt_tables[piecesquare][transform(all_white_pieces(), Bmagics[piecesquare], BBits[piecesquare])]; 
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(B_attacks); j++){
                    individual_move = pop_1st_bit(&B_attacks);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, whiteB, side);
                    moves.push_back(help);
                    help.update_board(board[whiteB], whiteB);//reset help to original state, because for each gamestate in moves we want an individual move
                }
            }
            for(int i = 0; i < count_1s(board[whiteR]); i++){//for each bishop
                piecesquare = pop_1st_bit(&temp[whiteR]);//piecesquare is current bishop's location
                R_attacks = ratt_tables[piecesquare][transform(all_white_pieces(),Rmagics[piecesquare], RBits[piecesquare])]; 
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(R_attacks); j++){
                    individual_move = pop_1st_bit(&R_attacks);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, whiteR, side);
                    moves.push_back(help);
                    help.update_board(board[whiteR], whiteR);//reset help to original state, because for each gamestate in moves we want an individual move
                }
            }
            for(int i = 0; i < count_1s(board[whiteQ]); i++){//for each bishop
                piecesquare = pop_1st_bit(&temp[whiteQ]);//piecesquare is current bishop's location
                B_attacks = batt_tables[piecesquare][transform(all_white_pieces(), Bmagics[piecesquare], BBits[piecesquare])]; 
                R_attacks = ratt_tables[piecesquare][transform(all_white_pieces(),Rmagics[piecesquare], RBits[piecesquare])]; 
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(B_attacks); j++){
                    individual_move = pop_1st_bit(&B_attacks);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, whiteQ, side);
                    moves.push_back(help);
                    help.update_board(board[whiteQ], whiteQ);//reset help to original state, because for each gamestate in moves we want an individual move
                }
                for(int j = 0; j < count_1s(R_attacks); j++){
                    individual_move = pop_1st_bit(&R_attacks);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, whiteQ, side);
                    moves.push_back(help);
                    help.update_board(board[whiteQ], whiteQ);//reset help to original state, because for each gamestate in moves we want an individual move
                }//Queen moves like bishop and rook, for convenience of not making queen magic numbers use rook and bishop separately for same queen square
            }
            for(int i = 0; i < count_1s(board[whiteK]); i++){//for each knight
                piecesquare = pop_1st_bit(&temp[whiteK]);//piecesquare is current knight's location
                piecemoves = all_king_moves_mask(piecesquare, all_white_pieces());//piecemoves is bitboard with all knight moves
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(piecemoves); j++){
                    individual_move = pop_1st_bit(&piecemoves);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, whiteN, side);
                    moves.push_back(help);
                    help.update_board(board[whiteK], whiteK);//reset help to original state, because for each gamestate in moves we want an individual move
                }
            }
        }
        else{//for black pieces do the same
            for(int i = 0; i < count_1s(board[blackP]); i++){//for each pawn
                piecesquare = pop_1st_bit(&temp[blackP]);//piecesquare is current pawn's location
                piecemoves = all_pawn_moves_mask(piecesquare, side, all_white_pieces(), all_black_pieces());//piecemoves is bitboard with all pawn moves
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(piecemoves); j++){
                    individual_move = pop_1st_bit(&piecemoves);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, blackP, side);
                    moves.push_back(help);
                    help.update_board(board[blackP], blackP);//reset help to original state, because for each gamestate in moves we want an individual move
                }
            }
            for(int i = 0; i < count_1s(board[blackN]); i++){//for each knight
                piecesquare = pop_1st_bit(&temp[blackN]);//piecesquare is current knight's location
                piecemoves = all_knight_moves_mask(piecesquare, all_black_pieces());//piecemoves is bitboard with all knight moves
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(piecemoves); j++){
                    individual_move = pop_1st_bit(&piecemoves);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, blackN, side);
                    moves.push_back(help);
                    help.update_board(board[blackN], blackN);//reset help to original state, because for each gamestate in moves we want an individual move
                }
            }
            for(int i = 0; i < count_1s(board[blackB]); i++){//for each bishop
                piecesquare = pop_1st_bit(&temp[blackB]);//piecesquare is current bishop's location
                B_attacks = batt_tables[piecesquare][transform(all_black_pieces(), Bmagics[piecesquare], BBits[piecesquare])]; 
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(B_attacks); j++){
                    individual_move = pop_1st_bit(&B_attacks);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, blackB, side);
                    moves.push_back(help);
                    help.update_board(board[blackB], blackB);//reset help to original state, because for each gamestate in moves we want an individual move
                }
            }
            for(int i = 0; i < count_1s(board[blackR]); i++){//for each bishop
                piecesquare = pop_1st_bit(&temp[blackR]);//piecesquare is current bishop's location
                R_attacks = ratt_tables[piecesquare][transform(all_black_pieces(),Rmagics[piecesquare], RBits[piecesquare])]; 
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(R_attacks); j++){
                    individual_move = pop_1st_bit(&R_attacks);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, blackR, side);
                    moves.push_back(help);
                    help.update_board(board[blackR], blackR);//reset help to original state, because for each gamestate in moves we want an individual move
                }
            }
            for(int i = 0; i < count_1s(board[blackQ]); i++){//for each bishop
                piecesquare = pop_1st_bit(&temp[blackQ]);//piecesquare is current bishop's location
                B_attacks = batt_tables[piecesquare][transform(all_black_pieces(), Bmagics[piecesquare], BBits[piecesquare])]; 
                R_attacks = ratt_tables[piecesquare][transform(all_black_pieces(),Rmagics[piecesquare], RBits[piecesquare])]; 
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(B_attacks); j++){
                    individual_move = pop_1st_bit(&B_attacks);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, blackQ, side);
                    moves.push_back(help);
                    help.update_board(board[blackQ], blackQ);//reset help to original state, because for each gamestate in moves we want an individual move
                }
                for(int j = 0; j < count_1s(R_attacks); j++){
                    individual_move = pop_1st_bit(&R_attacks);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, blackQ, side);
                    moves.push_back(help);
                    help.update_board(board[blackQ], blackQ);//reset help to original state, because for each gamestate in moves we want an individual move
                }//Queen moves like bishop and rook, for convenience of not making queen magic numbers use rook and bishop separately for same queen square
            }
            for(int i = 0; i < count_1s(board[blackK]); i++){//for each knight
                piecesquare = pop_1st_bit(&temp[blackK]);//piecesquare is current knight's location
                piecemoves = all_king_moves_mask(piecesquare, all_black_pieces());//piecemoves is bitboard with all knight moves
                //help.update_board(piecemoves, whiteP);
                for(int j = 0; j < count_1s(piecemoves); j++){
                    individual_move = pop_1st_bit(&piecemoves);//for each move in piecemoves
                    move_piece(piecesquare, individual_move, help, blackK, side);
                    moves.push_back(help);
                    help.update_board(board[blackK], blackK);//reset help to original state, because for each gamestate in moves we want an individual move
                }
            }
        }
        return moves;
    };
    std::vector<GameState> GameState::get_legal_moves(int side,int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables) const{
        
        std::vector<GameState> legal_moves, pseudo_legal = get_pseudo_legal_moves(side,int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tablesint side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables);
        for (int i = 0; i < pseudo_legal.size(); i++)
        {
            if(!is_checked(side, pseudo_legal[i])) legal_moves.push_back(pseudo_legal[i]); 
        }
        return legal_moves;
        
    };
    bool GameState::is_terminal(int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables) const{
        std::vector<GameState> white_moves, black_moves;
        white_moves = get_legal_moves(white,int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables);
        black_moves = get_legal_moves(black,int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables);
        if(fifty_move_counter >= 50) return true;
        if(!side){
            if(white_moves.size() == 0) return true;
            if(board[whiteP] == 0 && board[whiteR] == 0 && board[whiteQ] == 0 && (board[whiteB] == 0 || board[whiteN] == 0)) return true;//if no sufficient pieces to checkmate then draw
        }
        else{
            if(black_moves.size() == 0) return true;//if player doesnt have any legal moves he is in draw or mate
            if(board[blackP] == 0 && board[blackR] == 0 && board[blackQ] == 0 && (board[blackB] == 0 || board[blackN] == 0)) return true;
        }
        
        return false;
    };
    double GameState::get_result(int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables) const{
        std::vector<GameState> player_moves = get_legal_moves(side,int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables), enemy_moves = get_legal_moves(!side,int side,std::vector<uint64> Rmagics,std::vector<uint64> Bmagics, std::vector<std::vector<uint64>> ratt_tables, std::vector<std::vector<uint64>> batt_tables);
        if(fifty_move_counter >= 50) return 0;//according to the rule if no pawn moves or takes for 50 turns then its a draw
        if(player_moves == 0){
            if(is_checked(side, board)) return -1;//if the player is checked and cant move he lost
            return 0;//if he cant move but no check then its a draw(we check for repetition in mcts code)
        }
        else if(enemy_moves == 0) return 1;
        return 0;//because we only call code if game ended then we can assume that player drew
    };
    std::string GameState::hash_position() const {
        std::string hash;
        for (const auto& row : board) {
            for (const auto& piece : row) {
                hash += piece.to_string();
            }
        }
        return hash;
    }
    //bool operator ==(const GameState& other)const;



