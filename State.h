#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <bits/stdc++.h>
#include <random>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

#include "Action.h"
#include "Goal.h"

#ifndef STATE_H_
#define STATE_H_

class State{
    private:
        int boardSize=0;
		int blockCount;
		int value=0;
		int h=0;
		int g_cost = 0;
		int f_cost = 0;
		vector<vector<int>> grid;

    public:
		State* parent=nullptr;

    State(const State& other);
    ~State();
    State& operator=(const State& other);

		bool operator==(State s) {
			for (int i = 0; i < boardSize; i++)
				for (int j = 0; j < boardSize; j++)
					if (s.grid[i][j] != grid[i][j])
						return false;
			return true;
		}
		bool operator<(const State& other) const {
			return f_cost > other.f_cost;											// define the less-than operator for the priority queue
		}

		State(int, int);
		void pushDown();
		void printBoard();
		int removeBlockFrom(int);
		bool insertBlockTo(int,int);
		bool moveBlock(Action a);
		bool isValidMove(int, int);
		int getSize();
		int getValue();
		bool isGoalState(Goal);

		void setHeuristic(Goal);
		int getHeuristic();
		void setGCost(int, int);
		int getGCost();
		void setFCost();
		int getFCost();
		void setParent(State *parent);

		void get_block_position(int, int&, int&);
		bool is_position_empty(int, int);
		bool is_position_filled(int, int);
		bool is_block_on_top_of_a_column(int);
		bool is_goal_position_ready(int, int);
		bool remove_block_on_top_of_block(int, int);
		bool remove_block_on_top_of_goal_position(int, int, int);
		bool is_block_under_goal_position_needed(int,int,int);
		bool is_block_on_top_of_goal_column(int, int);
};

State::State(const State& other) {
    boardSize = other.boardSize;
    blockCount = other.blockCount;
    value = other.value;
    h = other.h;
    g_cost = other.g_cost;
    f_cost = other.f_cost;
    grid = other.grid;
    if (other.parent != nullptr) {
        parent = new State(*(other.parent));
    } else {
        parent = nullptr;
    }
}

State::~State() {
    delete parent;
}

State& State::operator=(const State& other) {
    if (this != &other) {
        boardSize = other.boardSize;
        blockCount = other.blockCount;
        value = other.value;
        h = other.h;
        g_cost = other.g_cost;
        f_cost = other.f_cost;
        grid = other.grid;
        if (other.parent != nullptr) {
            parent = new State(*(other.parent));
        } else {
            parent = nullptr;
        }
    }
    return *this;
}

/*	Functions and parameters for general game functionality */

State::State(int size, int count){
	boardSize = size;
	blockCount = count;

	grid.resize(size, vector<int>(size, 0));	//creating an empty 2D vector of zeros
	vector<int> n (blockCount);

	for(int i=0; i<blockCount; i++){
		n[i] = i+1;
	}

	random_shuffle(n.begin(), n.end());

	for(int i=0; i<blockCount; i++){
		int row = 0;
		int col = 0;

		while(grid[row][col] != 0){
			row = rand()%boardSize;
			col = rand()%boardSize;
		}

		grid[row][col] = n[i];
	}
}

void State::printBoard(){
    for (int i = 0; i < boardSize; i++){
            cout << " ";
            for (int j = 0; j < (boardSize < 4 ? boardSize : 5); j++){
                cout << "-";
            }
    }
    cout << endl;
    for (int i = 0; i < boardSize; i++){
        cout << "|";
        for (int j = 0; j < boardSize; j++){
            string x = " ";
            if (boardSize <= 3){
                cout << setw(boardSize - 1) << (grid[i][j] == 0 ? x : to_string(grid[i][j])) << setw(boardSize - 1) << "|";
            }else{
                cout << setw(3) << (grid[i][j] == 0 ? x : to_string(grid[i][j])) << setw(3) << "|";
            }
        }
            cout << endl;
        }
    for (int i = 0; i < boardSize; i++){
        cout << " ";
        for (int j = 0; j < (boardSize < 4 ? boardSize : 5); j++){
            cout << "-";
        }
    }
    cout << endl;
}

void State::pushDown() {
	for(int col=0; col < boardSize; col++){
		stack<int> nums;
		for(int row=0; row < boardSize; row++){
			if(grid[row][col] != 0){
				nums.push(grid[row][col]);
			}
		}
		int i = boardSize-1;
		while(!nums.empty()){
			grid[i][col] = nums.top();
			nums.pop();
			i--;
		}
		while(i >= 0){
			grid[i][col] = 0;
			i--;
		}
	}
}

