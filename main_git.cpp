//
// main.cpp
// Created by Guillermo Romero Alonso
//

#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <fstream>
using namespace std;

// INIT FUNCTIONS
int mainMenu ();
bool login ();
bool checkPass (string user, string pass);
void newUser (string user, string pass);


// ROCK-PAPER-SCISSORS-LIZARD-SPOCK FUNCTIONS
void spockRules ();
void playSpock (int &spockGames, int &wonSpock, int &lostSpock, int &tiesSpock);
void checkOption (string &machine, string &human);
void winnerSpock (string human, string machine, int &wonSpock, int &lostSpock);
void tieSpock (int &tiesSpock);


// THROW THE DICE FUNCTIONS
void diceRules ();
void playDice (int &diceGames, int &wonDice, int &lostDice);
void randRoll (int &count, int &face1, int &face2, int &face3);
void machineTurn (int face1, int face2, int face3, int count, int &diceGames, int &wonDice, int &lostDice);
void bestMove (int &face1, int &face2, int &face3, int &count);
void humanTurn (int face1, int face2, int face3, int count, int &diceGames, int &wonDice, int &lostDice);
void rolls (int &face1, int &face2, int &face3, int &count, char human);
void humanRolls (char &human);
void drawDice (int face1, int face2, int face3, int count);


// FINAL RESULTS FUNCTIONS
void showResults (int wonSpock, int lostSpock, int tiesSpock, int spockGames, int ganadasDado, int perdidasDado, int diceGames);


// Main function that shows the interface
int main (){
    int option, wonSpock = 0, lostSpock = 0, tiesSpock = 0, spockGames = 0, wonDice = 0, lostDice = 0, diceGames = 0;
    
    //system ("chcp 1252");  // Leer caracteres en CASTELLANO.
    srand ((unsigned int)time (NULL));
    
    if (login()){
        cout << "--------------------------------------" << endl;
        cout << "    WELCOME TO THE PORTAL OF GAMES    " << endl;
        cout << "--------------------------------------" << endl;
        
        do {
            option = mainMenu ();
            if (option == 1)
                spockRules ();
            else if (option == 2)
                playSpock (spockGames, wonSpock, lostSpock, tiesSpock);
            else if (option == 3)
                diceRules ();
            else if (option == 4)
                playDice (diceGames, wonDice, lostDice);
            else if (option == 0)
                showResults (wonSpock, lostSpock, tiesSpock, spockGames, wonDice, lostDice, diceGames);
        } while (option != 0);
    }
    
    return 0;
}


// login menu
bool login (){
    string user, pass;
    
    cout << "Username: ";
    cin >> user;
    cout << "Password: ";
    cin >> pass;
    
    // check the password
    if (!checkPass(user, pass)){
        cout << "Sorry, you have triewd many times. Game blocked" << endl;
        cout << endl;
        showResults (0, 0, 0, 0, 0, 0, 0);
        cin.get ();
        return false;
    }
    
    return true;
}


// return whether the password is ok or not
// add a new user if is not in the log
bool checkPass (string user, string pass){
    string line;
    ifstream logRd;
    bool known;
    
    logRd.open ("log.txt");
    if (logRd.is_open ()){
        
        logRd >> line;
        known = false;
        while (!known && line != "XXX"){
            if (line == user){
                known = true;
                
                // check pass
                logRd >> line;
                int i = 2;
                while(i != 0 && line != pass) {
                    cout << "Incorrect password, " << i << " tries left" << endl;
                    cout << "Password: ";
                    cin >> pass;
                    i--;
                }
                
                if (line != pass){
                    cout << "Sorry, you have triewd many times. Game blocked" << endl;
                    cout << "Exiting..." << endl;
                    return false;
                }
            }
            else{
                logRd >> line;
            }
        }
        
        // add a new user
        if (!known){
            logRd.close ();
            newUser (user, pass);
        }
    }
    logRd.close ();
    
    return true;
}


