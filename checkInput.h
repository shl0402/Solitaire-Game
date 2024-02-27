#ifndef CHECKINPUT_H
#define CHECKINPUT_H

#include "card.h"
#include "checkInput.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/*  Function to check if the input is valid and pass it to corresponding function */
int checkValid(vector<vector<Card>> &table, vector<CardMap> &cardMap, Ptr &p, string input);
/*  Function to find target card in the stack */
void findStack (vector<vector<Card>> &table, Ptr &p);
/*  Function to find target card in the columns */
void findTarget (vector<vector<Card>> &table, Ptr &p);
/*  Function to convert struct card to integer */
int convertCard(string input);

#endif