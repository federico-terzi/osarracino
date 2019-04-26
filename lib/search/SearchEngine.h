//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_SEARCHENGINE_H
#define OSARRACINO_SEARCHENGINE_H

#include <evaluator/TorettoWhiteEvaluator.h>
#include <evaluator/BlackEvaluator.h>
#include <movegenerator/MoveGenerator.h>
#include <movegenerator/ThetaMoveGenerator.h>
#include <movegenerator/ArnoldMoveGenerator.h>
#include <util/Timer.h>
#include <limits>

template<typename T>
class SearchEngine {
public:
    std::string make_decision(const Board &b) {
        reset_parameters();

        TorettoWhiteEvaluator white_eval;
        BlackEvaluator black_eval;
        ArnoldMoveGenerator move_generator;

        // Perform the actual search
        Move move;
        if (b.is_white) {
            move = __make_decision_internal(b, white_eval, move_generator);
        }else{
            move = __make_decision_internal(b, black_eval, move_generator);
        }

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
        timer.reset();
        move_count = 0;
    }

    Timer timer = Timer(5);
    int move_count = 0;
};

#endif //OSARRACINO_SEARCHENGINE_H
