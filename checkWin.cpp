//check if the game is won or there are no more available moves

/*checkWin.cpp
1. checkWin
2. moveAvailable
*/

#include "card.h"
#include "checkWin.h"
#include <vector>

using namespace std;

//Function to check if the game is won
bool checkWin(vector<vector<Card>> &table) {
//if the top cards of all stacks (column 8) are K, the game is won
  for (int i = 0; i < 4; ++i) {
    if (table[8][i].rank != 13) {
      return false;
    }
  }
  //if all cards are shown in columns (i.e. the bottom cards of all column are shown) and there are no cards in stock
  for (int i = 0; i < 7; ++i) {
    if (table[i][0].shown == false) {
      return false;
    }
  }
  return true;
}


//Function to check if there are no more available moves
bool moveAvailable(vector<vector<Card>> &table) {
  
  //if there are unshown cards in the stock, cards can be drawn (i.e. moves are available)
  if (table[7].size() > 0) {
    return true;
  }
  
  //check if cards can be stacked
  else {
    
    //sort all movable cards
    vector<Card> columnCards, topCards, stackCards;
    Card stockCard;
    
    //sort columnCards
    for (int i = 0; i < 8; i++) {

      //skip if the column is empty
      if (table[i].size() > 0) {
        continue;
      }
      
      //identify movable column cards
      for (int i = 0; i < 7; i++) {
        for (int j = 0; j < table[i].size(); j++) {
          //identify the shown cards in the columns
          if (table[i][j].shown) {
            columnCards.push_back(table[i][j]); 
          }
        }
        //identify the top card of each column
        topCards.push_back(table[i][table[i].size() - 1]);           
      }
      
      //identify the top card of each stack
      for (int i = 0; i < 4; i++) {
        stackCards.push_back(table[7][i]);
      }
      
      //identify the most recently drawn card from the stock
      stockCard = table[8][0];
    }
    
  
    /*1. column to column
    any shown card in the columns can be moved to a top card of another column
    if the cards have opposite colors and the rank of the shown card is less than the rank of the top card by 1*/
    for (int i = 0; i < columnCards.size(); i++) {
      for (int j = 0; j < topCards.size(); j++) {
        if (((columnCards[i].suit +1)%2 == (topCards[j].suit)%2) && (columnCards[i].rank == topCards[j].rank - 1)) {
          return true;
        }
      }
    }
    
    /*2. column to stack
    a top card of a column can be moved to the stack
    if the cards have opposite colors and the rank of the top card is greater than the rank of the stack card by 1*/
    for (int i = 0; i < topCards.size(); i++) {
      for (int j = 0; j < stackCards.size(); j++) {
        if (topCards[i].rank == 1) {
          return true;
        }
        else if (((topCards[i].suit +1)%2 == (stackCards[j].suit)%2) && (topCards[i].rank == stackCards[j].rank + 1)) {
          return true;
        }
      }
    }
    
    /*3. stock to column
    the most recently drawn card from the stock can be moved to the top card of a column
    if the cards have opposite colors and the rank of the shown card is less than the rank of the top card by 1*/
    for (int i = 0; i < topCards.size(); i++) {
      if (((stockCard.suit +1)%2 == (topCards[i].suit)%2) && (stockCard.rank == topCards[i].rank - 1)) {
          return true;
      }
    }
    
    /*4. stock to stack
    the most recently drawn card from the stock can be moved to the stack
    if the cards have opposite colors and the rank of the drawn card is less than the rank of the stack card by 1*/
    for (int i = 0; i < stackCards.size(); i++) {
      if (((stockCard.suit +1)%2 == (stackCards[i].suit)%2) && (stockCard.rank == stackCards[i].rank - 1)) {
          return true;
      }
    }
    
    /*5. stack to column
    a card in the stack can be moved to the top card of a column
    if the cards have opposite colors and the rank of the shown card is less than the rank of the top card by 1*/
    for (int i = 0; i < stackCards.size(); i++) {
      for (int j = 0; j < topCards.size(); j++) {
        if (((stackCards[i].suit +1)%2 == (topCards[j].suit)%2) && (stackCards[i].rank == topCards[j].rank - 1)) {
          return true;
        }
      }
    }
  }
  return false;
}
      
