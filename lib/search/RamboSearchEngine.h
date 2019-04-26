//
// Created by freddy on 26/04/19.
//

#ifndef OSARRACINO_RAMBOSEARCHENGINE_H
#define OSARRACINO_RAMBOSEARCHENGINE_H

#include "SearchEngine.h"
#include <evaluator/Evaluator.h>

class RamboSearchEngine : public SearchEngine<RamboSearchEngine> {
public:
    Move __make_decision_internal(const Board &b);

    template<typename EvalType, typename MoveGeneratorType>
    int minimax(int depth,
                const Evaluator<EvalType> &eval,
                const MoveGenerator<MoveGeneratorType> &move_generator,
                bool maximizing_player,
                const Board &board,
                int alpha, int beta);
};


#endif //OSARRACINO_RAMBOSEARCHENGINE_H
