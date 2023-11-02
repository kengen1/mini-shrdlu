#include "Solver.h"

class HumanSolver : public Solver {
    private:
        int steps = 1;
    public:
        HumanSolver():Solver(){}

        void getMove(State* state, vector<Goal> goals, Action action) override {}
        void getMove(State* state, Goal* goal, Action action);
};

void HumanSolver::getMove(State* state, Goal* goal, Action action){
    int n = state->getSize()-1;
    Goal g = *goal;
    State s = *state;
    while(!s.isGoalState(g) && steps <= 100){

        do{
            cout << "\nEnter [column] [0-" << n << "] to move FROM: ";
            cin >> action.source;
            cout << "Enter [column][0-" << n << "] to move TO: ";
            cin >> action.destination;
            cout << endl;
            if(!state->isValidMove(action.source,action.destination))
                cout << "Move is not valid, please try again." << endl;
        }while(!state->isValidMove(action.source,action.destination));

        if(s.moveBlock(action)){
            cout << "\nStep:" << steps << " --> [" << s.getValue() << "] from column " << action.source << " to " << action.destination << endl;
            s.printBoard();
            if(s.isGoalState(g)){
                cout << "Goal (" << g.getGoalBlock() << ", " << g.getGoalRow() << ", " << g.getGoalColumn() << ") has been achieved at step " << steps << endl;
                break;
            }else if(steps == 100){
                cout << "Maximal 100 steps has been reached! Solver failed to achieve the goal after visited 100 states!";
            }
            steps++;
        }
    }
}