int State::removeBlockFrom(int column){
    for(int i=0; i<boardSize; i++){
        int value = grid[i][column];
        if (value !=0){
            grid[i][column]=0;
            return value;
        }
    }
	return -1;
}

bool State::insertBlockTo(int column, int value){
	for(int i = boardSize-1; i >= 0; i--){
		if(grid[i][column] == 0){
			grid[i][column] = value;
			return true;;
		}
	}
	return false;
}

bool State::moveBlock(Action a){
	if(isValidMove(a.source, a.destination)){
		value = removeBlockFrom(a.source);
		if(value == -1){
			return false;
		}
		return insertBlockTo(a.destination, value);
	}

	return false;
}

bool State::isValidMove(int source, int destination){
	if (source==destination){
        return false;
    }

	if(grid[boardSize-1][source] == 0){	//check if source column is empty
		return false;
	}

	if(grid[0][destination] != 0){	//check if destination column is full
		return false;
	}

	return true;
}

int State::getSize(){
	return boardSize;
}

bool State::isGoalState(Goal g){
	queue<int>checkGoalColumn;
	for(int row=0; row<boardSize; row++){
		if(grid[row][g.getGoalColumn()] != 0){
			checkGoalColumn.push(grid[row][g.getGoalColumn()]);
		}
	}
		int s = checkGoalColumn.size()-1;
		if(checkGoalColumn.front() == g.getGoalBlock() && s == g.getGoalRow()){
			return true;
		}
		return false;
}

int State::getValue(){
	return value;
}

/*	Functions and parameters for A* Search */
void State::setHeuristic(Goal g){
	int x1,y1,x2,y2;
	get_block_position(g.getGoalBlock(), x1,y1);
	x2 = g.getReversedRow();
	y2 = g.getGoalColumn();
	//int euclidean_distance = sqrt((x2 - x1)^2 + (y2 - y1)^2);
    int manhattan_distance = abs(x1 - x2) + abs(y1 - y2);
	h = manhattan_distance;
}

int State::getHeuristic(){
	return h;
}

void State::setGCost(int old_g_cost, int dist_between_nodes){
	g_cost = (old_g_cost + dist_between_nodes);
}

int State::getGCost(){
	return g_cost;
}

void State::setFCost(){
	f_cost = g_cost + h;
}

int State::getFCost(){
	return f_cost;
}

void State::setParent(State* parent) {
    if (parent == nullptr || parent == (State*)0xfeeefeeefeeefeee) {
        this->parent = nullptr;
    } else {
        this->parent = parent;
    }
}

/* Functions and parameters for Smart Solver */
void State::get_block_position(int block, int& row, int& column){
	for(int i=0; i<boardSize; i++){
		for(int j=0; j<boardSize; j++){
			if(grid[i][j] == block){
				row=i;
				column=j;
				return;
			}
		}
	}
}

bool State::is_position_empty(int row, int column){
	if(row < 0)
		row++;
	if(grid[row][column]==0)
		return true;
	return false;
}

bool State::is_position_filled(int row, int column){
	if(row < 0)
		row++;
	if(grid[row][column]!=0)
		return true;
	return false;
}

bool State::is_block_on_top_of_a_column(int block){
	for(int i=0; i<boardSize; i++){														//check the top values of every column
		if(block==grid[0][i])															//if the top value of column [i] == block then return true
			return true;
	}
	return false;
}

bool State::is_block_on_top_of_goal_column(int block, int column){
	if(block == grid[0][column])
		return true;
	return false;
}

bool State::is_goal_position_ready(int row, int column){
    if(row>0){
        if(is_position_empty(row, column) && is_position_filled(row-1, column))			//check if the goal position is empty and the block below it is filled so it sits in the correct position
            return true;
    }
    else if(is_position_empty(row, column))												//checking edge case if goal position is row=0
    {
        return true;
    }
    return false;
}

bool State::remove_block_on_top_of_block(int block, int x){
    int row, column;
    if(!is_block_on_top_of_a_column(block)){
        get_block_position(block, row, column);
		if(x==column)
			return true;
	}
    return false;
}

bool State::remove_block_on_top_of_goal_position(int row, int column, int x){
    if(!is_goal_position_ready(row,column)){
        if(x==column)
           return true;
    }
    return false;
}

bool State::is_block_under_goal_position_needed(int row, int column, int destination){
	row = (boardSize-1)-row;
    if(!is_goal_position_ready(row,column)){											//if the goal_position is not ready
        if(is_position_empty(row-1, column))											//if the row below the goal position is empty return true
            return true;
    }
    return false;
}


#endif //STATE_H_