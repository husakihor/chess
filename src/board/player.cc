#include "player.hh"

#include "chessgame.hh"

namespace board
{
    // ++++++ MOVES GENERATION ++++++ //
    // ====== GENERATE ALL POSSIBLE MOVES FOR A PLAYER ====== //
    std::vector<Move> &Player::generate_player_moves(Chessgame &game)
    {
        generate_pawn_moves(game);
        generate_rook_moves(game);
        generate_bishop_moves(game);
        generate_knight_moves(game);
        generate_queen_moves(game);
        generate_king_moves(game);
        return player_moves_;
    }

    // ====== GENERATE ALL POSSIBLE MOVES FOR EACH TYPE OF PIECE ====== //

    // Function that generate the vector of the valid moves for the Pawn
    void Player::generate_pawn_moves(Chessgame &game)
    {
        constexpr PieceType ptype = PieceType::PAWN;
        int ind_mult = is_white_ ? 1 : -1;
        int board_ind = is_white_ ? 0 : 6;
        uint64_t board = game.get_boards()[board_ind];
        std::vector<int> move_adds = { 7, 8, 9, 16 };
        std::vector<PieceType> promote{ PieceType::BISHOP, PieceType::KNIGHT,
                                        PieceType::QUEEN, PieceType::ROOK };
        for (int i = 0; i < 64; i++)
        {
            if (Chessgame::is_occupied(board, i))
            {
                for (auto add : move_adds)
                {
                    int line = (i + (ind_mult * add) + 8) / 8;
                    if ((line == 8 && is_white_) || (line == 1 && !is_white_))
                    {
                        const bool capture = add == 7 || add == 9;
                        for (auto promo_piece : promote)
                        {
                            Move pawn_move = Move(i, i + (ind_mult * add),
                                                  ptype, capture, promo_piece);
                            if (is_valid_move(pawn_move, game))
                                player_moves_.push_back(pawn_move);
                        }
                    }
                    else
                    {
                        const bool capture = add == 7 || add == 9;
                        Move pawn_move =
                            Move(i, i + (ind_mult * add), ptype, capture);
                        if (is_valid_move(pawn_move, game))
                            player_moves_.push_back(pawn_move);
                    }
                }
            }
        }
    }

    // Function that generate the vector of the valid moves for the Rook
    void Player::generate_rook_moves(Chessgame &game)
    {
        constexpr PieceType ptype = PieceType::ROOK;
        int board_ind = is_white_ ? 1 : 7;
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        uint64_t board = game.get_boards()[board_ind];

        for (int i = 0; i < 64; i++)
        {
            if (Chessgame::is_occupied(board, i))
            {
                for (int j = -1; j < 2; j += 2)
                {
                    int new_ind = i;
                    bool capture = false;

                    const int start_row = (new_ind + 8) / 8;
                    while (!capture)
                    {
                        new_ind += j * 1;
                        int end_row = (new_ind + 8) / 8;
                        if (new_ind >= 64 || new_ind < 0
                            || end_row != start_row)
                            break;
                        capture = Chessgame::is_occupied(black_board, new_ind)
                            || Chessgame::is_occupied(white_board, new_ind);
                        Move rook_move = Move(i, new_ind, ptype, capture);
                        if (is_valid_move(rook_move, game))
                            player_moves_.push_back(rook_move);
                    }

                    capture = false;
                    new_ind = i;

                    while (!capture)
                    {
                        new_ind += j * 8;
                        if (new_ind >= 64 || new_ind < 0)
                            break;
                        capture = Chessgame::is_occupied(black_board, new_ind)
                            || Chessgame::is_occupied(white_board, new_ind);
                        Move rook_move = Move(i, new_ind, ptype, capture);
                        if (is_valid_move(rook_move, game))
                            player_moves_.push_back(rook_move);
                    }
                }
            }
        }
    }

