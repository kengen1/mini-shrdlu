#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "State.h"
#include "Solver.h"
#include "Action.h"
#include "HumanSolver.h"
#include "SimpleSolver.h"
#include "SmartSolver.h"
#include "DisjunctiveSolver.h"
#include "ConjunctiveSolver.h"
#include "AStarSolver.h"
#include "Goal.h"

using namespace std;

class Game {
    private:
        int done=0;
        int gameType;
        bool cont = true;
        vector<Goal> multipleGoals;
        char input;

    public:
        int boardSize;
        int blockCount;
        State *state;
        Goal *goal;
        Solver *solver;
        Action action;
        Game();
        void play();
        int getGameType();
        bool isValid(int);
        int setSize();
        int setBlockCount();
        bool isValidSize(int);
        bool isValidBlockCount(int);
        void goalConfiguration();
};

Game::Game(){
    int gameType = getGameType();

    if(gameType == 5 || gameType == 6){
        boardSize = setSize();
        blockCount = setBlockCount();
    }else{
        boardSize = 3;
        blockCount = 6;
    }

    switch(gameType) {
        case 0:
            solver = new HumanSolver;
            break;
        case 1:
            solver = new SimpleSolver;
            break;
        case 2:
            solver = new SmartSolver;
            break;
        case 3:
            solver = new DisjunctiveSolver;
            break;
        case 4:
            solver = new ConjunctiveSolver;
            break;
        case 5:
            solver = new ConjunctiveSolver;
            break;
        case 6:
            solver = new AStarSolver;
            break;
        case 7:
            break;
    }


    state = new State(boardSize, blockCount);
    goal = new Goal(boardSize, blockCount);
    state->pushDown();

}

void Game::play(){
    state->printBoard();
    goalConfiguration();
    if(gameType == 3 || gameType == 4 || gameType == 5){
        solver->getMove(state, multipleGoals, action);
    }
    solver->getMove(state,goal,action);
}

int Game::getGameType(){

        cout << "         MINI-SHRDLU       " << endl;
    do{
        cout << "=================================" << endl;
        cout << "Choose solver method:" << endl;
        cout << "[0]    Human solver "<< endl;
        cout << "[1]    Single goal solver without heuristics "<< endl;
        cout << "[2]    Single goal solver with heuristics  " << endl;
        cout << "[3]    Disjunctive goal solver " << endl;
        cout << "[4]    Conjunctive goal solver"<< endl;
        cout << "[5]    Conjunctive goal solver on n*n board"<< endl;
        cout << "[6]    A* solver on n*n board"<< endl;
        cout << "[7]    Quit"<< endl;

        cin >> gameType;
    }while(!isValid(gameType));

    return gameType;
}

bool Game::isValid(int gameType){
    if(gameType >=0 && gameType <= 7)
        return true;
    else{
        cout << "Invalid input, please try again." << endl;
        return false;
    }
}

int Game::setSize(){
    do{
        cout << "Input the size of the board (size limit of 3-15) - -" << endl;
        cin >> boardSize;
    }while(!isValidSize(boardSize));
    return boardSize;
}

int Game::setBlockCount(){
    do{
        cout << "Input the number of blocks (block limit of n^2 -n) - - " << endl;
        cin >> blockCount;
    }while(!isValidBlockCount(blockCount));
    return blockCount;
}

bool Game::isValidSize(int boardSize){
    if(boardSize >= 3 && boardSize <= 15){
        return true;
    }else{
        cout << "Invalid board size, please try again" << endl;
        return false;
    }
}

bool Game::isValidBlockCount(int blockCount){
    if(blockCount <= (boardSize*boardSize)-boardSize){
        return true;
    }else{
        cout << "Invalid block count, please try again" << endl;
        return false;
    }
}

void Game::goalConfiguration(){
    if(gameType == 3 || gameType == 4 || gameType == 5){
        while(cont && multipleGoals.size() < 3){
            goal->setGoal();
            Goal g = *goal;
            multipleGoals.push_back(g);
            cout << "Goal   (" << g.getGoalBlock() << ", " << g.getGoalRow() << ", " << g.getGoalColumn() << ") added.\n";
            cout << "Press g to input more goals, Press s(or other letters to start search).\n";
            cin >> input;
            if(input == 'g'){
                continue;
            }else if(input != 'g'){
                cont = false;
            }
        }
    }else{
        goal->setGoal();
    }
}