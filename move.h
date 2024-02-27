#ifndef MOVE_H
#define MOVE_H

#include "card.h"
#include <vector>
#include <iostream>

using namespace std;

void flipStock(vector<vector<Card> > &table, Ptr &p);
void moveCard(vector<vector<Card> > &table, vector<CardMap> &cardMap, Ptr &p);

#endif