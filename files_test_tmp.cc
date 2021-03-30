#include <iostream>
#include <iterator>
#include <map>

// Function used to test if an index in a bitboard is occupied
bool is_occupied(uint64_t board, int index)
{
    return (board & (1ULL << index));
}
// Function used to print a specified board (debug purposes)
std::string printSingleBoard(uint64_t board, char occupied)
{
    std::string ret = "";
    for (int i = 7; i >= 0; i--)
    {
        for (int j = i * 8; j < (i + 1) * 8; j++)
        {
            if (is_occupied(board, j))
            {
                ret += occupied;
            }
            else
                ret += '.';
        }

        ret += '\n';
    }
    return ret;
}
// Function used to modify a given index in a given board
void modify_index(uint64_t &board, int index)
{
    board &= ~(1ULL << index);
    board |= (1ULL << index);
}

int main(void)
{
    uint64_t board = 0;
    modify_index(board, 1);
    modify_index(board, 7);
    modify_index(board, 63);
    if (!is_occupied(board, 55))
        modify_index(board, 55);
    std::cout << printSingleBoard(board, 'O');
}