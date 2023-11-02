#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include "State.h"

#ifndef SOLVER_h
#define SOLVER_h

class Solver{
    public:
        Solver(){}
        virtual void getMove(State*, Goal*, Action)=0;
        virtual void getMove(State*, std::vector<Goal>, Action) = 0;

};
#endif //SOLVER_h