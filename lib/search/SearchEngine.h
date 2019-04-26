//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_SEARCHENGINE_H
#define OSARRACINO_SEARCHENGINE_H

#include <movegenerator/MoveGenerator.h>
#include <util/Timer.h>
#include <limits>

template <typename T>
class SearchEngine {
public:
    std::string make_decision(const Board& b) {
        reset_parameters();

        // Perform the actual search
        auto move = __make_decision_internal(b);

        float elapsed = timer.elapsed();
        float speed = float(move_count) / elapsed;
        std::cout << "Explored " << move_count << " moves in " << elapsed << " seconds " << std::endl;
        std::cout << "Speed: " << speed << " moves/second." << std::endl;
        std::cout << "Evaluation completed. Move: " << move << std::endl;

        std::string color = b.is_white ? "WHITE" : "BLACK";
        return std::string(
                "{\"from\":\"" + move.from.to_move() + "\",\"to\":\"" + move.to.to_move() + "\",\"turn\":\"" + color + "\"}");
    }

    Move __make_decision_internal(const Board &b) {
        return static_cast<T&>(*this).__make_decision_internal(b);
    }

    void reset_parameters() {
        timer.reset();
        move_count = 0;
    }

    Timer timer = Timer(5);
    int move_count = 0;
};

#endif //OSARRACINO_SEARCHENGINE_H
