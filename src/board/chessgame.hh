#pragma once

#include <iostream>
#include <vector>

#include "../pieces/color.hh"
#include "moves.hh"
#include "player.hh"

namespace board
{
    class Chessgame
    {
    public:
        // ===== Functions ===== //
        // Generate an instance of the Chessgame class
        Chessgame();
        Chessgame(std::string fen_config);

        // Function used to test if an index in a bitboard is occupied
        // is_occupied returns true if there is a piece
        // is_free returns true if there is no piece
        static constexpr bool is_occupied(uint64_t board, int index)
        {
            return (board & (1ULL << index));
        }
        static constexpr bool is_free(uint64_t board, int index)
        {
            return !(board & (1ULL << index));
        }

        // +++++ Index Functions +++++ //
        // Set a given index (place a piece)
        void modify_index(int board_index, int index);
        void clear_index(int board_index, int index);
        // Function used to get the board index depending of the piece symbol
        int index_from_char(char symbol);

        // +++++ Printing functions (debug uses) +++++ //
        std::string print_single_board(int board_index);
        std::string print_full_board();
        std::string fen_from_game();
        std::string print_color_boards();

        // Initialize a board with the basic board to start the game
        void init_basic_board();
        // Function used to initialize the board of the game from a Fen string
        void init_board_from_fen(std::string fen_str);

        // Function used to clear all the bitboards
        void clear_boards();

        // Function generating full white and black boards
        void generate_color_boards();
        uint64_t get_white_board();
        uint64_t get_black_board();
        std::vector<uint64_t> get_boards();

        // Function used to execute a move on the board
        void execute_castling(int end);
        void execute_en_passant(int start, int end);
        void execute_move(Move move);

        // ===== Variables ===== //

        // Color of the player that is playing
        Color playing_color_;

        // Players
        Player black_player_;
        Player white_player_;

        // Castling White
        bool castling_K_;
        bool castling_Q_;
        // Castling Black
        bool castling_k_;
        bool castling_q_;

        // En passant indexes default -1, set to end +- 8 when pawn moves jump 2
        int en_passant_w_;
        int en_passant_b_;

    private:
        // Bitboards 'PRNBQKprnbqk'
        std::vector<uint64_t> boards_;
        // Color boards to check collisions
        uint64_t white_board_;
        uint64_t black_board_;
    };

} // namespace board
