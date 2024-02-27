#include<iostream>
#include<string>
#include<vector>
#include <ncurses.h>
#include <locale.h>
#include "gui.h"
#include "initTable.h"
#include "move.h"
#include "checkInput.h"
#include "card.h"
#include "redoUndo.h"
#include "checkWin.h"
#include "saveLoadFile.h"


using namespace std;

int main(){

    string userName = "My friend";
    string difficulty = "easy";
    
    // ------------------------------------------ GUI ------------------------------------------
    // ---------------------------------initialize the screen-----------------------------------
    // set locale to support unicode
    setlocale(LC_ALL,"");
    // initialize the screen
    initscr();
    // hide the cursor
    curs_set(0);
    // initialize the color
    start_color();
    // initialize the color pair, RED for hearts and diamonds, BLACK for spades and clubs
    init_pair(1, COLOR_RED, -1);
    init_pair(2, COLOR_BLACK, -1);
    // init start screen GUI
    static WINDOW *logo , *menu , *background, *diffMenu;
    // initialize the logo window with 17 row and 95 columns, start at (3,12)
    logo = newwin(9, 95, 8, 12);
    // initialize the background with 15 row and 40 columns, start at (18,40)
    background = newwin(15, 40, 18, 40);
    // initialize difficulty selection
    diffMenu = newwin(15, 26, 20, 47);
    // refresh the screen
    refresh();
    // print the logo
    printLogo(logo);
    // print the background
    printBackground(background);
    // print the menu
    bool load = false;
    while (true) {
        // initialize the menu window with 15 row and 40 columns, start at (20,40)
        menu = newwin(15, 26, 20, 47);
        int status = printMenu(menu);
        // status = 4 means exit
        if (status == 4){
                delwin(menu);
	            endwin();
                exit(0);
        }
        // start new game
        if (status==0){
            refresh();
            int diff= printDifficulty(diffMenu);
            if (diff == 0){
                difficulty = "easy";
                break;
            }
            else if (diff == 1){
                difficulty = "medium";
                break;
            }
            else if (diff == 2){
                difficulty = "hard";
                break;
            }
            else if (diff == 3){
                difficulty = "expert";
                break;
            }
            wclear(diffMenu);
        }
        // load savegame
        else{
            load = true;
            break;        
        }
    }
    refresh();
    noecho();
    wclear(logo);
    wclear(background);
    wclear(menu);
    // endwin();
    

    // store process
    vector<singleProcess> processes;

    // initialize table, cardMap, deck
    vector<vector<Card>> table(9);
    vector<CardMap> cardMap(52);
    Card * deck = new Card[52];
    // Ptr p to store the information of the table
    Ptr ptr;
    bool newAnyway = true;
    cout << table[0].size() << endl;
    if (load){
        // load game
        int loadfail = loadGame(table, ptr, cardMap);
        newAnyway = false;
        if (loadfail) {
            std::cout << "Unable to find saved game! Generating new easy game. Press any key to continue..." << endl;
            difficulty = "easy";
            getch();
            newAnyway = true;
        }
    }
    if (newAnyway){
        // if newgame
        // initialize a winnable deck
        initWinnableDeck(deck, difficulty);
        initTable(table, cardMap, deck);
        // free memory from deck
    }
    // free memory from deck
    delete [] deck;
    deck = nullptr;
    
    // command, prevCommand to store user input, message to store the output string
    string command="000", prevCommand="000", message="Welcome my friend! Please enter command:";
    char yesNo;
    message="Welcome my friend! Please enter command:";
    
    // valid to check if command is valid and pass it to corresponding function
    int valid;
    //save the initial process
    saveProcess(table, ptr, cardMap, processes);

    // ------------------------------------------ GUI ------------------------------------------
    static WINDOW *topStatus, *stock, *stack, *column[7], *bottomStatus, *inputWindow, *commandWindow;
    // initialize the top status window with 1 row and 90 columns, start at (0,0)
    topStatus = newwin(1, 120, 0, 1);
    // initialize the stock window with 36 rows and 10 columns, start at (2,0)
    stock = newwin(20, 13, 2, 12);
    // initialize the stack window with 36 rows and 10 columns, start at (2,80)
    stack = newwin(36, 13, 2, 95);
    // initialize the column window 0-6 with 36 rows and 10 columns each, start at (2,10)
    column[0] = newwin(36, 10, 2, 25);
    column[1] = newwin(36, 10, 2, 35);
    column[2] = newwin(36, 10, 2, 45);
    column[3] = newwin(36, 10, 2, 55);
    column[4] = newwin(36, 10, 2, 65);
    column[5] = newwin(36, 10, 2, 75);
    column[6] = newwin(36, 10, 2, 85);
    // initialize command window
    commandWindow = newwin(11, 22, 26, 2);
    // initialize the bottom status window with 1 row and 90 columns, start at (38,0)
    bottomStatus = newwin(1, 120, 38, 1);
    // initialize the input window with 1 row and 90 columns, start at (39,0)
    inputWindow = newwin(1, 120, 39, 1);
    mvprintw(LINES - 2, 1, "                              ");
    // refresh the windows
    echo();
    refresh();

    // update the top status window
    updateTopStatus(topStatus, ptr);
    // update the stock window
    updateStock(table, stock);
    // update the column window
    for (int i = 0; i < 7; i++){
        updateColumn(table, column[i], i);
    }
    // update the stack window
    updateStack(table, stack);
    // update the bottom status window
    updateBottomStatus(bottomStatus, message);
    // print command window
    // initialize ifstream
    std::ifstream fin;
    // open logo bitmap
    fin.open("bitmap/command.txt");
    std::string line;
    // print line by line
    int i=0;
    while (getline(fin, line) && i<11){
        mvwprintw(commandWindow, i, 0, line.c_str());
        i++;
        }
        wrefresh(commandWindow);
    // close the file
    fin.close();


    // listen to user input
    command = listenInput(inputWindow);

    // ------------------------------------------ game loop ------------------------------------------
    while (true)
    {   
        //get user input
        valid = checkValid(table, cardMap, ptr, command);
        // if the prevCommand is undo or redo, current command is not undo or redo, delete exceed process
        if (valid != -1){
            if (detectPreviousCommand(command, prevCommand)){
                //delete exceed process
                deleteProcess(table,ptr,cardMap,processes); 
            }
        }
        // if command is valid, execute the command
        switch (valid){
            case 1:
                // if valid == 1, flip the stock deck
                flipStock(table, ptr);//ptr.move++
                //save process
                saveProcess(table, ptr, cardMap, processes);
                updateStock(table, stock);
                updateTopStatus(topStatus, ptr);
                break;
            case 2:
                // if valid == 2, move card to column or stack, priority: column > stack
                findTarget(table, ptr);
                if (ptr.target == -1){
                    message = "No possible move! " + userName + ", Please enter command:";
                    updateBottomStatus(bottomStatus, message);
                    break;
                }
                moveCard(table, cardMap, ptr);//ptr.move++
                //save process
                saveProcess(table, ptr, cardMap, processes);
                updateTopStatus(topStatus, ptr);
                if (ptr.target == 8)
                    updateStack(table, stack);
                else
                    updateColumn(table, column[ptr.target], ptr.target);
                if (ptr.column == 7)
                    updateStock(table, stock);
                else if (ptr.column == 8)
                    updateStack(table, stack);
                else
                    updateColumn(table, column[ptr.column], ptr.column);
                break;
            case 3:
                // if valid == 3, move card to stack
                findStack(table, ptr);
                if (ptr.target == -1){
                    message = "No possible move! " + userName + ", Please enter command:";
                    updateBottomStatus(bottomStatus, message);
                    break;
                }
                moveCard(table, cardMap, ptr);//move++
                //save process
                saveProcess(table, ptr, cardMap, processes);
                updateTopStatus(topStatus, ptr);
                if (ptr.target == 8)
                    updateStack(table, stack);
                else
                    updateColumn(table, column[ptr.target], ptr.target);
                if (ptr.column == 7)
                    updateStock(table, stock);
                else
                    updateColumn(table, column[ptr.column], ptr.column);
                break;
            case 4:
                //if valid == 4, redo the process
                if(ptr.move < processes.size()-1){
                    redo(table, ptr, cardMap, processes);
                    updateTopStatus(topStatus, ptr);
                    // update the stock window
                    updateStock(table, stock);
                    // update the column window
                    for (int i = 0; i < 7; i++){
                        updateColumn(table, column[i], i);
                    }
                    // update the stack window
                    updateStack(table, stack);
                    // update the bottom status window
                    message = "Redo successful! " + userName + ", Please enter command:";
                    updateBottomStatus(bottomStatus, message);
                }
                else{
                    message = "No more redo! " + userName + ", Please enter command:";
                    updateBottomStatus(bottomStatus, message);
                }
                break;
            case 5:
                //if valid == 5, undo the process
                if(ptr.move > 0){
                    undo(table, ptr, cardMap, processes);
                    updateTopStatus(topStatus, ptr);
                    // update the stock window
                    updateStock(table, stock);
                    // update the column window
                    for (int i = 0; i < 7; i++){
                        updateColumn(table, column[i], i);
                    }
                    // update the stack window
                    updateStack(table, stack);
                    // update the bottom status window
                    message = "Undo successful! " + userName + ", Please enter command:";
                    updateBottomStatus(bottomStatus, message);
                    break;
                }
                else{
                    message = "No more Undo! " + userName + ", Please enter command:";
                    updateBottomStatus(bottomStatus, message);
                    break;
                }
            case 6:
                //if valid == 6, exit the game
                message = "My friend, " + userName + ", Are you sure you want to exit? (y/n)";
                updateBottomStatus(bottomStatus, message);

                wclear(inputWindow);
                yesNo = wgetch(inputWindow);
                if (tolower(yesNo) == 'y'){
                    //exit the game
                    message = "Bye! " + userName + ", See you next time!";
                    updateBottomStatus(bottomStatus, message);
                    napms(500);
                    delwin(topStatus);
                    delwin(stock);
                    delwin(stack);
                    for (int i = 0; i < 7; i++){
                        delwin(column[i]);
                        delwin(bottomStatus);
                        delwin(inputWindow);
                        endwin();
                        exit(0);
                    }
                }
                else{
                    message = "Welcome back! " + userName + ", Please enter command:";
                    updateBottomStatus(bottomStatus, message);
                }
                break;
            case 7:
                saveGame(table, ptr);
                message = "Game saved!" + userName + ", exit the game? (y/n)";
                updateBottomStatus(bottomStatus, message);
                wclear(inputWindow);
                yesNo = wgetch(inputWindow);
                if (tolower(yesNo) == 'y'){
                    //exit the game
                    message = "Bye! " + userName + ", See you next time!";
                    updateBottomStatus(bottomStatus, message);
                    napms(500);
                    delwin(topStatus);
                    delwin(stock);
                    delwin(stack);
                    for (int i = 0; i < 7; i++){
                        delwin(column[i]);
                        delwin(bottomStatus);
                        delwin(inputWindow);
                        endwin();
                        exit(0);
                    }
                }
                else{
                    message = "Welcome back! " + userName + ", Please enter command:";
                    updateBottomStatus(bottomStatus, message);
                }
                break;
            default:
                // if valid == -1, print invalid input
                message = "Invalid input! " + userName + ", Please enter command:";
                updateBottomStatus(bottomStatus, message);
                break;
        }
        if (checkWin(table)){
            message = "Congratulations! You win! " + userName + ", Your score is: " + to_string(ptr.move) + ", Press any key to exit.";
            updateBottomStatus(bottomStatus, message);
            wclear(inputWindow);
            getch();
            //exit the game
            message = "Bye! " + userName + ", See you next time!";
            updateBottomStatus(bottomStatus, message);
            napms(500);

            // free memory from table
            delwin(topStatus);
            delwin(stock);
            delwin(stack);
            for (int i = 0; i < 7; i++){
                delwin(column[i]);
                }
            delwin(bottomStatus);
            delwin(inputWindow);
            endwin();
            exit(0);
        }
        //save the last command
        prevCommand = command;
        // listen to the user input
        command = listenInput(inputWindow);
    }
    //exit the game
    message = "Bye! " + userName + ", See you next time!";
    updateBottomStatus(bottomStatus, message);
    napms(500);
   
    // free memory from table
    delwin(topStatus);
    delwin(stock);
    delwin(stack);
    for (int i = 0; i < 7; i++){
        delwin(column[i]);
    }
    delwin(bottomStatus);
    delwin(inputWindow);
    endwin();

    return 0;
}