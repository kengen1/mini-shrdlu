#include "Solver.h"
#include <queue>
#include <vector>
#include <functional>
#include <cmath>

class AStarSolver : public Solver{
    private:
        priority_queue<State> open_set;
        vector<State> closed_list;
        int g_cost=0;
        int f_cost=0;
        int steps=0;

    public:
        AStarSolver():Solver(){}

        void getMove(State* state, vector<Goal> goals, Action action) override {}
        void getMove(State* state, Goal* goal, Action action);
        vector<State> findPath(State* startingNode, Goal* goal);

        vector<Action> getLegalMoves(State, Goal*);
        priority_queue<State> getNeighbors(State startingNode, State current, Goal* Goal, vector<Action> legalMoves);
        int getCost(State, State, int);
        bool isVisitedState(State);
        bool exists_in_open_set(priority_queue<State> pq, State s);
        priority_queue<State>update_g_cost(priority_queue<State> pq, State s);

};

void AStarSolver::getMove(State* startingNode, Goal* goal, Action action){
    Goal g = *goal;
    vector<State> path = findPath(startingNode, goal);

    cout << "\nStep:" << steps << " --> Initial State "<< endl;
    path[0].printBoard();

    for(int i=1; i<path.size(); i++){
        steps = i;
        int sourceCol, sourceRow=0;
        int destinationCol, destinationRow=0;

        //path[i].parent->get_block_position(goal->getGoalBlock(), sourceRow, sourceCol);
        path[i].get_block_position(goal->getGoalBlock(), destinationRow, destinationCol);
	    cout << "\nStep:" << steps << " --> [" << path[i].getValue() << "] from column " << sourceCol << " to " << destinationCol << " with heuristic value " << path[i].getHeuristic() << " and a f-score of " << path[i].getFCost() << endl;
        path[i].printBoard();
    }

    cout << "==============================ENDGAME INFO==============================" << endl;
    cout << "Goal (" << g.getGoalBlock() << ", " << g.getGoalRow() << ", " << g.getGoalColumn() << ") has been achieved at step " << steps << endl;
}

vector<State> AStarSolver::findPath(State* startingNode, Goal* goal){
    Goal g = *goal;
    State initial_state (*startingNode);
    initial_state.parent = nullptr;

    open_set.push(initial_state);                                       //initialise open_list with initial state

    initial_state.setGCost(0,0);                                        //initialise g_cost for initial state to 0
    initial_state.setHeuristic(g);                                      //initialise heuristic cost for initial state
    initial_state.setFCost();                                           //initialise f_cost for initial state

    while(!open_set.empty()){
        State current = open_set.top();                                 //make current state the top of open_set with the lowest f_score
        open_set.pop();                                                 //pop the node with the lowest f-value from the  open list

        if(current.isGoalState(g)){
            vector<State> path;
            while(current.parent != nullptr){
                path.push_back(current);
                current = *(current.parent);
            }
            path.push_back(initial_state);
            reverse(path.begin(), path.end());
            return path;
        }

        closed_list.push_back(current);                                //add current to closed_set

        vector<Action>legalMoves = getLegalMoves(current,goal);
        priority_queue<State> neighbors = getNeighbors(initial_state, current, goal, legalMoves);

        while(!neighbors.empty()){
            State neighbor = neighbors.top();
            neighbors.pop();

            if(isVisitedState(neighbor)){
                continue;
            }

            //int tentative_g_cost = getCost(initial_state, current, neighbor.getGCost());
            int tentative_g_cost = getCost(initial_state, current, g.getGoalBlock());
            neighbor.setGCost(tentative_g_cost, steps);
            neighbor.setHeuristic(g);
            neighbor.setFCost();
            neighbor.setParent(new State(current));

            if(tentative_g_cost < neighbor.getGCost()){
                open_set = update_g_cost(open_set, neighbor);
                if(!exists_in_open_set(open_set, neighbor)){
                    continue;
                }
            }
            open_set.push(neighbor);

        }
    }

    return vector<State>();
}

vector<Action>AStarSolver::getLegalMoves(State s, Goal* goal){
    vector<Action> legalMoves;
    for(int fromCol=0; fromCol < s.getSize(); fromCol++){
        for(int toCol=0; toCol < s.getSize(); toCol++){
            State temp = s;

            Action a(fromCol,toCol);
        if(temp.isValidMove(fromCol, toCol)){
            if(temp.moveBlock(a)){
                //temp.printBoard();
                legalMoves.push_back(a);
            }
        }
        }
    }
    return legalMoves;
}

int AStarSolver::getCost(State curr, State neighbour, int goalBlock){
    int x1,y1,x2,y2;

    curr.get_block_position(goalBlock,x1,y1);
    neighbour.get_block_position(goalBlock,x2,y2);

	int euclidean_distance = sqrt((x2 - x1)^2 + (y2 - y1)^2);
    //int manhattan_distance = abs(x1 - x2) + abs(y1 - y2);
    return euclidean_distance;
}

bool AStarSolver::isVisitedState(State s){
    for(int i=0; i<closed_list.size(); i++){
        if(s==closed_list[i]){
            return true;
        }
    }
    return false;
}

bool AStarSolver::exists_in_open_set(priority_queue<State> pq, State s){
    priority_queue<State> tempPQ = pq;
    while(!tempPQ.empty()){
        State currState = tempPQ.top();
        if(currState == s){
            return true;
        }
        tempPQ.pop();
    }
    return false;
}

std::priority_queue<State, std::vector<State>> AStarSolver::update_g_cost(priority_queue<State> pq, State neighbour){
    priority_queue<State> existingNeighbours = pq;
    priority_queue<State> newPQ;

    while(!existingNeighbours.empty()){
        State existingNeighbour = existingNeighbours.top();
        if(existingNeighbour == neighbour){
            if(existingNeighbour.getGCost() < neighbour.getGCost()){
                existingNeighbour.setGCost(neighbour.getGCost(),0);
                existingNeighbour.parent = neighbour.parent;
            }

        }
        existingNeighbours.pop();
        newPQ.push(existingNeighbour);
    }
    return newPQ;
}

std::priority_queue<State, std::vector<State>> AStarSolver::getNeighbors(State startingNode, State current, Goal* goal, vector<Action> legalMoves){
    State temp = current;
    priority_queue<State> pq;

    while(!legalMoves.empty()){
        if(temp.moveBlock(legalMoves.back())){
            pq.push(temp);
            legalMoves.pop_back();
            temp = current;
        }
    }

    return pq;
}