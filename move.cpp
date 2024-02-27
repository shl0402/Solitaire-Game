#include "card.h"
#include "move.h"
#include <vector>
#include <iostream>


using namespace std;


// Function to flip the stock deck, i.e. table[7], the next card not shown will be shown, if there is more than 3 cards shown, the prev >3 cards will be hidden
// Input: vector<vector<Card> > &table, the cards in the game table
//        Ptr &p, struct Ptr stores the information of the game, defined in card.h, p.next is the index of next card to be shown
// Output: void, but the table[7] will be updated, p.move +1 and p.next +1 (reset to 0 if equals the size of table[7] -1, score will be deducted by 100 if p.score >= 100, else p.score will be set to 0)
void flipStock(vector<vector<Card> > &table, Ptr &p){
    // next is the index of next card to be shown
    if (table[7][p.next].shown == false){
        table[7][p.next].shown = true;
    }
    // if there are more than 3 cards shown in stock
    if (p.next - 3 >= 0){
        table[7][p.next - 3].shown = false;
    }
    // if next is not the last card in stock, ++next
    if (p.next < table[7].size() - 1){
        ++p.next;
    }
    // if next is the last card in stock, reset the stock
    else{
        for (int i = 0; p.next - i >= 0 && i < 3; ++i){
            table[7][p.next - i].shown = false;
        }
        p.next = 0;
        if (p.score < 100){
            p.score = 0;
        }
        else{
            p.score -= 100;
        }
    }
    ++p.move;
}

// Function to move the card(s) from source (stock deck, column, stack piles) to target column or stack
// Input: vector<vector<Card> > &table, the cards in the game table
//        vector<CardMap> &cardMap, defined in card.h, cardMap[52] store the column and row of the card, where index 0 is A of Diamond to index 51 is K of Spade
//        Ptr &p, struct Ptr stores the information of the game, defined in card.h
// Output: void, but the table and cardMap will be updated, and p.score will be updated
void moveCard(vector<vector<Card> > &table, vector<CardMap> &cardMap, Ptr &p){
    // fill the taget column with the card(s) from the source column
    if (p.column == 8){
        // from stack to column
        table[p.target].push_back(table[p.column][p.row]);
        // update the cardMap
        cardMap[table[p.column][p.row].rank-1 + 13 * p.row] = {p.target, static_cast<int>(table[p.target].size()-1)};
        // remove the card from the source column
        table[p.column][p.row].rank -= 1;
        // deduct 30 points
        p.score -= 30;
        if (p.score < 15){
            p.score = 15;
        }
    }

    else if (p.column < 7){
        if (p.target == 8){
            // from column to stack
            table[p.target][table[p.column][p.row].suit].rank += 1;
            cardMap[table[p.column][p.row].rank-1 + 13*table[p.column][p.row].suit] = {p.target, table[p.column][p.row].rank};
            p.score += 15;
        }
        else{ 
            // from column to column
            for (int i = p.row; i < table[p.column].size(); ++i){
                table[p.target].push_back(table[p.column][i]);
                // update the cardMap
                cardMap[table[p.column][i].rank-1 + 13*table[p.column][i].suit] = {p.target, static_cast<int>(table[p.target].size()-1)};
            }
        }
        // remove the card(s) from the source column
        table[p.column].erase(table[p.column].begin() + p.row, table[p.column].end());
        // if the source column is not empty, show the last card
        if (table[p.column].size() > 0){
            if (!table[p.column][p.row - 1].shown){
                table[p.column][p.row - 1].shown = true;
                p.score += 5;
            }
        }
    }
    // fill the target column with the card from the stock
    else{
        if (p.target == 8){
            // from stock to stack
            table[p.target][table[p.column][p.row].suit].rank += 1;
            // update the cardMap
            cardMap[table[p.column][p.row].rank-1 + 13*table[p.column][p.row].suit] = {p.target, table[p.column][p.row].rank};
            p.score += 10;
        }
        else{
            // from stock to column
            table[p.target].push_back(table[p.column][p.row]);
            // update the cardMap
            cardMap[table[p.column][p.row].rank-1 + 13*table[p.column][p.row].suit] = {p.target, static_cast<int>(table[p.target].size()-1)};
            p.score += 5;
        }
        // remove the card from the stock
        table[p.column].erase(table[p.column].begin() + p.row);
        // move the next pointer to the previous card in stock
        for (int i = p.row; i < table[p.column].size(); ++i){
            cardMap[table[p.column][i].rank-1 + 13*table[p.column][i].suit].row -= 1;
        }
        --p.next;
        // the third previous card in stock is not shown
        if (p.next - 3 >= 0){
            table[p.column][p.row - 3].shown = true;
        }
    }
    ++p.move;
}