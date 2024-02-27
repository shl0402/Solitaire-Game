#ifndef CHECKWIN_H
#define CHECKWIN_H

#include <vector>
#include "card.h"

using namespace std;

//Function to check if the game is won
bool checkWin(vector<vector<Card>> &table);
//Function to check if there are no more available moves
bool moveAvailable(vector<vector<Card>> &table);

#endif