    // Function that generate the vector of the valid moves for the Bishop
    void Player::generate_bishop_moves(Chessgame &game)
    {
        constexpr PieceType ptype = PieceType::BISHOP;
        int board_ind = is_white_ ? 3 : 9;
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        uint64_t board = game.get_boards()[board_ind];

        for (int i = 0; i < 64; i++)
        {
            if (Chessgame::is_occupied(board, i))
            {
                for (int j = -1; j < 2; j += 2)
                {
                    bool capture = false;
                    int new_ind = i;
                    int end_col = (i + 8) % 8;

                    while (!capture)
                    {
                        int prev_col = end_col;
                        new_ind += j * 7;
                        end_col = (new_ind + 8) % 8;
                        if (new_ind >= 64 || new_ind < 0)
                            break;
                        if (abs(end_col - prev_col) != 1)
                            break;
                        capture = Chessgame::is_occupied(black_board, new_ind)
                            || Chessgame::is_occupied(white_board, new_ind);
                        Move bishop_move = Move(i, new_ind, ptype, capture);
                        if (is_valid_move(bishop_move, game))
                            player_moves_.push_back(bishop_move);
                    }

                    capture = false;
                    new_ind = i;
                    end_col = (i + 8) % 8;

                    while (!capture)
                    {
                        int prev_col = end_col;
                        new_ind += j * 9;
                        end_col = (new_ind + 8) % 8;
                        if (new_ind >= 64 || new_ind < 0)
                            break;
                        if (abs(end_col - prev_col) != 1)
                            break;
                        capture = Chessgame::is_occupied(black_board, new_ind)
                            || Chessgame::is_occupied(white_board, new_ind);
                        Move bishop_move = Move(i, new_ind, ptype, capture);
                        if (is_valid_move(bishop_move, game))
                            player_moves_.push_back(bishop_move);
                    }
                }
            }
        }
    }

    // Function that generate the vector of the valid moves for the Knight
    void Player::generate_knight_moves(Chessgame &game)
    {
        constexpr PieceType ptype = PieceType::KNIGHT;
        int board_ind = is_white_ ? 2 : 8;
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        uint64_t board = game.get_boards()[board_ind];
        for (int i = 0; i < 64; i++)
        {
            if (Chessgame::is_occupied(board, i))
            {
                for (int j = -1; j < 2; j += 2)
                {
                    bool capture =
                        Chessgame::is_occupied(black_board, i + (j * 15))
                        || Chessgame::is_occupied(white_board, i + (j * 15));
                    Move forward_one = Move(i, i + (j * 15), ptype, capture);
                    capture = Chessgame::is_occupied(black_board, i + (j * 17))
                        || Chessgame::is_occupied(white_board, i + (j * 17));
                    Move forward_two = Move(i, i + (j * 17), ptype, capture);
                    capture = Chessgame::is_occupied(black_board, i + (j * 6))
                        || Chessgame::is_occupied(white_board, i + (j * 6));
                    Move side_one = Move(i, i + (j * 6), ptype, capture);
                    capture = Chessgame::is_occupied(black_board, i + (j * 10))
                        || Chessgame::is_occupied(white_board, i + (j * 10));
                    Move side_two = Move(i, i + (j * 10), ptype, capture);

                    if (is_valid_move(forward_one, game))
                        player_moves_.push_back(forward_one);

                    if (is_valid_move(forward_two, game))
                        player_moves_.push_back(forward_two);

                    if (is_valid_move(side_one, game))
                        player_moves_.push_back(side_one);

                    if (is_valid_move(side_two, game))
                        player_moves_.push_back(side_two);
                }
            }
        }
    }

