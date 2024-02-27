#ifndef INITTABLE_H
#define INITTABLE_H

#include <vector>
#include "card.h"

using namespace std;

void initRandomDeck(Card deck[]);
void initTable(vector<vector<Card>> &table, vector<CardMap> &cardMap, Card deck[]);
void initWinnableDeck(Card deck[], string difficulty);

#endif