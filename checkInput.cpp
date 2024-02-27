/*  checkInput.cpp
    1.  checkValid
    2.  findTarget
    3.  findStack
    4.  convertCard
*/

#include "card.h"
#include "checkInput.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/*  Function to check if the input is valid and pass it to corresponding function */
int checkValid(vector<vector<Card>> &table, vector<CardMap> &cardMap, Ptr &p, string input){
    int card, valid = -1;
    if (input.substr(0, 2) == "10"){
        input = 't' + input.substr(2); 
    }
    switch (input.length()){
        case 1:
            // if input == 'f', flip the stock
            if (input[0] == 'f'){
                // valid = 1 means flip deck
                valid = 1;
            }
            else if (input[0] == 'r'){
                // if input[0] == r
                //valid = 4 means redo
                valid = 4;
            }
            else if (input[0] == 'u'){
                //if input[0] == u
                //valid = 5 means undo
                valid = 5;
            }
            else if (input[0] == 'e'){
                // if input[0] == e
                // valid = 6 means exit
                valid = 6;
            }
            break;
        case 2:
            // convert the string card to integer
            card = convertCard(input);
            // if the card is valid
            if (card != -1){
                // if the card is in stack
                if (cardMap[card].column == 8){
                    
                    if (table[8][card/13].rank == card%13 + 1){
                        p.column = 8;
                        p.row = card/13;
                        // valid = 2 means move card to column
                        valid = 2;
                    }
                    else
                        valid = -1;
                }
                else{
                    // if the card is in column
                    p.column = cardMap[card].column;
                    p.row = cardMap[card].row;
                    // valid = 2 means move card to column
                    if (table[p.column][p.row].shown)
                        valid = 2;
                    else
                        valid = -1;
                }
            }   
            break;
        case 3:
            if (input == "sav")
                valid = 7;
            if (input[0] != 'm')
                break; 
            // if input[0] == m, move card to stack
            input = input.substr(1, 2);
            card = convertCard(input);
            // if the card is valid
            if (card != -1){
                p.column = cardMap[card].column;
                p.row = cardMap[card].row;
                // valid = 3 means move card to stack
                if (table[p.column][p.row].shown){
                    valid = 3;
                }else{
                    valid = -1;
                }
            }
            break;
        default:
            break;
    }
    return valid;
}

/* Function to find the stack */
void findStack (vector<vector<Card>> &table, Ptr &p){
    p.target = -1;
    // if the rank is one greater than card in stack with the same suit, set target to the stack
    if (table[8][table[p.column][p.row].suit].rank == table[p.column][p.row].rank - 1){
        p.target = 8;
    }
}

/* Function to find the target column */
void findTarget (vector<vector<Card>> &table, Ptr &p){
    p.target = -1;
    for (int i = 0; i < 7 && p.target == -1; ++i){
        if (i == p.column){
            continue;
        }
        // search for the column that is not empty
        if (table[i].size() > 0){
            // if the card is opposite color and rank is one less than the last card in the target column, set target to the column
            if (((table[i][table[i].size() - 1].suit +1)%2 == (table[p.column][p.row].suit)%2) && (table[i][table[i].size() - 1].rank == table[p.column][p.row].rank + 1)){
                p.target = i;
            }
        }
        //  else check if the card is king, set target to the column
        else{
            if (table[p.column][p.row].rank == 13){
                p.target = i;
            }
        }
    }
    // if target is still -1, check if it can be moved to the stack
    if (p.target == -1){
        findStack(table, p);
    }
}

/* Function to convert card to integer */
int convertCard (string card){
    int rank = -1;
    int suit = -1;
    switch (tolower(card[0])){
        case 'a':
            rank = 1;
            break;
        case '2':
            rank = 2;
            break;
        case '3':
            rank = 3;
            break;
        case '4':
            rank = 4;
            break;
        case '5':
            rank = 5;
            break;
        case '6':
            rank = 6;
            break;
        case '7':
            rank = 7;
            break;
        case '8':
            rank = 8;
            break;
        case '9':
            rank = 9;
            break;
        case 't':
            rank = 10;
            break;
        case 'j':
            rank = 11;
            break;
        case 'q':
            rank = 12;
            break;
        case 'k':
            rank = 13;
            break;
    }
    switch (tolower(card[1])){
        case 'd':
            suit = 0;
            break;
        case 'c':
            suit = 1;
            break;
        case 'h':
            suit = 2;
            break;
        case 's':
            suit = 3;
            break;
    }
    if (rank == -1 || suit == -1){
        return -1;
    }
    return rank -1 + suit * 13;
}