    // Functions that generate the vector of the valid moves for the Queen
    void Player::sub_generate_queen_moves(Chessgame &game,
                                          std::vector<int> &diag_move_adds,
                                          std::vector<int> &front_move_adds,
                                          int i)
    {
        constexpr PieceType ptype = PieceType::QUEEN;
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        for (int j = -1; j < 2; j += 2)
        {
            int start_row = (i + 8) / 8;
            int start_col = (i + 8) % 8;
            for (auto m_d : diag_move_adds)
            {
                int end_row = (i + 8) / 8;
                int end_col = (i + 8) % 8;
                int new_ind = i;
                while (abs(end_col - start_col) == abs(end_row - start_row))
                {
                    int prev_col = end_col;
                    new_ind += j * m_d;
                    if (new_ind > 64 || new_ind < 0)
                        break;
                    end_row = (new_ind + 8) / 8;
                    end_col = (new_ind + 8) % 8;
                    if (abs(end_col - prev_col) != 1)
                        break;
                    bool capture = Chessgame::is_occupied(black_board, new_ind)
                        || Chessgame::is_occupied(white_board, new_ind);
                    Move queen_diag_move = Move(i, new_ind, ptype, capture);
                    if (is_valid_move(queen_diag_move, game))
                        player_moves_.push_back(queen_diag_move);
                    if (capture)
                        break;
                }
            }
            for (auto m_f : front_move_adds)
            {
                int end_row = (i + 8) / 8;
                int end_col = (i + 8) % 8;
                int new_ind = i;
                while ((start_row == end_row || start_col == end_col))
                {
                    int prev_col = end_col;
                    new_ind += j * m_f;
                    if (new_ind > 64 || new_ind < 0)
                        break;
                    end_row = (new_ind + 8) / 8;
                    end_col = (new_ind + 8) % 8;
                    if (abs(end_col - prev_col) > 1)
                        break;
                    bool capture = Chessgame::is_occupied(black_board, new_ind)
                        || Chessgame::is_occupied(white_board, new_ind);
                    Move queen_front_move = Move(i, new_ind, ptype, capture);
                    if (is_valid_move(queen_front_move, game))
                        player_moves_.push_back(queen_front_move);
                    if (capture)
                        break;
                }
            }
        }
    }

    void Player::generate_queen_moves(Chessgame &game)
    {
        int board_ind = is_white_ ? 4 : 10;
        uint64_t board = game.get_boards()[board_ind];
        std::vector<int> diag_move_adds = { 7, 9 };
        std::vector<int> front_move_adds = { 1, 8 };
        for (int i = 0; i < 64; i++)
        {
            if (Chessgame::is_occupied(board, i))
            {
                sub_generate_queen_moves(game, diag_move_adds, front_move_adds,
                                         i);
            }
        }
    }

    void Player::generate_castling_moves(Chessgame &game)
    {
        constexpr PieceType ptype = PieceType::KING;
        int board_ind = is_white_ ? 5 : 11;
        uint64_t board = game.get_boards()[board_ind];
        int king_index = -1;
        for (int i = 0; i < 64; i++)
        {
            if (Chessgame::is_occupied(board, i))
            {
                king_index = i;
            }
        }
        if (is_white_)
        {
            if (game.castling_K_)
            {
                Move king_side_move =
                    Move(king_index, king_index + 2, ptype, false);
                if (is_valid_move(king_side_move, game))
                {
                    player_moves_.push_back(king_side_move);
                }
            }
            if (game.castling_Q_)
            {
                Move queen_side_move =
                    Move(king_index, king_index - 2, ptype, false);
                if (is_valid_move(queen_side_move, game))
                {
                    player_moves_.push_back(queen_side_move);
                }
            }
        }
        else
        {
            if (game.castling_k_)
            {
                Move king_side_move =
                    Move(king_index, king_index + 2, ptype, false);
                if (is_valid_move(king_side_move, game))
                {
                    player_moves_.push_back(king_side_move);
                }
            }
            if (game.castling_q_)
            {
                Move queen_side_move =
                    Move(king_index, king_index - 2, ptype, false);
                if (is_valid_move(queen_side_move, game))
                {
                    player_moves_.push_back(queen_side_move);
                }
            }
        }
    }

