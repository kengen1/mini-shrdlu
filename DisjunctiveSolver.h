#include "Solver.h"
#include <queue>
#include <vector>
#include <functional>

class DisjunctiveSolver : public Solver{
    private:
        int steps=0;
        vector<State> visitedList;
        bool found = false;
    public:
        DisjunctiveSolver():Solver(){}
        void getMove(State*, Goal*, Action action) override {}
        void getMove(State*, vector<Goal>, Action);
        priority_queue<Action>generateLegalMoves(State, Goal, Action);
        int getHeuristicValues(State, Goal, int, int);
        bool isVisitedState(State);
};

void DisjunctiveSolver::getMove(State* state,  vector<Goal> goals, Action action){
    State s = *state;

    for(int i=0; i<goals.size(); i++){

        while(steps < 100){
            if(s.isGoalState(goals[i])){
                goals[i].assignSteps(steps);
                found = true;
                steps = 0;
                break;
            }

            priority_queue<Action> legalMoves;
            steps++;
            legalMoves = generateLegalMoves(s, goals[i], action);
            visitedList.push_back(s);

            if(!legalMoves.empty()){
                State temp(s);
                Action frontOfQueue = legalMoves.top();
                temp.moveBlock(frontOfQueue);
                cout << "\nStep:" << steps << " --> [" << temp.getValue() << "] from column " << frontOfQueue.source << " to " << frontOfQueue.destination << " with heuristic value " << frontOfQueue.heuristic << endl;
                temp.printBoard();
                s = temp;
            }else{
                cout << "No more legal moves without repetition.\n";
                break;
            }
        }

         if(steps == 100){
            goals[i].assignSteps(steps);
            cout << "Goal (" << goals[i].getGoalBlock() << ", " << goals[i].getGoalRow() << ", " << goals[i].getGoalColumn() << ") could not be found, maximum steps were reached. " << endl;
            steps=0;
         }

         if(found){
            break;
         }

    }

    cout << "==============================ENDGAME INFO==============================" << endl;
    for(int i=0; i<goals.size(); i++){
        if(goals[i].getSteps() < 100){
            cout << "Goal (" << goals[i].getGoalBlock() << ", " << goals[i].getGoalRow() << ", " << goals[i].getGoalColumn() << ") has been achieved at step " << goals[i].getSteps() << endl;
            break;
        }
    }

}

std::priority_queue<Action, std::vector<Action>> DisjunctiveSolver::generateLegalMoves(State s, Goal goal, Action action) {
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

int DisjunctiveSolver::getHeuristicValues(State temp, Goal goal, int source, int destination){
    int h=0;

    if(temp.isGoalState(goal)){
        return 100;
    }

    if(isVisitedState(temp)){
        return 0;
    }

    if(temp.is_block_on_top_of_a_column(goal.getGoalBlock())){
        h=h+50;
    }

    if(temp.is_goal_position_ready(goal.getReversedRow(), goal.getGoalColumn())){
        h=h+40;
    }

    if(temp.remove_block_on_top_of_block(goal.getGoalBlock(), source)){
        h=h+30;
    }

    if(temp.remove_block_on_top_of_goal_position(goal.getReversedRow(),goal.getGoalColumn(), source)){
        h=h+20;
    }

    if(temp.is_block_under_goal_position_needed(goal.getReversedRow(),goal.getGoalColumn(), destination)){
        h=h+10;
    }
    return h;
}

bool DisjunctiveSolver::isVisitedState(State s){
    for(int i=0; i<visitedList.size(); i++){
        if(s==visitedList[i]){
            return true;
        }
    }
    return false;
}

