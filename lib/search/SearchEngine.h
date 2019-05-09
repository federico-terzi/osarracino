//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_SEARCHENGINE_H
#define OSARRACINO_SEARCHENGINE_H

#include <evaluator/Evaluator.h>
#include <movegenerator/MoveGenerator.h>
#include <util/Timer.h>
#include <limits>

template<typename T>
class SearchEngine {
public:
    std::string get_name() const {
        return static_cast<T const&>(*this).get_name();
    }

    template<typename EvalType, typename MoveGeneratorType>
    std::string make_decision(const Board &b, const Evaluator<EvalType> &eval,
                              const MoveGenerator<MoveGeneratorType> &move_generator) {
        reset_parameters();

        // Perform the actual search
        Move move = __make_decision_internal(b, eval, move_generator);

        float elapsed = timer.elapsed();
        float speed = float(move_count) / elapsed;
        std::cout << "Explored " << move_count << " moves in " << elapsed << " seconds " << std::endl;
        std::cout << "Speed: " << speed << " moves/second." << std::endl;
        std::cout << "Evaluation completed. Move: " << move << std::endl;

        std::string color = b.is_white ? "WHITE" : "BLACK";
        return std::string(
                "{\"from\":\"" + move.from.to_move() + "\",\"to\":\"" + move.to.to_move() + "\",\"turn\":\"" + color +
                "\"}");
    }

    template<typename EvalType, typename MoveGeneratorType>
    Move __make_decision_internal(const Board &b,
                                  const Evaluator<EvalType> &eval,
                                  const MoveGenerator<MoveGeneratorType> &move_generator) {

        return static_cast<T &>(*this).__make_decision_internal(b, eval, move_generator);
    }

    void reset_parameters() {
        move_count = 0;
    }

    void set_timer(const Timer& __timer) {
        this->timer = __timer;
    }

    void set_worker_count(int count) {
        worker_count = count;
    }

    Timer timer;
    int worker_count = 4;
    int move_count = 0;
    int quiet_count = 0;
};

#endif //OSARRACINO_SEARCHENGINE_H
