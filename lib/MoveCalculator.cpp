//
// Created by max on 11/04/19.
//

#include "MoveCalculator.h"
#include <algorithm>


std::bitset<9> MoveCalculator::white_mask[9];
std::bitset<3> MoveCalculator::black_hor_mask[2];
std::bitset<2> MoveCalculator::black_ver_mask[3];

// Input: Board object and a position
// Output: a vector of positions that represents the cells which that pawn can go
std::vector<Action::Position> MoveCalculator::Get_All_Moves(const Board &b, const Action::Position &pos) {

    // String used to track the column of the mask.
    // No method found to take all the row in one instruction.
    std::string vert = "";

    for(int i = 0; i < 9; i++){
        // If is white turn or (the pawn is not black and inside the citadel)
        if(b.is_white || b.board[pos.column][pos.row] != Pawn::FullCitadel){
            // Apply the mask used to take out the citadels as legal moves and store the bit inside the string
            // The bit is matrix_of_bits[col][row]
            vert+= (b.empty[i] & MoveCalculator::white_mask[i]).to_string()[pos.column];
        } else {
            // You are black and inside the citadel.
            vert+= b.empty[i].to_string()[pos.column];
        }

    }

    // Init the strings used to store parts of the matrix.
    // One for each directions that a pawn can go.
    std::string right;
    std::string left;
    std::string down;
    std::string up;

    // Same as before
    if (b.is_white || b.board[pos.column][pos.row] != Pawn::FullCitadel){

        // Take the matrix ROW apply the masks to take out the citadel and take the substring of that bitstring
        // That goes from col + 1 to the end.
        // Col + 1 stands for the next cell to the right.
        right = ((b.empty[pos.row] & MoveCalculator::white_mask[pos.row])).to_string();
        reverse(right.begin(), right.end());
        right = right.substr(pos.column+1);
        // Take the matrix ROW apply the masks to take out the citadel and take the substring of that bitstring
        // That goes from the start to column
        // Col stands for the next cell to the left.
        // The second parameter in the substr is not included in the range so we need to specify column instead of column -1
        left = ((b.empty[pos.row] & MoveCalculator::white_mask[pos.row])).to_string();
        reverse(left.begin(), left.end());
        left = left.substr(0, pos.column);

        // Those are the vertical moves, here we have already applied the masks for the citadels so there is no need to
        // Reapply.
        // The strategy is the same as before.
        up = vert.substr(0, pos.row);
        down = vert.substr(pos.row+1);

    }else {
        // This is the case where we don't need to apply masks for the citadels.
        right = b.empty[pos.row].to_string().substr(pos.column+1);
        left = b.empty[pos.row].to_string().substr(0, pos.column);
        up = vert.substr(0, pos.row);
        down = vert.substr(pos.row+1);

    }
    // Create the vector of possible moves
    std::vector<Action::Position> moves;


    // Here I analyze the possible moves to the right
    // So i need to analyze the string in the classic way (first to last)
    // The 0 represents that the cell is full so i need to stop to analyze
    // Because that row is blocked by another pawn.
    for (int i = 0; i+pos.column+1 < 9 && right[i] != '0' && i < right.length(); i++){
        moves.push_back(Action::Position{i+pos.column+1, pos.row});
    }

    // Here I analyze the possible moves that goes down.
    // So i need to analyze the string in the classic way (first to last)
    // The 0 represents that the cell is full so i need to stop to analyze
    // Because that column is blocked by another pawn.
    for (int i = 0; i+pos.row < 9 && down[i] != '0' && i < down.length(); i++){
        moves.push_back(Action::Position{pos.column, i+pos.row+1});
    }

    // Here I analyze the possible moves that goes left.
    // So i need to analyze the string in another way (last to first)
    // Because the last char of the string represents the closest to the pawn that we are analyzing
    // The 0 represents that the cell is full so i need to stop to analyze
    // Because that row is blocked by another pawn.
    for(int i = left.length()-1; i >= 0 && left[i] != '0'; i--) {
        moves.push_back(Action::Position{i, pos.row});
    }

    // Here I analyze the possible moves that goes left.
    // So i need to analyze the string in another way (last to first)
    // Because the last char of the string represents the closest to the pawn that we are analyzing
    // The 0 represents that the cell is full so i need to stop to analyze
    // Because that column is blocked by another pawn.
    for(int i = up.length()-1; i>= 0 && up[i] != '0'; i--) {
        moves.push_back(Action::Position{pos.column+1, i});
    }

    return moves;
}

void MoveCalculator::init_masks() {

    // Set all rows of the matrix to 1
    for (auto &bitmask: MoveCalculator::white_mask) {
        bitmask.set();
    }

    /*Setting citadel mask -> citadels cells =  0*/
    MoveCalculator::white_mask[0].reset(3);
    MoveCalculator::white_mask[0].reset(4);
    MoveCalculator::white_mask[0].reset(5);

    MoveCalculator::white_mask[1].reset(4);

    MoveCalculator::white_mask[3].reset(0);
    MoveCalculator::white_mask[3].reset(8);

    MoveCalculator::white_mask[4].reset(0);
    MoveCalculator::white_mask[4].reset(1);
    MoveCalculator::white_mask[4].reset(7);
    MoveCalculator::white_mask[4].reset(8);

    MoveCalculator::white_mask[5].reset(0);
    MoveCalculator::white_mask[5].reset(8);

    MoveCalculator::white_mask[7].reset(4);

    MoveCalculator::white_mask[8].reset(3);
    MoveCalculator::white_mask[8].reset(4);
    MoveCalculator::white_mask[8].reset(5);

    /*Set black horizontal mask*/
    MoveCalculator::black_hor_mask[0].set(0);
    MoveCalculator::black_hor_mask[1].set(2);

    /*Set black vertical mask*/
    MoveCalculator::black_ver_mask[0].set(1);
    MoveCalculator::black_ver_mask[2].set(1);
}



