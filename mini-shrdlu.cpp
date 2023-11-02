#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstdlib>

using namespace std;

#include "State.h"
#include "Game.h"

void playAgain();

int main(){
    srand(time(0));
    Game g;
    g.play();
    playAgain();

    return 0;
}

void playAgain(){
    cout << endl << "Would you like to play again? [y][n]";
    char playAgain;
    cin >> playAgain;

    if(playAgain == 'y' || playAgain == 'Y'){
        main();
    }
}