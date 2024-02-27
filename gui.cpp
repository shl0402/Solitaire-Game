#include"card.h"
#include "checkInput.h"
#include"gui.h"
#include <vector>
#include <string>
#include <ncurses.h>

using namespace std;

// Function drawCardTop will draw the top part of the card
// ╭─────╮
// │7   ♦│ for 7 of Diamond
// Input: Card &card, the card to draw
//        WINDOW * &window, the window to draw
//        int y, the start position of y
// Output: Void, but the corresponding window will be updated with the card drawn
void drawCardTop(Card &card, WINDOW * &window, int y){
    // set solor if card red
    if (card.suit%2==0 && card.shown)
        wattron(window, COLOR_PAIR(1));
    // draw the top part of the card
    // if the card is not shown, draw the line only
    
    mvwprintw(window, y, 0, "╭─────╮");
    if(!card.shown){
        mvwprintw(window, y+1, 0, "│     │");
    }
    // if the card is shown, draw the rank and suit
    else{
        if (card.rank == 10){
            // get the unicode of suit from SUIT array defined in gui.h
            mvwprintw(window, y+1, 0, "│10  %lc│", SUIT[card.suit]);  
        }
        else{
            // get the ascii of rank and unicode of suit from RANK and SUIT array defined in gui.h
            mvwprintw(window, y+1, 0, "│%c   %lc│", RANK[card.rank], SUIT[card.suit]);
        }
    }
    // reset color
    wattroff(window, COLOR_PAIR(1));
}



// drawCardBottom function will draw the bottom part of the card, for the card is not under another card
// It will look like this
// │     │
// │     │
// ╰─────╯
// Input: WINDOW * &window, the window to draw
//        int y, the start position of y
// Output: Void, but the corresponding window will be updated with the card drawn
void drawCardBottom(Card &card, WINDOW * &window, int y){
    // set solor if card red
    if (card.suit%2==0 && card.shown)
        wattron(window, COLOR_PAIR(1));
    // draw the rest part of the card
    mvwprintw(window, y, 0, "│     │");
    mvwprintw(window, y+1, 0, "│     │");
    mvwprintw(window, y+2, 0, "╰─────╯");
    // reset color
    wattroff(window, COLOR_PAIR(1));

}


// updateStock function will draw the stock deck
// Clear the window, pass the stock deck table[7] and the window of stock deck to drawCardTop and drawCardBottom, and update the window
// The stock deck window will look like this
// ╭────╮
// │    │
// │    │
// ╰────╯
// ╭────╮
// │6  ♠│
// ╭────╮
// │2  ♠│
// ╭────╮
// │9  ♠│
// │    │
// ╰────╯
// Input: vector<vector<Card> > &table, the stock deck is table[7]
//        WINDOW * &window, the window to draw
// Output: Void, but the corresponding window (WINDOW * window) of stock deck will be updated with the stock deck drawn
void updateStock(vector<vector<Card> > &table, WINDOW * &window){
    // draw the stock deck
    wclear(window);
    int y = 0;
    Card card = {0,Card::Suit(1),false};
    drawCardTop(card, window, y);
    drawCardBottom(card, window, y+2);
    y += 5;
    for (int i = 0; i < table[7].size(); ++i){
        if (table[7][i].shown == true) {
            drawCardTop(table[7][i], window, y);
            card = table[7][i];
            y += 2;
        }
    }
    if (y>5){
        drawCardBottom(card, window, y);
    }
    // refresh the stock window
    wrefresh(window);
}