// add a new user to the game
void newUser (string user, string pass){
    string line;
    ifstream logRd, tmpRd;
    ofstream logWr, tmpWr;
    
    logRd.open ("log.txt");
    tmpWr.open ("tmp.txt");
    logRd >> line;
    
    // copy log.txt to temp.txt
    while (line != "XXX"){
        tmpWr << line << endl;
        logRd >> line;
        tmpWr << line << endl;
        logRd >> line;
    }
    tmpWr << "XXX";
    logRd.close ();
    tmpWr.close ();
    
    
    logWr.open ("log.txt");
    tmpRd.open ("tmp.txt");
    
    // copy tmp.txt to log.txt
    if (logWr.is_open () && tmpRd.is_open ()){
        tmpRd >> line;
        while (line != "XXX"){
            logWr << line << endl;
            tmpRd >> line;
        }
        logWr << user << endl;
        logWr << pass << endl;
        logWr << "XXX";
    }
    tmpRd.close ();
    logWr.close ();
}


// Return the chosen option to main()
int mainMenu (){
    int option;
    
    cout << "1. Show Rock-Paper-Scissors-Lizard-Spock Rules" << endl;
    cout << "2. Play Rock-Paper-Scissors-Lizard-Spock" << endl;
    cout << "3. Show Throw the Dice Rules" << endl;
    cout << "4. Play Throw the Dice" << endl;
    cout << "0. Exit" << endl;
    cout << "Option: ";
    cin >> option;
    cout << endl;
    
    return option;
}


// Show Rock-Paper-Scissors-Lizard-Spock Rules
void spockRules (){
    string line;
    ifstream rules;
    
    rules.open ("spockRules.txt");
    if (rules.is_open ()){
        do{
            getline (rules, line);
            if (line != "XXX")
                cout << line << endl;
        } while (line != "XXX");
    }
    cout << endl;
    rules.close ();
}


// Starts Rock-Paper-Scissors-Lizard-Spock game
void playSpock (int &spockGames, int &wonSpock, int &lostSpock, int &tiesSpock){
    
    typedef enum {stone, paper, scissors, lizard, spock} tElemento;
    string machine;
    int seed;
    tElemento elem;
    
    seed = rand() % 5;
    elem = tElemento (seed);
    
    // machine chooses randomly
    switch (seed){
        case stone: machine = "stone";
            break;
        case paper: machine = "paper";
            break;
        case scissors: machine = "scissors";
            break;
        case lizard: machine = "lizard";
            break;
        case spock: machine = "spock";
            break;
    }
    
    cout << " ------------------------------------- " << endl;
    cout << " |             WELCOME TO            | " << endl;
    cout << " |  ROCK-PAPER-SCISSOR-LIZARD-SPOCK  | " << endl;
    cout << " ------------------------------------- " << endl;
    
    
    string human;
    
    // checks human option and computes the winner
    checkOption (machine, human);
    if (machine != human){
        winnerSpock (human, machine, wonSpock, lostSpock);
    }
    else{
        tieSpock (tiesSpock);
        tiesSpock++;
    }
    spockGames++;
}


// check if the chosen option is valid
void checkOption (string &machine, string &human){
    bool ok = false;
    
    while (!ok){
        cout << "Choose an element (stone, paper, scissors, lizard, spock): ";
        cin >> human;
        
        if (human == "stone" || human == "paper" || human == "scissors" || human == "lizard" || human == "spock"){
            cout << "Machine option: " << machine << endl;
            cout << "Human option: " << human << endl;
            ok = true;
        }
        else {
            cout << "Unknown option. Try again" << endl;
        }
    }
}


// check whether the machine or the human win
void winnerSpock (string human, string machine, int &wonSpock, int &lostSpock){
    
    // human wins
    if ((human == "stone" && (machine == "scissors" || machine == "lizard"))
        || (human == "paper" && (machine == "stone" || machine == "spock"))
        || (human == "scissors" && (machine == "paper" || machine == "lizard"))
        || (human == "lizard" && (machine == "paper" || machine == "spock"))
        || (human == "spock" && (machine == "stone" || machine == "scissors"))){
        cout << endl;
        cout << "........." << endl;
        cout << endl;
        cout << "You win! :)" << endl;
        wonSpock++;
        cout << endl;
    }
    // human looses
    else{
        cout << endl;
        cout << "........." << endl;
        cout << endl;
        cout << "I win!" << endl;
        lostSpock++;
        cout << endl;
    }
}


