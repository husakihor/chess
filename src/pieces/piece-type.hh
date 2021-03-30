#pragma once

#include <array>
#include <optional>
#include <stdexcept>

namespace board
{
    /* PieceType is an enum representing every possible
     * piece type present on the board. The char associated
     * with each value is the ascii char representing the
     * piece on the board */
    enum class PieceType
    {
        QUEEN,
        ROOK,
        BISHOP,
        KNIGHT,
        PAWN,
        KING, // Last position allows better optimization (order-move)
    };

    using opt_piecetype_t = std::optional<PieceType>;

    /* Can be useful to iterate over PieceTypes */
    constexpr std::array<PieceType, 6> piecetype_array{
        PieceType::QUEEN,  PieceType::ROOK, PieceType::BISHOP,
        PieceType::KNIGHT, PieceType::PAWN, PieceType::KING
    };

    inline PieceType char_to_piece(const char c)
    {
        switch (c)
        {
        case 'P':
            return PieceType::PAWN;
        case 'N':
            return PieceType::KNIGHT;
        case 'B':
            return PieceType::BISHOP;
        case 'R':
            return PieceType::ROOK;
        case 'Q':
            return PieceType::QUEEN;
        case 'K':
            return PieceType::KING;
        default:
            throw std::invalid_argument("Unknown piece");
        }
    }

    inline char piece_to_char(const PieceType piece, bool is_black)
    {
        int lower = 0;
        if (is_black)
            lower = 32;
        switch (piece)
        {
        case PieceType::PAWN:
            return 'P' + lower;
        case PieceType::KNIGHT:
            return 'N' + lower;
        case PieceType::BISHOP:
            return 'B' + lower;
        case PieceType::ROOK:
            return 'R' + lower;
        case PieceType::QUEEN:
            return 'Q' + lower;
        case PieceType::KING:
            return 'K' + lower;
        default:
            throw std::invalid_argument("Unknown piece");
        }
    }

    inline std::string print_piece(const PieceType piece)
    {
        switch (piece)
        {
        case PieceType::PAWN:
            return "Pawn";
        case PieceType::KNIGHT:
            return "Knight";
        case PieceType::BISHOP:
            return "Bishop";
        case PieceType::ROOK:
            return "Rook";
        case PieceType::QUEEN:
            return "Queen";
        case PieceType::KING:
            return "King";
        default:
            throw std::invalid_argument("Unknown piece");
        }
    }

} // namespace board
