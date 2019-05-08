//
// Created by freddy on 08/05/19.
//

#ifndef OSARRACINO_PARAMETRIZEDPLAYERPROFILE_H
#define OSARRACINO_PARAMETRIZEDPLAYERPROFILE_H

#include "PlayerProfile.h"

template <typename SearchEngineType, typename EvalType, typename MoveGeneratorType>
class ParametrizedPlayerProfile : public PlayerProfile {
public:
    std::string calculate_move(const Board &b) override {
        Timer timer {Timer(get_config().timeout)};
        engine.set_timer(timer);
        return engine.make_decision(b, eval, move_generator);
    }

    void print_configuration() override {
        std::cout << "SearchEngine: " << engine.get_name() << std::endl;
        std::cout << "Evaluator: " << eval.get_name() << std::endl;
        std::cout << "MoveGenerator: " << move_generator.get_name() << std::endl;
    }

private:
    SearchEngineType engine;
    EvalType eval;
    MoveGeneratorType move_generator;
};

#endif //OSARRACINO_PARAMETRIZEDPLAYERPROFILE_H
