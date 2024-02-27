/* This file include the function of redo, undo and saveProess*/
#include <iostream>
#include <vector>
#include "card.h" //for Card
#include "redoUndo.h" // singleProcess

bool detectPreviousCommand(string command, string previousCommand){
    if(previousCommand == "u" && (command != "r" && command != "u")){
        //going to delete exceed process
        return true;
    }
    else if(previousCommand == "r" && (command != "r" && command != "u")){
        //going to delete exceed process
        return true;
    }      
    else{
        //not to delete exceed process
        return false;
    }
}
void saveProcess(vector<vector<Card>> &table, Ptr &p, vector<CardMap> &cardMap, vector<singleProcess> &processes){
    singleProcess newProcess;
    newProcess.table = table;
    newProcess.cardMap = cardMap;
    newProcess.ptr = p;
    processes.push_back(newProcess);
}


void deleteProcess(vector<vector<Card>> &table, Ptr &p, vector<CardMap> &cardMap, vector<singleProcess> &processes){
    int oldStep = processes.size() - p.move - 1;
    for(int i=0; i<oldStep; i++){
        processes.pop_back();
    }
    
}


void undo(vector<vector<Card>> &table, Ptr &p, vector<CardMap> &cardMap, vector<singleProcess> &processes){
    int move = p.move - 1;
    table = processes[move].table;
    p = processes[move].ptr;
    cardMap = processes[move].cardMap;
}


void redo(vector<vector<Card>> &table, Ptr &p, vector<CardMap> &cardMap, vector<singleProcess> &processes){
    int move = p.move+1;
    table = processes[move].table;
    p = processes[move].ptr;
    cardMap = processes[move].cardMap;
}