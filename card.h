#ifndef CARD_H
#define CARD_H

struct Card{
    // define rank, suit, shown status of each card
    int rank; //1 for A, 11 for J, 12 for Q, 13 for K
    enum Suit {Diamond, Club, Heart, Spade}; //Spade, Club, Heart, Diamond is the winnable deck order
    Suit suit;  //0 for Diamond, 1 for Club, 2 for Heart, 3 for Spade
    bool shown; //true for shown, false for not shown
};

struct CardMap{
    // define the map of each card, i.e. {column, row} = {2,3}
    int column; //0-6 for the 7 columns, 7 for the stock, 8 for the stack
    int row;
};

struct Ptr{
    // define the pointer to the table
    int column; //0-6 for the 7 columns, 7 for the stock, 8 for the stack
    int row;    //0-n for the n cards in each column, 0-23 for the 24 cards in the stock, 0-3 for the 4 cards in the stack
    int target; //target column, from 0 to 6, 8 for the stack
    int next;   //the index of next card will be shown in the stack deck, i.e. tabe[7][next]
    int move;   //the number of move in the game
    int score;  //the score of the game
    // define the default constructor
    Ptr(){
        column = 0;
        row = 0;
        target = -1;
        next = 0;
        move = 0;
        score = 0;
    };
    // define the constructor with input
    Ptr(int n, int m, int s){
        column = 0;
        row = 0;
        target = -1;
        next = n;
        move = m;
        score = s;
    };
};

#endif
