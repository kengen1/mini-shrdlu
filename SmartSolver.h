#include "Solver.h"
#include <queue>
#include <vector>
#include <functional> // for std::greater

/* Best-first search
   Best-action search with heuristic */

class SmartSolver : public Solver{
    private:
        int steps = 0;
        vector<State> visitedList;
    public:
        SmartSolver():Solver(){}
        void getMove(State* state, vector<Goal> goals, Action action) override {}
        void getMove(State* state, Goal* goal, Action action);
        priority_queue<Action>generateLegalMoves(State s, Goal* goal, Action action);
        int getHeuristicValues(State temp, Goal* goal, int source, int destination);
        bool isVisitedState(State s);
};

void SmartSolver::getMove(State* state, Goal* goal, Action action){
    Goal g = *goal;
    State s = *state;

    if(state->isGoalState(g)){
        return;
    }

    while(steps < 100){
        if(s.isGoalState(g)){
            cout << "==============================ENDGAME INFO==============================" << endl;
            cout << "Goal (" << g.getGoalBlock() << ", " << g.getGoalRow() << ", " << g.getGoalColumn() << ") has been achieved at step " << steps << endl;
            break;
        }

        priority_queue<Action> legalMoves;
        steps++;
        legalMoves = generateLegalMoves(s, goal, action);
        visitedList.push_back(s);

        if(!legalMoves.empty()){
            State temp(s);
            Action frontOfQueue = legalMoves.top();
            if(temp.moveBlock(frontOfQueue)){
                cout << "\nStep:" << steps << " --> [" << temp.getValue() << "] from column " << frontOfQueue.source << " to " << frontOfQueue.destination << " with heuristic value " << frontOfQueue.heuristic << endl;
                temp.printBoard();
                s = temp;
            }
        }else{
            cout << "No more legal moves without repetition.\n";
            break;
        }
    }

    if(steps == 100){
        cout << "Maximal 100 steps has been reached! Solver failed to achieve the goal after visited 100 states!";
    }
}

std::priority_queue<Action, std::vector<Action>> SmartSolver::generateLegalMoves(State s, Goal* goal, Action action) {
    priority_queue<Action> legalMoves;
    for(int fromCol=0; fromCol < s.getSize(); fromCol++){
        for(int toCol=0; toCol < s.getSize(); toCol++){
            State temp = s;

            Action a(fromCol,toCol);
            if(temp.isValidMove(fromCol, toCol)){
                if(temp.moveBlock(a)){
                    int heuristic = getHeuristicValues(temp, goal, fromCol, toCol);
                    Action act = Action(fromCol, toCol, heuristic);
                    //temp.printBoard();
                    legalMoves.push(act);
                }
            }
        }
    }
    return legalMoves;
}

int SmartSolver::getHeuristicValues(State temp, Goal* goal, int source, int destination){
    int h=0;
    Goal g = *goal;

    if(temp.isGoalState(g)){
        return 100;
    }

    if(isVisitedState(temp)){
        return 0;
    }

    if(temp.is_block_on_top_of_a_column(g.getGoalBlock())){
        h+=50;
    }

    if(temp.is_goal_position_ready(g.getReversedRow(), g.getGoalColumn())){
        h+=40;
    }

    if(temp.remove_block_on_top_of_block(g.getGoalBlock(), source)){
        h+=30;
    }

    if(temp.remove_block_on_top_of_goal_position(g.getReversedRow(),g.getGoalColumn(), source)){
        h+=20;
    }

    if(temp.is_block_under_goal_position_needed(g.getReversedRow(),g.getGoalColumn(), destination)){
        h+=10;
    }
    return h;
}

bool SmartSolver::isVisitedState(State s){
    for(int i=0; i<visitedList.size(); i++){
        if(s==visitedList[i]){
            return true;
        }
    }
    return false;
}