//
// Created by max on 24/04/19.
//

#include "ZobristHash.h"
#include <random>
#include <fstream>

ZobristHash::ZobristHash() {

    //Init the table with random bitstrings

    std::default_random_engine generator;

    /*Generating the seed*/
    unsigned int random_seed, random_seed_a, random_seed_b;
    std::ifstream file ("/dev/urandom", std::ios::binary);
    if (file.is_open())
    {
        char * memblock;
        int size = sizeof(int);
        memblock = new char [size];
        file.read (memblock, size);
        file.close();
        random_seed_a = *reinterpret_cast<int*>(memblock);
        delete[] memblock;
    }// end if
    else
    {
        random_seed_a = 0;
    }
    random_seed_b = time(0);
    random_seed = random_seed_a xor random_seed_b;

    /*Seeding the generator*/
    generator.seed(random_seed);

    std::uniform_int_distribution<uint64_t> distribution(1, 9223372036854775807);
    for (int i = 0; i < 81; i++) {
        for (int j = 0; j < 3; j++) {
            table[i][j] = distribution(generator);
        }
    }
}

uint64_t ZobristHash::hash(const Board &b) const {
    uint64_t h = 0;
    for (int i = 0; i < 81; i++) {
        Position pos = {i%9, i/9};
        if (b.board[pos.col][pos.row] != Pawn::Empty &&
            b.board[pos.col][pos.row] != Pawn::EmptyThrone &&
            b.board[pos.col][pos.row] != Pawn::EmptyCitadel) {


            /*Qua c'è da fare una mappa al post di sto coso*/
            int j = 0;
            if (b.board[pos.col][pos.row] == Pawn::King) {
                j = king;
            } else if (b.board[pos.col][pos.row] == Pawn::White) {
                j = white_pawn;
            } else {
                j = black_pawn;
            }
            h ^= table[i][j];
        }
    }
    return h;
}
