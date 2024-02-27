#include <ncurses.h>
#include <vector>
#include <string>
#include <fstream>

void printLeaderboard(WINDOW * &window);
void printAbout(WINDOW * &window);

// Print the logo
void printLogo(WINDOW * &window) {
    // initialize ifstream
    std::ifstream fin;
    // open logo bitmap
    fin.open("bitmap/logo.txt");
    std::string line;
    // print line by line
    int i=0;
    start_color();
    while (getline(fin, line) && i<9){
        mvwprintw(window, i, 0, line.c_str());
        i++;
        }
        wrefresh(window);
    // close the file
    fin.close();
}

// Print background
void printBackground(WINDOW * &window) {
    // initialize ifstream
    std::ifstream fin;
    // open background bitmap
    fin.open("bitmap/background.txt");
    std::string line;
    // print line by line
    int i=0;
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    wattron(window, COLOR_PAIR(1));
    while (getline(fin, line) && i<15){
        mvwprintw(window, i, 0, line.c_str());
        i++;
        }
        wrefresh(window);
    wattroff(window,COLOR_PAIR(1));
    // close the file
    fin.close();
}

// Print the menu
int printMenu(WINDOW * &window) {
    start_color();
	init_pair(2, COLOR_RED, COLOR_WHITE);
    refresh();
    wrefresh(window);
    keypad(window, TRUE);
    // char to store choices
    char *choices[] = {
                        "        Start        ",
                        "        Load         ",
                        "     Leaderboard     ",
                        "        About        ",
                        "        Quit         ",
                  };
    // initializing variables
    int choice;
    int highlight = 0;
    int userInput=0;
    // main menu loop
    while(1) {
        mvprintw(LINES - 2, 1, "ENGG1340 Group 69 Project ");
        refresh();
        for(int i = 0; i < 5; ++i) {
            // highlights the present choice
            if (i == highlight) {
                wattron(window, A_REVERSE);
            }
            // prints the choices
            mvwprintw(window, i*2+1, 3, choices[i]);
            wattroff(window, A_REVERSE);
        }
        // read the choice
        choice = wgetch(window);
        switch(choice) {
            // if up key is pressed
            case KEY_UP:
                highlight--;
                if (highlight == -1) {
                    highlight = 0;
                }
                break;
            // if down key is pressed
            case KEY_DOWN:
                highlight++;
                if (highlight == 5) {
                    highlight = 4;
                }
                break;
            // if other key is pressed
            default:
                break;
        }
        // if enter is pressed
        if (choice == 10) {
            userInput=highlight;
            // do actions based on user input
            if (userInput==0)
                return 0;    
            if (userInput==1) {
                // load
                // clear the screen
                wclear(window);
                // print the background
                return 1;
            }
            else if (userInput==2) {
                // leaderboard
                // clear the screen
                wclear(window);
                // print the menu
                printLeaderboard(window);
            }
            else if (userInput==3) {
                // about
                // clear the screen
                wclear(window);
                // print the menu
                printAbout(window);
            }
            else if (userInput==4) {
                // quit
                return 4;
            }
            // refresh the screen
            wrefresh(window);
        }
    }
    
	endwin();
}

// Print the difficulty menu
int printDifficulty(WINDOW * &window){
    refresh();
    wrefresh(window);
    keypad(window, TRUE);
    // char to store choices
    char *choices[] = {
                        "        Easy         ",
                        "       Medium        ",
                        "        Hard         ",
                        "       Expert        ",
                        "        Back         ",
                  };
    // initializing variables
    int choice;
    int highlight = 0;
    int userInput=0;
    // main menu loop
    while(1) {
        for(int i = 0; i < 5; ++i) {
            // highlights the present choice
            if (i == highlight) {
                wattron(window, A_REVERSE);
            }
            // prints the choices
            mvwprintw(window, i*2+1, 3, choices[i]);
            wattroff(window, A_REVERSE);
        }
        // read the choice
        choice = wgetch(window);
        switch(choice) {
            // move the highlight
            // if up key is pressed
            case KEY_UP:
                highlight--;
                if (highlight == -1) {
                    highlight = 0;
                }
                break;
            // if down key is pressed
            case KEY_DOWN:
                highlight++;
                if (highlight == 5) {
                    highlight = 4;
                }
                break;
            // if other key is pressed
            default:
                break;
        }
        // if enter key is pressed
        if (choice == 10) {
            userInput=highlight;
            break;
        }
    }
    // do actions based on user input
	return userInput;	       
}

// Print the leaderboard
void printLeaderboard(WINDOW * &window) {
    // initialize ifstream
    std::ifstream fin;
    // open leaderboard file
    fin.open("bitmap/leaderboard.txt");
    // initialize line variable
    std::string line;
    // print line by line
    int i=0;
    while (getline(fin, line) && i<15){
        mvwprintw(window, i, 0, line.c_str());
        i++;
        }
        wrefresh(window);
    // print return message and refresh
    mvprintw(LINES - 2, 1, "Press any key to return...");
    refresh();
    // close the file
    fin.close();
    // hold the screen
    wgetch(window);
    // clear the screen
    wclear(window);
    // refresh the screen
    wrefresh(window);
}

// Print the about page
void printAbout(WINDOW * &window) {
    // initialize ifstream
    std::ifstream fin;
    // open about file
    fin.open("bitmap/about.txt");
    // initialize line variable
    std::string line;
    // print line by line
    int i=0;
    while (getline(fin, line) && i<15){
        mvwprintw(window, i, 0, line.c_str());
        i++;
        }
    // print return message and refresh
    mvprintw(LINES - 2, 1, "Press any key to return...");
    refresh();
    // close the file
    fin.close();
    // hold the screen
    wgetch(window);
    // clear the screen
    wclear(window);
    // refresh the screen
    wrefresh(window);
}