    // Function that generate the vector of the valid moves for the King
    void Player::generate_king_moves(Chessgame &game)
    {
        constexpr PieceType ptype = PieceType::KING;
        int board_ind = is_white_ ? 5 : 11;
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        uint64_t board = game.get_boards()[board_ind];
        for (int i = 0; i < 64; i++)
        {
            if (Chessgame::is_occupied(board, i))
            {
                for (int j = -1; j < 2; j += 2)
                {
                    bool capture =
                        Chessgame::is_occupied(black_board, i + (j * 8))
                        || Chessgame::is_occupied(white_board, i + (j * 8));
                    Move forward_move = Move(i, i + (j * 8), ptype, capture);
                    capture = Chessgame::is_occupied(black_board, i + (j * 1))
                        || Chessgame::is_occupied(white_board, i + (j * 1));
                    Move side_move = Move(i, i + (j * 1), ptype, capture);
                    capture = Chessgame::is_occupied(black_board, i + (j * 7))
                        || Chessgame::is_occupied(white_board, i + (j * 7));
                    Move forward_side_one =
                        Move(i, i + (j * 7), ptype, capture);
                    capture = Chessgame::is_occupied(black_board, i + (j * 9))
                        || Chessgame::is_occupied(white_board, i + (j * 9));
                    Move forward_side_two =
                        Move(i, i + (j * 9), ptype, capture);

                    if (is_valid_move(forward_move, game))
                        player_moves_.push_back(forward_move);

                    if (is_valid_move(forward_side_one, game))
                        player_moves_.push_back(forward_side_one);

                    if (is_valid_move(side_move, game))
                        player_moves_.push_back(side_move);

                    if (is_valid_move(forward_side_two, game))
                        player_moves_.push_back(forward_side_two);
                }
            }
        }
        generate_castling_moves(game);
    }

    // ++++++ VALID MOVE CHECKER ++++++ //
    // ====== CHECK IF A GIVEN MOVE IS VALID ====== //
    bool Player::is_valid_move(const Move &move, Chessgame &game) const
    {
        if (move.start_get() == move.end_get() || move.end_get() < 0
            || move.end_get() > 63)
            return false;

        char symbol = piece_to_char(move.piece_get(),
                                    game.playing_color_ == Color::BLACK);
        uint64_t piece_board = game.get_boards()[game.index_from_char(symbol)];
        if (Chessgame::is_free(piece_board, move.start_get()))
            return false;

        bool is_legal_move = false;

        if (move.piece_get() == PieceType::PAWN)
            is_legal_move = is_valid_pawn_move(move, game);

        else if (move.piece_get() == PieceType::ROOK)
            is_legal_move = is_valid_rook_move(move, game);

        else if (move.piece_get() == PieceType::BISHOP)
            is_legal_move = is_valid_bishop_move(move, game);

        else if (move.piece_get() == PieceType::KNIGHT)
            is_legal_move = is_valid_knight_move(move, game);

        else if (move.piece_get() == PieceType::QUEEN)
            is_legal_move = is_valid_queen_move(move, game);

        else if (move.piece_get() == PieceType::KING)
            is_legal_move = is_valid_king_move(move, game);

        if (is_legal_move)
        {
            Chessgame tmp_game = game;
            tmp_game.execute_move(move);
            if (is_in_check(tmp_game))
            {
                return false;
            }
            else
                return true;
        }
        return false;
    }

