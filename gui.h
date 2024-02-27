#ifndef GUI_H
#define GUI_H

#include<vector>
#include<string>
#include"card.h"
#include<ncurses.h>
#include <menu.h>
#include<map>

using namespace std;

// RANK and SUIT for the convertion of rank and suit to ascii or unicode
static const wchar_t SUIT[4] = {L'\u2666', L'\u2663', L'\u2665', L'\u2660'};
static char RANK[14] = {' ', 'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};

void drawCardTop(Card &card, WINDOW * &window, int y);
void drawCardBottom(WINDOW * &window, int y);
void updateStock(vector<vector<Card> > &table, WINDOW * &window);
void updateStack(vector<vector<Card> > &table, WINDOW * &window);
void updateColumn(vector<vector<Card> > &table, WINDOW * &window, int column);
void updateTopStatus(WINDOW * &window, Ptr &ptr);
void updateBottomStatus(WINDOW * &window, string message);
string listenInput(WINDOW * &window);

// for start.cpp
void printLogo(WINDOW * &window);
void printBackground(WINDOW * &window);
int printMenu(WINDOW * &window);
void printLeaderboard(WINDOW * &window);
void printAbout(WINDOW * &window);
int printDifficulty(WINDOW * &window);
#endif