// dateStack function will draw the stack deck
// Clear the window, Pass the table[8], and the window to draw to drawCardTop and drawCardBottom, and update the window
// The window will look like this
// ╭────╮
// │   ♦│
// │    │
// ╰────╯
// ╭────╮
// │   ♣│
// │    │
// ╰────╯
// ╭────╮
// │   ♥│
// │    │
// ╰────╯
// ╭────╮
// │   ♠│
// │    │
// ╰────╯
// Input: vector<vector<Card> > &table, the stack deck is table[8]
//        WINDOW * &window, the window to draw
// Output: Void, but the corresponding window (WINDOW * window) of stack deck will be updated with the stack deck drawn
void updateStack(vector<vector<Card> > &table, WINDOW * &window){
    // draw the stack deck
    wclear(window);
    int y = 0;
    for (int i = 0; i < 4; ++i){
        drawCardTop(table[8][i], window, y);
        drawCardBottom(table[8][i], window, y+2);
        y += 5;
    }
    // refresh the stack window
    wrefresh(window);
}


// updateColumn function will draw the column deck
// Clear the window, Pass the table[column], and the window to draw to drawCardTop and drawCardBottom, and update the window
// The window will look like this
// ╭────╮
// │    │
// ╭────╮
// │    │
// ╭────╮
// │9  ♠│
// │    │
// ╰────╯
// Input: vector<vector<Card> > &table, the column deck is table[column]
//        WINDOW * &window, the window to draw
//        int column, indicate which column to draw
// Output: Void, but the corresponding window (WINDOW * window) of column deck will be updated with the column deck drawn
void updateColumn(vector<vector<Card> > &table, WINDOW * &window, int column){
    // draw the column deck
    wclear(window);
    int y = 0;
    Card card = {0,Card::Suit(0),false};
    for (int i = 0; i < table[column].size(); ++i){
        drawCardTop(table[column][i], window, y);
        y += 2;
    }
    // draw the rest part of the card
    if (table[column].size() != 0)
        drawCardBottom(table[column].back(), window, y);
    // refresh the column window
    wrefresh(window);
}


// updateTopStatus function will clear and then draw the Top status window, i.e. Move: 0      Score: 0
// Input: WINDOW * &window, the window to draw
//        Ptr &ptr, struct Ptr defined in card.h, which contains in game information with members: int move, int score, int column, int row, int target, int next
//                  ptr.move and ptr.score will be used here
// Output: Void, but the corresponding window (WINDOW * window) of top status window will be updated with the value of ptr.move and ptr.score
void updateTopStatus(WINDOW * &window, Ptr &ptr){
    // draw the top status window
    // draw the move and score
    wclear(window);
    mvwprintw(window, 0, 0, "Move: %d", ptr.move);
    mvwprintw(window, 0, 20, "Score: %d", ptr.score);
    mvwprintw(window, 0, 55, "SOLITAIRE");
    // refresh the top status window
    wrefresh(window);
}


// updateBottomStatus function will clear and then draw the bottom status window, output a message with string message
// Input: WINDOW * &window, the window to draw
//        string message, the message to output
// Output: Void, but the corresponding window (WINDOW * window) of bottom status window will be updated with the message input
void updateBottomStatus(WINDOW * &window, string message){
    wclear(window);
    // draw the bottom status window
    mvwprintw(window, 0, 0, message.c_str());
    // refresh the bottom status window
    wrefresh(window);
}


// listenInput function will clear the window and listen to the keyboard input (1 - 3 characters), convert in lower case and output a string
// Input: WINDOW * &window, the window to listen
// Output: string, the string of the combined keyboard input (1 - 3 characters)
string listenInput(WINDOW * &window){
    // listen to the keyboard input
    refresh();
    wclear(window);
    char input;
    string temp;
    for (int i = 1; i < sizeof(temp); ++i){
        input = wgetch(window);
        temp += tolower(input);
        wdelch(window);
        switch (i)
        {
            case 1:
                if (temp == "f" || temp == "e" || temp == "r" || temp == "u")
                    return temp;
                break;
            case 2:
                if (convertCard(temp) != -1)
                    return temp;
                else if (temp == "10")
                    temp = "t";
                break;
            case 3:
                return temp;
            default:
                break;
        }
    }
    wrefresh(window);
    return temp;
}