    // ====== CHECK IF A SPECIFIC PIECE MOVE IS VALID ====== //
    // ----- Pawn check ----- //
    bool Player::is_valid_pawn_move(const Move &move, Chessgame &game) const
    {
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        int start = move.start_get();
        int end = move.end_get();

        int start_col = (start + 8) % 8;
        int start_row = (start + 8) / 8;
        int end_col = (end + 8) % 8;
        int end_row = (end + 8) / 8;

        if ((is_white_ && end_row == 8) || (!is_white_ && end_row == 1))
        {
            if (!move.promotion_get().has_value())
                return false;
            if (move.promotion_get().value() == PieceType::KING
                || move.promotion_get().value() == PieceType::PAWN)
                return false;
        }

        int rank = (is_white_) ? 1 : -1;

        if ((is_white_ && end < start) || (!is_white_ && end > start))
            return false;

        bool W_collide = Chessgame::is_occupied(white_board, end);
        bool B_collide = Chessgame::is_occupied(black_board, end);

        if (end_row == start_row + (rank * 2) && end_col == start_col)
        {
            if ((is_white_ && start_row != 2) || (!is_white_ && start_row != 7))
                return false;
            if (W_collide || B_collide)
                return false;

            W_collide = Chessgame::is_occupied(white_board, start + (rank * 8));
            B_collide = Chessgame::is_occupied(black_board, start + (rank * 8));
            if (W_collide || B_collide)
                return false;

            return true;
        }

        if ((end_col == start_col + 1 || end_col == start_col - 1)
            && end_row == start_row + (rank * 1))
        {
            W_collide = Chessgame::is_occupied(white_board, end);
            B_collide = Chessgame::is_occupied(black_board, end);
            if (end == game.en_passant_b_ && is_white_)
                B_collide = true;
            else if (end == game.en_passant_w_ && !is_white_)
                W_collide = true;

            return move.capture_get()
                && ((is_white_ && B_collide) || (!is_white_ && W_collide));
        }

        if (abs(end_col) == start_col && abs(end_row) == start_row + (rank * 1))
        {
            if (W_collide || B_collide)
                return false;
            return true;
        }
        return false;
    }

    // ----- Knight check ----- //
    bool Player::is_valid_knight_move(const Move &move, Chessgame &game) const
    {
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        int start = move.start_get();
        int end = move.end_get();

        int start_col = (start + 8) % 8;
        int start_row = (start + 8) / 8;
        int end_col = (end + 8) % 8;
        int end_row = (end + 8) / 8;

        if (abs(end_row - start_row) > 2 && abs(end_col - start_col) > 2)
            return false;

        if (abs(end_col - start_col) + abs(end_row - start_row) != 3)
            return false;

        bool W_collide = Chessgame::is_occupied(white_board, end);
        bool B_collide = Chessgame::is_occupied(black_board, end);
        if (W_collide || B_collide)
            return move.capture_get()
                && ((is_white_ && B_collide) || (!is_white_ && W_collide));

        return true;
    }

    // ----- Bishop check ----- //
    bool Player::is_valid_bishop_move(const Move &move, Chessgame &game) const
    {
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        int start = move.start_get();
        int end = move.end_get();
        int start_col = (start + 8) % 8;
        int start_row = (start + 8) / 8;
        int end_col = (end + 8) % 8;
        int end_row = (end + 8) / 8;

        if (abs(end_col - start_col) != abs(end_row - start_row))
            return false;

        int rank = 0;
        if (end < start)
            rank =
                (start_col < end_col) ? -7 : -9; // 7 -> Droite || 9 -> Gauche
        else
            rank = (start_col < end_col) ? 9 : 7; // 1 -> Haut || -1 -> Bas

        for (int i = start + rank; true; i += rank)
        {
            bool W_collide = Chessgame::is_occupied(white_board, i);
            bool B_collide = Chessgame::is_occupied(black_board, i);
            if (W_collide || B_collide)
                return (i == end) && move.capture_get()
                    && ((is_white_ && B_collide) || (!is_white_ && W_collide));
            if (i == end)
                break;
        }
        return true;
    }

    // ----- Rook check ----- //
    bool Player::is_valid_rook_move(const Move &move, Chessgame &game) const
    {
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        int start = move.start_get();
        int end = move.end_get();

        int start_col = (start + 8) % 8;
        int start_row = (start + 8) / 8;
        int end_col = (end + 8) % 8;
        int end_row = (end + 8) / 8;

        if (end_col != start_col && end_row != start_row)
            return false;

        int rank = (end > start + 7 || end < start - 7) ? 8 : 1; // Up || Right
        rank *= (end > start) ? 1 : -1; // Si -1 -> Inversion de la direction

        for (int i = start + rank; true; i += rank)
        {
            bool W_collide = Chessgame::is_occupied(white_board, i);
            bool B_collide = Chessgame::is_occupied(black_board, i);
            if (W_collide || B_collide)
                return (i == end) && move.capture_get()
                    && ((is_white_ && B_collide) || (!is_white_ && W_collide));
            if (i == end)
                break;
        }
        return true;
    }

