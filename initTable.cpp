/*  The initiation of table and deck */
#include <iostream>
#include <vector>       // for std::vector
#include <random>       // for std::mt19937
#include <algorithm>    // for std::shuffle
#include "card.h"       // for struct Card, CardMap
#include "initTable.h"  // for initTable
#include <sstream>      // for istringstream
#include <fstream>      // for ifstream

using namespace std;
/*  Function initRandomDeck inititalize a random deck
    Input:  Card deck[52], where Card is a struct defined in Card.h with 3 members: int rank, enum Suit::suit, bool shown
    Output: Void, but Card deck[52] is passed by reference, it will write each array element with a deck of 52 cards with random order
    */
// initialize a random deck
void initRandomDeck(Card deck[]) {

    // initialize a deck of 52 cards, starting from 1 of Diamond to 13 of Diamond, then 1 of Clubs to 13 of Clubs, then to Hearts, then to Spades
    for (int i = 0; i < 52; ++i) {
        deck[i].rank = i % 13 + 1;
        deck[i].suit = Card::Suit(i / 13);
        deck[i].shown = false;
    }
    // create a random device
    random_device rd;
    mt19937 g(rd());
    // shuffle the deck
    shuffle(deck, deck + 52, g);

}
/* Function initWinnableDeck import a winndable deck from winningDeck.txt to Card deck[52] with string difficulty
    different difficulty will select different range of random number to read a line from winningDeck.txt
    
   Input: Card deck[52], deck is an array with type Card where Card is a struct defined in Card.h with 3 members: int rank, enum Suit::suit, bool shown
          string difficulty, with 3 options: "easy", "medium", "hard", this will select 3 ranges of random number to read a line from winningDeck.txt  

   Output: Void, but Card deck[52] is passed by reference, it will write the array element with a deck of 52 cards with random order
    */
// initialize a winnable deck
void initWinnableDeck(Card deck[], string difficulty){
    // open the file "winningDeck.txt"
    ifstream fin;
    fin.open("winningDeck.txt");
    // check if the file is opened successfully
    if (fin.fail()){
        cout << "Error in opening file!\nPlease try again!" << endl;
        exit(1);
    }else{
        // generate a random number to read a line from the file
        int randomLine;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1501, 1871);
        if (difficulty == "easy"){
            uniform_int_distribution<> dis(0, 200);
        }
        else if(difficulty == "medium"){
            uniform_int_distribution<> dis(201, 800);
        }
        else if(difficulty == "hard"){
            uniform_int_distribution<> dis(801, 1500);
        }
        randomLine = dis(gen);
        string line;
        string word;
        int num = 0;
        // read the line
        for (int i = 0; i < randomLine; ++i){
            getline(fin, line);
            // convert the line to a card
            istringstream iss(line);
            string token;
            if( i == randomLine - 1){
                int j = 0;
                while (getline(iss, token, ',')){
                    // fill the card into the deck
                    num = stoi(token)-1;
                    // the code initially change 1 to Ace, 2 to 2, 13 to King, etc.
                    // but the winnableDeck.txt file is using 1 to 2, 2 to 3, 12 to K, 13 to Ace, etc.
                    //Diamond, Club, Heart, Spade(to be changed)
                    if (num / 13 == 0){
                        num += 39;
                    }else if (num / 13 == 3){
                        num -= 39;
                    }
                    deck[j].suit = Card::Suit(num / 13); //0-12, 13-25, 26-38, 39-51 to (Spades, Clubs, Hearts, then Diamonds)
                    num = num+1 % 13;
                    deck[j].rank = num % 13 + 1;
                    deck[j].shown = false;
                    j++;
                }
            }
        }
    }
    fin.close();
}

/*  Function initTable inititalize the game table, table[0] to table[6] with 1 to 7 elements(cards), table[7] with 24 elements, table[8] with 4 elements(cards with 0 rank)
    where Card is a struct defined in Card.h with 3 members: int rank, enum Suit::suit, bool shown

    Input:  vector<vector<Card>> &table, table is a nested vector where Card is a struct defined in Card.h with 3 members: int rank, enum Suit::suit, bool shown
            vector<CardMap> &cardMap, cardMap is a vector where CardMap is a struct defined in Card.h with 2 members: int row, int column
            Card deck[52], where Card is a struct defined in Card.h with 3 members: int rank, enum Suit::suit, bool shown

    Output: Void, but vector<vector<Card>> &table, vector<CardMap> &cardMap, Card deck[] are passed by reference
            vector<vector<Card>> &table, table[0] to table[6] with 1 to 7 elements(cards), table[7] with 24 elements, table[8] with 4 elements(cards with 0 rank)
            vector<CardMap> &cardMap, from index 0-51 (represent A to K, Diamond to Spade) are filled the {column, row} of each card
            Card deck[52] have no change
    */
// initialize the game table
void initTable(vector<vector<Card>> &table, vector<CardMap> &cardMap, Card deck[]){

    // Fill the deck into game table, 7 columns (column 0 - 6), from 1 to 7 cards
    // The last card (size -1) of each 0-6 column is shown
    // fill the remaining cards into the stock (column 7)
    // fill the foundation stack(column 8) with 4 cards with rank 0

    for(int i = 0; i<7; ++i){
        for (int j = i; j < 7; ++j){
            // fill the card into the table, from column 0 to 6, from 1 to 7 cards, the last card is shown
            table[j].push_back(deck[27-(7*i-i*(i+1)/2+j)]);
            // fill the cardMap with the card
            cardMap[table[j].back().rank-1 + table[j].back().suit*13] = {j,i};
            // show the last card
            if (j == i){
                table[i][j].shown = true;
            }
        }
    }

    for (int i = 0; i < 24; ++i) {
        // fill the remaining cards into the stock (column 7)
        table[7].push_back(deck[i+28]);
        // fill the cardMap with {row, column} of the card
        cardMap[deck[i+28].rank-1 + deck[i+28].suit*13] = {7, i};
    }
    // Initialize the stack (column 8) as 4 cards with 0 rank
    for (int i = 0; i < 4; ++i) {
        table[8].push_back({0, Card::Suit(i), true});
    }
}