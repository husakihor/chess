#include "moves.hh"

namespace board
{
    /* The move symbolize a movement of a piece.
     * It contains every informations related to the action */
    using opt_piece_t = std::optional<PieceType>;

    Move::Move(int start, int end, PieceType piece, bool capture,
               opt_piece_t promotion)
    {
        start_ = start;
        end_ = end;
        piece_ = piece;
        capture_ = capture;
        promotion_ = promotion;
    }

    std::vector<Move> Move::pgn_to_moves(std::vector<PgnMove> moves)
    {
        std::vector<Move> newMoves = std::vector<Move>();

        for (PgnMove move : moves)
        {
            Position start = move.start_get();
            Position end = move.end_get();

            Move newMove =
                Move(start.pos_to_index(), end.pos_to_index(), move.piece_get(),
                     move.capture_get(), move.promotion_get());
            newMoves.push_back(newMove);
        }

        return newMoves;
    }

    // Convertion functions (print and debug uses)
    Position Move::index_to_pos(int index)
    {
        Rank rank = (Rank)(index / 8);
        File file = (File)(index % 8);
        return Position(file, rank);
    }

    // Getters
    int Move::start_get() const
    {
        return start_;
    }

    int Move::end_get() const
    {
        return end_;
    }

    PieceType Move::piece_get() const
    {
        return piece_;
    }

    opt_piece_t Move::promotion_get() const
    {
        return promotion_;
    }

    bool Move::capture_get() const
    {
        return capture_;
    }

    // bool operator==(const Move &other) const;
    // bool operator!=(const Move &other) const;

} // namespace board
