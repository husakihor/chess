#pragma once

#include "moves.hh"

namespace board
{
    class Chessgame;
    class Player final
    {
    public:
        // ====== FUNCTIONS ====== //
        Player() = default;
        Player(bool is_white)
            : is_white_(is_white)
        {}

        // +++++ Generating moves +++++ //
        std::vector<Move> &generate_player_moves(Chessgame &game);

        // +++++ Checking moves +++++ //
        bool is_valid_move(const Move &move, Chessgame &game) const;
        bool is_in_check(Chessgame &game) const;

        // ====== VARIABLES ====== //
        bool is_white_;
        std::vector<Move> player_moves_;
        // std::vector<Move> oponent_moves_;

    private:
        // ====== CHECK IF A SPECIFIC PIECE MOVE IS VALID ====== //
        bool is_valid_pawn_move(const Move &move, Chessgame &game) const;
        bool is_valid_knight_move(const Move &move, Chessgame &game) const;
        bool is_valid_bishop_move(const Move &move, Chessgame &game) const;
        bool is_valid_rook_move(const Move &move, Chessgame &game) const;
        bool is_valid_queen_move(const Move &move, Chessgame &game) const;
        bool is_valid_king_move(const Move &move, Chessgame &game) const;
        bool is_valid_castling(const Move &move, Chessgame &game) const;

        // ====== GENERATE ALL POSSIBLE MOVES FOR EACH TYPE OF PIECE ====== //
        void generate_pawn_moves(Chessgame &game);
        void generate_knight_moves(Chessgame &game);
        void generate_bishop_moves(Chessgame &game);
        void generate_rook_moves(Chessgame &game);
        void sub_generate_queen_moves(Chessgame &game,
                                      std::vector<int> &diag_move_adds,
                                      std::vector<int> &front_move_adds, int i);
        void generate_queen_moves(Chessgame &game);
        void generate_king_moves(Chessgame &game);
        void generate_castling_moves(Chessgame &game);

        // ===== CHECKING IF THE KING IS IN CHECK ===== //
        bool check_castling_path(const Chessgame &game, const int &start,
                                 const int &end) const;
        bool is_king_check(const std::vector<uint64_t> &boards,
                           const int &king_index, const int &start_col) const;
        bool is_pawn_check(const std::vector<uint64_t> &boards,
                           const int &king_index, const int &start_col) const;
        bool is_knight_check(const uint64_t &boards, const int &king_index,
                             const int &start_col) const;
        bool is_bishop_check(const std::vector<uint64_t> &boards,
                             const uint64_t &white_board,
                             const uint64_t &black_board, const int &king_index,
                             const int &start_col) const;
        bool is_rook_check(const std::vector<uint64_t> &boards,
                           const uint64_t &white_board,
                           const uint64_t &black_board, const int &king_index,
                           const int &start_col) const;
    };
} // namespace board