    // ----- Queen check ----- //
    bool Player::is_valid_queen_move(const Move &move, Chessgame &game) const
    {
        return is_valid_rook_move(move, game)
            || is_valid_bishop_move(move, game);
    }

    // ----- King check ----- //
    bool Player::is_valid_king_move(const Move &move, Chessgame &game) const
    {
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        int start = move.start_get();
        int end = move.end_get();

        if ((start == 4 && end == 6) || (start == 4 && end == 2)
            || (start == 60 && end == 62) || (start == 60 && end == 58))
            return is_valid_castling(move, game);

        int start_col = (start + 8) % 8;
        int start_row = (start + 8) / 8;
        int end_col = (end + 8) % 8;
        int end_row = (end + 8) / 8;

        if (end_col > start_col + 1 || end_row > start_row + 1)
            return false;
        if (end_col < start_col - 1 || end_row < start_row - 1)
            return false;

        bool W_collide = Chessgame::is_occupied(white_board, end);
        bool B_collide = Chessgame::is_occupied(black_board, end);
        if (W_collide || B_collide)
            return move.capture_get()
                && ((is_white_ && B_collide) || (!is_white_ && W_collide));

        return true;
    }

    // ----- Castling check ----- //
    bool Player::is_valid_castling(const Move &move, Chessgame &game) const
    {
        if (move.capture_get() || is_in_check(game))
            return false;

        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();

        uint64_t full_board = white_board;
        full_board |= black_board;

        int start = move.start_get();
        int end = move.end_get();

        if (end == 6 && game.castling_K_ == false)
            return false;
        if (end == 2 && game.castling_Q_ == false)
            return false;
        if (end == 62 && game.castling_k_ == false)
            return false;
        if (end == 58 && game.castling_q_ == false)
            return false;

        if (game.castling_K_ == true
            && Chessgame::is_free(game.get_boards()[1], 7))
            return false;
        if (game.castling_Q_ == true
            && Chessgame::is_free(game.get_boards()[1], 0))
            return false;
        if (game.castling_k_ == true
            && Chessgame::is_free(game.get_boards()[7], 63))
            return false;
        if (game.castling_q_ == true
            && Chessgame::is_free(game.get_boards()[7], 56))
            return false;

        if (check_castling_path(game, start, end))
            return false;

        int rank = (start < end) ? 1 : -1;
        int end_line = (start < end) ? end : start - 3;

        for (int i = start + rank; true; i += rank)
        {
            if (Chessgame::is_occupied(full_board, i))
                return false;
            if (i == end_line)
                return true;
        }
        return true;
    }

    bool Player::check_castling_path(const Chessgame &game, const int &start,
                                     const int &end) const
    {
        if (end == 62 || end == 6)
        {
            Move tmp_position = Move(start, start + 1, PieceType::KING, false);
            Chessgame tmp_game = game;
            tmp_game.execute_move(tmp_position);
            if (is_in_check(tmp_game))
                return true;
        }
        else if (end == 2 || end == 58)
        {
            Move tmp_position = Move(start, start - 1, PieceType::KING, false);
            Chessgame tmp_game = game;
            tmp_game.execute_move(tmp_position);
            if (is_in_check(tmp_game))
                return true;
        }
        return false;
    }

