#include "Solver.h"
#include <queue>

/* Best-first search
   Best-action search without heuristic */

class SimpleSolver : public Solver {
    private:
        int steps=1;
    public:
        SimpleSolver():Solver(){}
        void getMove(State* state, vector<Goal> goals, Action action) override {}
        void getMove(State* state, Goal* goal, Action action);
        queue<Action> generateLegalMoves(State state);
};

void SimpleSolver::getMove(State* state, Goal* goal, Action action){
    State s = *state;
    Goal g = *goal;
    vector<State>visitedList = {s};

    while(!s.isGoalState(g) && steps <= 100){     //&& not reached maximal steps
        queue<Action> legalMoves = generateLegalMoves(s);

        while(!legalMoves.empty()){
            bool flag = false;
            Action frontOfQueue = legalMoves.front();
            State temp(s);
            visitedList.push_back(temp);

            temp.moveBlock(frontOfQueue);

            for(int i=0; i<visitedList.size(); i++){
                if(temp == visitedList[i])
                    flag = true;
                }

            if(flag){
                legalMoves.pop();
                continue;
            }else{
                s = temp;
	            cout << "\nStep:" << steps << " --> [" << s.getValue() << "] from column " << frontOfQueue.source << " to " << frontOfQueue.destination << endl;
                break;
            }
        }

        if(legalMoves.empty()){
            cout << "No more legal moves without repetition.\n";
            break;
        }

        s.printBoard();

        if(s.isGoalState(g)){
            cout << "==============================ENDGAME INFO==============================" << endl;
            cout << "Goal (" << g.getGoalBlock() << ", " << g.getGoalRow() << ", " << g.getGoalColumn() << ") has been achieved at step " << steps << endl;
            break;
        }else if(steps == 100){
            cout << "Maximal 100 steps has been reached! Solver failed to achieve the goal after visited 100 states!";
        }
        steps++;
    }
}

queue<Action> SimpleSolver::generateLegalMoves(State state){
    queue<Action> legalMoves;

    for(int fromCol=0; fromCol < state.getSize(); fromCol++){
        for(int toCol=0; toCol < state.getSize(); toCol++){
            if(state.isValidMove(fromCol,toCol) && fromCol != toCol){
                Action a(fromCol,toCol);
                    legalMoves.push(a);
            }
        }
    }
    return legalMoves;
}