// there is a tie
void tieSpock (int &tiesSpock){
    cout << endl;
    cout << "........." << endl;
    cout << endl;
    cout << "Empate!" << endl;
    cout << endl;
    tiesSpock++;
}


// show throw the dice rules
void diceRules (){
    ifstream reglas;
    string instrucciones;
    
    reglas.open ("diceRules.txt");
    if (reglas.is_open ()){
        do{
            getline (reglas, instrucciones);
            if (instrucciones != "XXX")
                cout << instrucciones << endl;
        } while (instrucciones != "XXX");
    }
    cout << endl;
    reglas.close ();
}

// Starts Throw the Dice game
void playDice (int &diceGames, int &wonDice, int &lostDice){
    int face1, face2, face3, count = 0;
    
    cout << " -------------------------------- " << endl;
    cout << " |         WELCOME TO           | " << endl;
    cout << " |       THROW THE DICE         | " << endl;
    cout << " -------------------------------- " << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "Game starts! Who's first?..." << endl;
    cout << endl;
    
    // choose randomly who starts
    // human starts
    if (rand()%2 == 0){
        cout << "You start" << endl;
        cout << "Your die roll has been...";
        randRoll (count, face1, face2, face3);
        drawDice (face1, face2, face3, count);
        machineTurn (face1, face2, face3, count, diceGames, wonDice, lostDice);
    }
    // machine starts
    else {
        cout << "I start" << endl;
        cout << "My die roll has been...";
        randRoll (count, face1, face2, face3);
        drawDice (face1, face2, face3, count);
        humanTurn (face1, face2, face3, count, diceGames, wonDice, lostDice);
    }
}

// the first dice throw of the game is random
void randRoll (int &count, int &face1, int &face2, int &face3){
    int face = rand () % 6;
    
    switch (face){
        case 0:
            face1 = 1;
            face2 = 2;
            face3 = 4;
            break;
        case 1:
            face1 = 2;
            face2 = 6;
            face3 = 4;
            break;
        case 2:
            face1 = 3;
            face2 = 6;
            face3 = 2;
            break;
        case 3:
            face1 = 4;
            face2 = 1;
            face3 = 2;
            break;
        case 4:
            face1 = 5;
            face2 = 1;
            face3 = 4;
            break;
        case 5:
            face1 = 6;
            face2 = 5;
            face3 = 4;
            break;
    }
    count = face1;
    
}


// machine plays
void machineTurn (int face1, int face2, int face3, int count, int &diceGames, int &wonDice, int &lostDice){
    char human = '9';
    
    while (human != '0' && count < 50){
        bestMove (face1, face2, face3, count);
        
        if (count >= 50){
            cout << "YOU WIN!" << endl;
            cout << endl;
            wonDice++;
            diceGames++;
        }
        
        if (count < 50){
            cout << "It is your turn" << endl;
            cout << endl;
            humanRolls (human);
            
            if (count >= 50){
                cout << "YOU LOOSE!" << endl;
                cout << endl;
                lostDice++;
                diceGames++;
            }
        }
    }
}


// human plays
void humanTurn (int face1, int face2, int face3, int count, int &diceGames, int &wonDice, int &lostDice){
    char human = '9';
    
    while (human != '0' && count < 50){
        humanRolls (human);
        rolls (face1, face2, face3, count, human);
        
        if (count >= 50){
            cout << "YOU LOOSE!" << endl;
            cout << endl;
            lostDice++;
            diceGames++;
        }
        if (count < 50){
            bestMove (face1, face2, face3, count);
            if (count >= 50){
                cout << "YOU WIN!" << endl;
                cout << endl;
                wonDice++;
                diceGames++;
            }
        }
    }
}


// human chooses the direction where to roll the dice
void humanRolls (char &human){
    bool ok = false;
    
    while (!ok && human != '0'){
        cout << "Choose rolling direction (N, S, E, W)" << endl;
        cout << "(To show rules press 1 and hit Enter. To extit press 0 and hit Enter)" << endl;
        cout << "Option: ";
        cin >> human;
        
        if (human == '1'){
            diceRules ();
        }
        else if (human == 'n' || human == 'N' || human == 's' || human == 'S' || human == 'e' || human == 'E' ||
                 human == 'w' || human == 'W' || human == '0')
            ok = true;
    }
}