    // Function checking if the king is in check
    bool Player::is_in_check(Chessgame &game) const
    {
        uint64_t white_board = game.get_white_board();
        uint64_t black_board = game.get_black_board();
        std::vector<uint64_t> boards = game.get_boards();

        int boards_color = (is_white_) ? 6 : 0; // Enemy pieces boards
        int king_index = -1;
        for (int i = 0; i < 64 && king_index == -1; i++)
        {
            if (Chessgame::is_occupied(boards[(is_white_) ? 5 : 11], i))
            {
                king_index = i;
            }
        }
        if (king_index == -1)
            return false;

        int start_col = (king_index + 8) % 8;

        // ===== CHECKING MENACING KING ===== //
        if (is_king_check(boards, king_index, start_col))
            return true;

        // ===== CHECKING MENACING PAWN ===== //
        if (is_pawn_check(boards, king_index, start_col))
            return true;

        // ===== CHECKING MENACING KNIGHT ===== //
        if (is_knight_check(boards[2 + boards_color], king_index, start_col))
            return true;

        // ===== MENACING TOWER AND QUEEN ===== //
        if (is_rook_check(boards, white_board, black_board, king_index,
                          start_col))
            return true;

        // ===== MENACING BISHOP AND QUEEN ===== //
        if (is_bishop_check(boards, white_board, black_board, king_index,
                            start_col))
            return true;
        return false;
    }

    // Function used to check is a king is threatening the king
    bool Player::is_king_check(const std::vector<uint64_t> &boards,
                               const int &king_index,
                               const int &start_col) const
    {
        int boards_color = (is_white_) ? 6 : 0;
        // Up diag check
        int right_check = king_index + 9;
        if (start_col + 1 <= 7 && right_check <= 63
            && Chessgame::is_occupied(boards[5 + boards_color], right_check))
            return true;
        int left_check = king_index + 7;
        if (start_col - 1 >= 0 && left_check <= 63
            && Chessgame::is_occupied(boards[5 + boards_color], left_check))
            return true;
        // Down diag check
        right_check = king_index - 7;
        if (start_col + 1 <= 7 && right_check >= 0
            && Chessgame::is_occupied(boards[5 + boards_color], right_check))
            return true;
        left_check = king_index - 9;
        if (start_col - 1 >= 0 && left_check >= 0
            && Chessgame::is_occupied(boards[5 + boards_color], left_check))
            return true;
        // Right and left check
        right_check = king_index + 1;
        if (start_col + 1 <= 7 && right_check <= 63
            && Chessgame::is_occupied(boards[5 + boards_color], right_check))
            return true;
        left_check = king_index - 1;
        if (start_col - 1 >= 0 && left_check >= 0
            && Chessgame::is_occupied(boards[5 + boards_color], left_check))
            return true;
        // Up front check
        int up_check = king_index + 8;
        if (up_check <= 63
            && Chessgame::is_occupied(boards[5 + boards_color], up_check))
            return true;
        int down_check = king_index - 8;
        if (down_check >= 0
            && Chessgame::is_occupied(boards[5 + boards_color], down_check))
            return true;
        return false;
    }

    // Function used to check is a pawn is threatening the king
    bool Player::is_pawn_check(const std::vector<uint64_t> &boards,
                               const int &king_index,
                               const int &start_col) const
    {
        if (is_white_)
        {
            int right_check = king_index + 9;
            if (start_col + 1 <= 7 && right_check <= 63
                && Chessgame::is_occupied(boards[6], right_check))
                return true;
            int left_check = king_index + 7;
            if (start_col - 1 >= 0 && left_check <= 63
                && Chessgame::is_occupied(boards[6], left_check))
                return true;
        }
        else
        {
            int right_check = king_index - 7;
            if (start_col + 1 <= 7 && right_check >= 0
                && Chessgame::is_occupied(boards[0], right_check))
                return true;
            int left_check = king_index - 9;
            if (start_col - 1 >= 0 && left_check >= 0
                && Chessgame::is_occupied(boards[0], left_check))
                return true;
        }
        return false;
    }

