#pragma once

#include <ostream>
#include <utility>

#include "../tools/utype.hh"

namespace board
{
    /* The file enum represent the colomns
     * on the board */
    enum class File
    {
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
    };

    /* The rank enum represent the lines
     * on the board */
    enum class Rank
    {
        ONE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
    };

    /* Position represent a coordinate on the board */
    class Position final
    {
    public:
        Position() = default;
        Position(File file, Rank rank);

        File file_get() const;
        Rank rank_get() const;

        friend std::ostream &operator<<(std::ostream &out, const Position &pos)
        {
            char file_char = 'A' + (int)pos.file_;
            int rank_char = 1 + (int)pos.rank_;
            out << file_char << rank_char;
            return out;
        }

        int pos_to_index()
        {
            int rank = (int)rank_;
            int file = (int)file_;
            return file + (rank * 8);
        }

        bool operator==(const Position &pos) const;
        bool operator!=(const Position &pos) const;

    private:
        File file_;
        Rank rank_;
    };

} // namespace board

#include "position.hxx"
