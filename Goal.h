#ifndef GOAL_h_
#define GOAL_h_

class Goal{
    private:
        int row=0;
        int column=0;
        int block=0;
        int blockCount=0;
        int boardSize;
        int n=0;
        int reversedRow=0;
        int steps=0;

    public:
        Goal(int size, int count){
            blockCount = count;
            boardSize = size;
        };
        void setGoal();
        bool isValidGoal(int,int,int);
        int getGoalColumn();
        int getGoalRow();
        int getGoalBlock();
        int getReversedRow();

        void assignSteps(int);
        bool isFound();
        int getSteps();
};

void Goal::setGoal(){
    do{
        n=boardSize-1;
        cout << "\nInput a goal in the format (block, row, col): \n";
        cout << "Block (1-" << blockCount << "): ";
        cin >> block;
        cout << "Row (0-" << n << "): ";
        cin >> row;
        cout << "Column (0-" << n << "): ";
        cin >> column;
		cout << endl;
        if(!isValidGoal(block,row,column)){
            cout << "\nOut of range!\n";
        }
    }while(!isValidGoal(block,row,column));
}

bool Goal::isValidGoal(int block, int row, int column){
    if(block <= blockCount && column < boardSize && row < boardSize)
        return true;
    return false;
}

int Goal::getGoalColumn(){
	return column;
}

int Goal::getGoalRow(){
	return row;
}

int Goal::getGoalBlock(){
	return block;
}

int Goal::getReversedRow(){
    reversedRow = (boardSize-1)-row;
    return reversedRow;
}

void Goal::assignSteps(int s){
    steps = s;
}

bool Goal::isFound(){
    if(steps < 100){
        return true;
    }
        return false;
}

int Goal::getSteps(){
    return steps;
}
#endif //GOAL_h_