    // Function used to check is a knight is threatening the king
    bool Player::is_knight_check(const uint64_t &board, const int &king_index,
                                 const int &start_col) const
    {
        if (start_col - 2 >= 0 && king_index + 6 < 64 && king_index + 6 >= 0
            && Chessgame::is_occupied(board, king_index + 6))
            return true;
        if (start_col + 2 <= 7 && king_index + 10 < 64 && king_index + 10 >= 0
            && Chessgame::is_occupied(board, king_index + 10))
            return true;
        if (start_col - 1 >= 0 && king_index + 15 < 64 && king_index + 15 >= 0
            && Chessgame::is_occupied(board, king_index + 15))
            return true;
        if (start_col + 1 <= 7 && king_index + 17 < 64 && king_index + 17 >= 0
            && Chessgame::is_occupied(board, king_index + 17))
            return true;
        // ====== NEGATIVE KNIGHT ====== //
        if (start_col + 2 <= 7 && king_index - 6 < 64 && king_index - 6 >= 0
            && Chessgame::is_occupied(board, king_index - 6))
            return true;
        if (start_col - 2 >= 0 && king_index - 10 < 64 && king_index - 10 >= 0
            && Chessgame::is_occupied(board, king_index - 10))
            return true;
        if (start_col + 1 <= 7 && king_index - 15 < 64 && king_index - 15 >= 0
            && Chessgame::is_occupied(board, king_index - 15))
            return true;
        if (start_col - 1 >= 0 && king_index - 17 < 64 && king_index - 17 >= 0
            && Chessgame::is_occupied(board, king_index - 17))
            return true;

        return false;
    }

    // Function used to check is a bishop or a queen is threatening the king
    bool Player::is_bishop_check(const std::vector<uint64_t> &boards,
                                 const uint64_t &white_board,
                                 const uint64_t &black_board,
                                 const int &king_index,
                                 const int &start_col) const
    {
        int boards_color = (is_white_) ? 6 : 0;
        for (int i = -1; i <= 1; i += 2)
        {
            int col = start_col;
            for (int j = king_index + (i * 9); j >= 0 && j <= 63; j += 9 * i)
            {
                col += i;
                if (col < 0 || col > 7)
                    break;
                if (Chessgame::is_occupied(white_board, j)
                    || Chessgame::is_occupied(black_board, j))
                {
                    if (Chessgame::is_occupied(boards[3 + boards_color], j)
                        || Chessgame::is_occupied(boards[4 + boards_color], j))
                        return true;
                    break;
                }
            }

            col = start_col;

            for (int j = king_index + (i * 7); j >= 0 && j <= 63; j += 7 * i)
            {
                col -= i;
                if (col < 0 || col > 7)
                    break;
                if (Chessgame::is_occupied(white_board, j)
                    || Chessgame::is_occupied(black_board, j))
                {
                    if (Chessgame::is_occupied(boards[3 + boards_color], j)
                        || Chessgame::is_occupied(boards[4 + boards_color], j))
                        return true;
                    break;
                }
            }
        }
        return false;
    }

    // Function used to check is a rook or a queen is threatening the king
    bool Player::is_rook_check(const std::vector<uint64_t> &boards,
                               const uint64_t &white_board,
                               const uint64_t &black_board,
                               const int &king_index,
                               const int &start_col) const
    {
        int boards_color = (is_white_) ? 6 : 0;
        for (int i = -1; i <= 1; i += 2)
        {
            int col = start_col;
            for (int j = king_index + i; j >= 0 && j <= 63; j += i)
            {
                col += i;
                if (col < 0 || col > 7)
                    break;
                if (Chessgame::is_occupied(white_board, j)
                    || Chessgame::is_occupied(black_board, j))
                {
                    if (Chessgame::is_occupied(boards[1 + boards_color], j)
                        || Chessgame::is_occupied(boards[4 + boards_color], j))
                        return true;
                    break;
                }
            }

            for (int j = king_index + (i * 8); j >= 0 && j <= 63; j += 8 * i)
            {
                if (Chessgame::is_occupied(white_board, j)
                    || Chessgame::is_occupied(black_board, j))
                {
                    if (Chessgame::is_occupied(boards[1 + boards_color], j)
                        || Chessgame::is_occupied(boards[4 + boards_color], j))
                        return true;
                    break;
                }
            }
        }
        return false;
    }

} // namespace board