// draw the dice (where face1 is the good face)
void drawDice (int face1, int face2, int face3, int count){
    
    cout << endl;
    cout << "    ___ " << endl;
    cout << "  / " << face1 << " /" << "| " << endl;
    cout << "  --" << " |" << face2 << "| " << right << setw (20) << "count = " << count << endl;
    cout << " | " << face3 << " |" << "/ " << endl;
    cout << "  --" << endl;
    cout << endl;
}


// roll dice in different directions
void rolls (int &face1, int &face2, int &face3, int &count, char option){
    int aux2;
    
    if (option == 'N' || option == 'n'){
        aux2 = face1;
        face1 = face3;
        face3 = 7 - aux2;
    }
    else if (option == 'S' || option == 's'){
        aux2 = face3;
        face3 = face1;
        face1 = 7 - aux2;
    }
    else if (option == 'E' || option == 'e'){
        aux2 = face2;
        face2 = face1;
        face1 = 7 - aux2;
    }
    else if (option == 'W' || option == 'w'){
        aux2 = face1;
        face1 = face2;
        face2 = 7 - aux2;
    }
    
    count = count + face1;
    drawDice (face1, face2, face3, count);
}


// the machine choose the movement so the puntuation is maxima
void bestMove (int &face1, int &face2, int &face3, int &count){
    string machine;
    char option = ' ';
    
    if ((face1 == 1 && face2 == 3 && face3 == 2) || (face1 == 4 && face2 == 5 && face3 == 1) || (face1 == 5 && face2 == 3 && face3 == 1)
        || (face1 == 2 && face2 == 4 && face3 == 1) || (face1 == 3 && face2 == 2 && face3 == 1) || (face1 == 6 && face2 == 4 && face3 == 2)){
        machine = "North";
        option = 'N';
    }
    else if ((face1 == 1 && face2 == 4 && face3 == 5) || (face1 == 4 && face2 == 6 && face3 == 5) || (face1 == 5 && face2 == 4 && face3 == 6)
             || (face1 == 2 && face2 == 3 && face3 == 6) || (face1 == 3 && face2 == 5 && face3 == 6) || (face1 == 6 && face2 == 3 && face3 == 5)
             || (face1 == 4 && face2 == 2 && face3 == 6)){
        machine = "South";
        option = 'S';
    }
    else if ((face1 == 2 && face2 == 6 && face3 == 4) || (face1 == 3 && face2 == 6 && face3 == 2) || (face1 == 6 && face2 == 5 && face3 == 4)
             || (face1 == 1 && face2 == 5 && face3 == 3) || (face1 == 4 && face2 == 6 && face3 == 5) || (face1 == 5 && face2 == 6 && face3 == 3)){
        machine = "East";
        option = 'E';
    }
    else if ((face1 == 2 && face2 == 1 && face3 == 3) || (face1 == 3 && face2 == 1 && face3 == 5) || (face1 == 6 && face2 == 2 && face3 == 3)
             || (face1 == 1 && face2 == 2 && face3 == 4) || (face1 == 4 && face2 == 1 && face3 == 2) || (face1 == 5 && face2 == 1 && face3 == 4)){
        machine = "West";
        option = 'W';
    }
    
    cout << "It is my turn" << endl;
    cout << "My dice roll has been " << machine << endl;
    rolls (face1, face2, face3, count, option);
}


// show games ranks
void showResults (int wonSpock, int lostSpock, int tiesSpock, int spockGames, int wonDice, int lostDice, int diceGames){
    
    cout << endl;
    cout << "Total 'Rock-Paper-Scissors-Lizard-Spock' games played " << spockGames << endl;
    cout << "Total won games: " << wonSpock << endl;
    cout << "Total tied games " << tiesSpock << endl;
    cout << "Total lost games: " << lostSpock << endl;
    cout << endl;
    cout << "Total 'Throw the Dice' games played: " << diceGames << endl;
    cout << "Total won games: " << wonDice << endl;
    cout << "Total lost games: " << lostDice << endl;
    cout << endl;
    cout << "See you!" << endl;
}
