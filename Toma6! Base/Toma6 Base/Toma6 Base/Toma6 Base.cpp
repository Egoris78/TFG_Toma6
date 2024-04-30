#include <iostream>
#include "Deck.hpp"
#include "Player.hpp"
#include "AI.hpp"
#include <Windows.h>
#include <chrono>
#include <iostream>
#include <fstream>
/*
Right Click on title bar of your running console application
Select Properties
Select Layout
Then set the window size.
*/
using namespace std::chrono;
static const int NUM_CARTAS = 104;
static const int NUM_PLAYERS = 2;
static const int NUM_FILAS = 4;
static const int NUM_PARTITIONS = 6;

//tester part
bool saveTrain = false;
bool test = true;
bool savingData = false;
int savePlayerNum = 1; // 0...
vector<int> IAPLAYERS{ 6,2 };
int num_games = 100;
std::ofstream outputFile("data/dataset.txt");
//Tester variables
vector<vector<int>>  qTable;
int winnerCount[NUM_PLAYERS];
int winnerRoundCount[NUM_PLAYERS];
int actualRoundPoints[NUM_PLAYERS];

Deck deck;
Player players[NUM_PLAYERS];
list<Card> filas[NUM_FILAS];
int playersTurn = 0;
bool endGame = false;
list<Card> cardsPlayed;
int rounds = 0;
vector<int> allPlayedCards;

void pritnListOfCards(list<Card> cards,bool hand) {
    string Display;
    for (int i = 0; i < cards.size(); i++)
    {
        //top part
        Display.append(" ----- ");
        Display.append(" ");
    }Display.append("\n");
    //number 
    for (auto card :cards)
    {
        Display.append("|");
        Display.append(to_string(card.getNum()));
        if (card.getNum() / 10 < 1) 
            Display.append(" ");
        if (card.getNum() / 100 < 1) 
            Display.append(" ");
        Display.append("  |");
        Display.append(" ");
    }Display.append("\n");
    //|      |
    for (int i = 0; i < cards.size(); i++)
    {
        //top part
        Display.append("|     |");
        Display.append(" ");
    }Display.append("\n");
    //|      |
    for (int i = 0; i < cards.size(); i++)
    {
        //top part
        Display.append("|     |");
        Display.append(" ");
    }Display.append("\n");
    //value
    for (auto card : cards)
    {
        Display.append("|    ");
        Display.append(to_string(card.getValue()));
        Display.append("|");
        Display.append(" ");
    }Display.append("\n");
    //end
    for (int i = 0; i < cards.size(); i++)
    {
        //top part
        Display.append(" ----- ");
        Display.append(" ");
    }Display.append("\n");
    if (hand) {
        for (int i = 0; i < cards.size(); i++) {
            Display.append(" ( ");
            Display.append(to_string(i+1));
            Display.append(" )  ");
        }
    }
    cout << Display << endl;
}

void printHand(Hand hand) {
    list<Card> cards = hand.getHand();
    pritnListOfCards(cards,true);
    cout << endl;
}

void printFilas() {
    for (int i = 0; i < NUM_FILAS; i++)
    {
        cout << "Fila: " << i+1 << endl;
        pritnListOfCards(filas[i],false);
    }
}

void displayTable(){
    cout << "Players turn :" << players[playersTurn].getPlayerName() << endl;
    cout << "##########################################--Game--##########################################" << endl;
    printFilas();
}

void displayPlayersHand() {
    cout << "##########################################--" << players[playersTurn].getPlayerName() << " cards--##########################################" << endl;
    cout << endl;
    cout <<"Points: " << players[playersTurn].getPoints() << endl;
    cout << "Select a card to play" << endl;
    printHand(players[playersTurn].getPlayersHand());
}

void display() {
    if (!test) {
        system("CLS");
        displayTable();
        displayPlayersHand();
    }
}

int nearValue(int cardNum) {
    int num = -1 , prox = 100;
    for (int i = 0; i< NUM_FILAS;i++) {
        if (filas[i].back().getNum() < cardNum) {
            int newProx = cardNum - filas[i].back().getNum();
            if (prox > newProx) {
                prox = newProx;
                num = i;
            }
        }
    }
    return num;
}

int getPointsFila(int fila) {
    int points = 0;
    for (auto carta : filas[fila]) {
        points += carta.getValue();
    }
    return points;
}

int askFila(int numPlayer) {
    int number = -1;
    cout << players[numPlayer].getPlayerName();
    cout << " que fila quieres cambiar.";
    while (!(cin >> number) || (number < 0 || number > NUM_FILAS)) {
        cin.clear();
        cin.ignore(256, '\n');
        cout << "Tiene que ser un numero del 1 al " << NUM_FILAS << endl;
    }
    return number-1;
}

int askTypeIa() {
    AI temp =  AI();
    int number = -1;
    for (int i = 0; i < temp.listaT.size(); i++)
    {
        cout << i+1 << ") " << temp.listaT[i] << "   ";
    }
    cout << endl;
    
    while (!(cin >> number) || (number < 0 || number > temp.listaT.size())) {
        cin.clear();
        cin.ignore(256, '\n');
        cout << "Tiene que ser un numero del 1 al " << temp.listaT.size() << endl;
    }
    return number;
}

void changeFila(int fila) {
    filas[fila].clear();
    filas[fila].push_back(cardsPlayed.front());
}

void setCards() {
    cardsPlayed.sort([](Card& c1, Card& c2) {return c1.getNum() < c2.getNum(); });
    int pos = -2;
    while (!cardsPlayed.empty()) {
        pos =nearValue(cardsPlayed.front().getNum());
        if (pos == -1) {
            int player = cardsPlayed.front().getPlayerId();
            int fila;
            if (players[player].isAi()) {
                fila = players[player].Ai->swapFila(filas);
            }
            else {
                fila = askFila(player);
            }
            int points = getPointsFila(fila);
            players[player].addPoints(points);
            actualRoundPoints[player] += points;
            changeFila(fila);
        }
        else {
            if (filas[pos].size() >= 5) {
                int points = getPointsFila(pos);
                players[cardsPlayed.front().getPlayerId()].addPoints(points);
                actualRoundPoints[cardsPlayed.front().getPlayerId()] += points;
                changeFila(pos);
            }
            else {
                filas[pos].push_back(cardsPlayed.front());
            }
        }
        allPlayedCards.push_back(cardsPlayed.front().getNum());
        cardsPlayed.pop_front();
    }
}

void endCond() {
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        if (players[i].getPoints() >= 66) {
            endGame = true;
        }
    }
}

int checkWinner() {
    int winner = 0, wiinnerPoints= players[0].getPoints();
    for (int i = 1; i < NUM_PLAYERS; i++)
    {
        if (players[i].getPoints() < wiinnerPoints) {
            winner = i;
            wiinnerPoints = players[i].getPoints();
        }
    }
    if (savingData) {
        if (outputFile.is_open()) {
            outputFile << ")" << winner << "\n";
        }
        else {
            std::cerr << "Error opening file\n";
        }
    }
    return winner;
}

void repartirCartas() {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        for (int y = 0; y < 10; y++)
        {
            players[i].addCardToPlayer(deck.robarCarta());
        }
    }
}

void setTable() {
    for (int i = 0; i < NUM_FILAS; i++)
    {
        filas[i].clear();
        Card carta = deck.robarCarta();
        filas[i].push_back(carta);
        allPlayedCards.push_back(carta.getNum());
    }
}

void roundWinner() {
    int best = 0,puntuation = actualRoundPoints[0];
    for (int i = 1; i < NUM_PLAYERS; i++)
    {
        if (actualRoundPoints[i] < puntuation) {
            puntuation = actualRoundPoints[1];
            best = i;
        }
    }
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        actualRoundPoints[i] = 0;
    }
    if (savingData) {
        if (outputFile.is_open()) {
            outputFile << ")" << best << "$";
        }
        else {
            std::cerr << "Error opening file\n";
        }
    }
    winnerRoundCount[best]++;
}

void saveCardPlayers() {
    if (savingData) {
        if (outputFile.is_open()) {
            outputFile << players[savePlayerNum].getCard(1).getNum();
            for (int y = 1; y < 10; y++)
            {
                outputFile << "," << players[savePlayerNum].getCard(y+1).getNum();
            }
            outputFile << "/";
        }
        else {
            std::cerr << "Error opening file\n";
        }
    }
}

void restartRound() {
    deck = Deck();
    roundWinner();
    allPlayedCards.clear();
    repartirCartas();
    if (savingData)
        saveCardPlayers();
    setTable();
    cardsPlayed.clear();
}

void nextPlayer() {
    if (playersTurn + 1 >= NUM_PLAYERS) {
        if (savingData) {
            if (outputFile.is_open()) {
                auto l_front = cardsPlayed.begin();
                advance(l_front, savePlayerNum);
                Card carta = *l_front;
                outputFile << carta.getNum() << "," << carta.getValue() << "]";
                for (int i = 0; i < NUM_FILAS; i++)
                {
                    outputFile << filas[i].back().getNum() << "," << filas[i].size() << "_";
                }
                outputFile << "}";
            }
            else {
                std::cerr << "Error opening file\n";
            }
        }
        playersTurn = 0;
        setCards();
        endCond();
        if (players[0].getPlayersHand().getHand().empty()) {
            restartRound();
        }
    }
    else { playersTurn++; }
    display();
}

int askCardToPlay() {
    int number = -1;
    while (!(cin >> number) || (number < 0 || number > players[playersTurn].getPlayersHand().getHand().size())) {
        cin.clear();
        cin.ignore(256, '\n');
        cout << "Tiene que ser un numero del 1 al " << players[playersTurn].getPlayersHand().getHand().size() << endl;
    }
    return number;
}

void playCard() {
    if (players[playersTurn].getPlayersHand().getHand().size() > 0) {
        int playedCard = -1;
        if (players[playersTurn].isAi()) {
            playedCard = players[playersTurn].Ai->playCard(players[playersTurn].getPlayersHand(), filas,allPlayedCards, players[playersTurn].getPoints());
        }
        else {
            playedCard = askCardToPlay();
        }
        Card carta;
        carta = players[playersTurn].getCard(playedCard);
        players[playersTurn].playCard(playedCard);
        carta.setPlayerId(playersTurn);
        cardsPlayed.push_back(carta);
    }
}

bool askAi() {
    char response;
    do
    {
        cout << "Quieres que el jugador se una IA [y / n]" << endl;
        cin >> response;
    } while (!cin.fail() && response != 'y' && response != 'n');
    return (response == 'y') ? true : false;
}

void printTuto() {

    string Title;
    Title.append("%%####################################%###%%%%################################################################%%@(%@&%####################%%%%####%%#%"); Title.append("\n");
    Title.append("%#############################################################################################################%%,,,,,/%&#**##########################*"); Title.append("\n");
    Title.append("%/,(#########################################################################################################&/,,*////,,,*%&&%#######################*"); Title.append("\n");
    Title.append("%/*(#######################################################################################################&%,,,////////*,*&&########################*"); Title.append("\n");
    Title.append("#/,(######################################################################################################&(,,*///////*,,%&################%%&&&&&&%#*"); Title.append("\n");
    Title.append("#/,(####################################################################################################%&*,,////////,,(&%############%&**,,,,*,*#&##*"); Title.append("\n");
    Title.append("#/,(##############################################################%#####%%#############################&%,,*///////,,*&%#%&%###########&*/&&&&%**&%##*"); Title.append("\n");
    Title.append("%/,(####################%&&(,,*%######################%@&&&&&&&&&(,*%&&%*#&%########%&&##&&###########&/,,*//////*,*&&&#*,,*%&%########&*(&&&&/,#&###*"); Title.append("\n");
    Title.append("%/,(####%&&%##########%@%*,/%,,(%############%&%#####%&*,***/*,,,/%(,,,,(,,#&%&&&(*,,,/#(,,*%&%######%%,,*/////*,,//,,,,*//*,,,%&&#####&*(&@@%,*&%###*"); Title.append("\n");
    Title.append("#(,(###%&*,*&&#######&&**#@&&*,*&&&%%###%&&(*,,,(&%##&#**&&&&%%@&&&&%%@&@&(,/&,,(%@&&@&@@&&&/,/&#####%&*,*////*,,,,*//////////*,*@%###%%,#@&&/,#&####*"); Title.append("\n");
    Title.append("#(,(##%&,*%*,,#&%#%##%&/,%&&&#,,*,,*&&(**/&@&@@&&*,#&&/,,,*&&&@&&&&&@@&&&&#,/%,/&&@&&&&@&@&@&,/&%#####&/,,////////////////////*,/&#####%,%&&&**&%####*"); Title.append("\n");
    Title.append("%(,(#&&,*&&@@%,,#&%##%&#,#&&@@&&&*,*&%,(@&&%%@@@&@&#,*&%,,%&&&*,,(&&%,*&&&(,/%,(@&&@(,,*#&@&&,/&%#####&#,,/////////*,,,,*/////,,%&#####%,,,,,,%&#####*"); Title.append("\n");
    Title.append("%(,(&&,**&&@&*,/&####%@&*(@@@&&&@*,*&(,%&&&*,,,,%&@%,*&/,*&&&(,,*&&&(,*&&&(,(#,#@&&&(,,,#&&&%**(&&####%%,,///////*,,,,,,/////*,*&%####&(,,*%*,,&&####*"); Title.append("\n");
    Title.append("%(,(##&&/,,*,,,(&##%##&&**&&@@/,,,,*&**&&&%,,,,*&@&&*,,,,&&&&*,,%&@&*,/&&&/,,**%&&&&/,,#&&@&%/(,*&%###%&,,*////////*,,*//////,,#&###%&/,*&@&&%,,(&%##*"); Title.append("\n");
    Title.append("#(,(####&/,,*((,&%####&@/,%&@&#,#,,,,,/@&@@@&#/&&&@&*,,&&&@&(,,(@&&#,,(&&&&&,,,(&&@@&&@&@@@&@&&(,%&%###&*,*/////////////////*,*&%####&(,,%&&%*,%&%###*"); Title.append("\n");
    Title.append("%(,/####&/,%@@&*(&####%@#,#@@@&,,,%&#*,,*%@&@&&&&%*,,,*((((/,,,,,,,,,,#&&@@%,*&#*,*%&&&%/*@@&%/,,(&&###%&(,,,*////////////,,,*#&######%&/,,,,#@%#####*"); Title.append("\n");
    Title.append("%(,/####&/,%@@@#*&%####&&,/&&@@@&@@@&#,,(*,*%(,,*(&&&&%&&&&&&&&%%%&&**,,,,,,,(&###&(*,,((,,,/&&&%%########%&&/,,,*///*,,,*#&&%##########&%*(@%#######*"); Title.append("\n");
    Title.append("%(,/####&/,#&&@&,(&#####&&(,,%&&@&@&#*,#%##&/%&%#####################################%##&@%###################%@%*,,,*%&%##################%#########*"); Title.append("\n");
    Title.append("%(,/####&(,#@@@@/*&&###%##%&&*,**,,,,*#%#########################################################################%&&%################################*"); Title.append("\n");
    Title.append("%(*/###%&(,(&&@&(,(&#######*.%&&@%%##%#######################################################################################################(###%###*"); Title.append("\n");
    Title.append("#(,/###,&(,,,*(%&&%%#####(.((((((./######%################################################################################################*,////./###*"); Title.append("\n");
    cout << Title << endl;
    string Display;
    Card cartaTuto = Card(16,1);
    //top part
    Display.append(" ----- ");
    Display.append(" ");
    Display.append("\n");
    //number 
    Display.append("|");
    Display.append(to_string(cartaTuto.getNum()));
    if (cartaTuto.getNum() / 10 < 1)
        Display.append(" ");
    if (cartaTuto.getNum() / 100 < 1)
        Display.append(" ");
    Display.append("  |");
    Display.append(" <----- Esto es el Numero de la carta");
    Display.append("\n");
    //|      |
    Display.append("|     |");
    Display.append(" ");
    Display.append("\n");
    //|      |
    Display.append("|     |");
    Display.append(" ");
    Display.append("\n");
    //value
    Display.append("|    ");
    Display.append(to_string(cartaTuto.getValue()));
    Display.append("|");
    Display.append(" <----- Esto es el valor de la carta");
    Display.append("\n");
    //end
    Display.append(" ----- ");
    Display.append(" ");
    Display.append("\n");
    cout << Display << endl;
}

vector<string> spliter(string text, string delimiter) {
    vector<string> content;
    size_t pos = 0;
    string token;
    while ((pos = text.find(delimiter)) != string::npos) {
        token = text.substr(0, pos);
        content.push_back(token);
        text.erase(0, pos + delimiter.length());
    }
    content.push_back(text);
    return content;
}

void read() {
    ifstream MyReadFile("data/dataset.txt");
    string tp;
    vector<vector<bool>> RoundWinners;
    vector < vector<vector<int>>> handsGames;
    vector < vector<vector<Card>>> cardPlayedGames;
    vector < vector<vector<vector<vector<int>>>>> filasPlayedGames;

    while (getline(MyReadFile, tp)) {
        vector<bool> winners;
        vector<string> res;
        vector<vector<int>> hands;
        vector<vector<Card>> cardPlayed;
        vector<vector<vector<vector<int>>>> filasPlayed;
        for (auto round : spliter(tp, "$")) {
            if (round.size() > 0) {
                res = spliter(round, ")");
                winners.push_back(stoi(res.back()) == savePlayerNum);
                res = spliter(res.front(), "/");
                vector<int> actualHand;
                for (auto num : spliter(res.front(), ",")) {
                    actualHand.push_back(stoi(num));
                }
                hands.push_back(actualHand);
                //spliter turn
                vector<string> turns;
                vector<Card> roundCards;
                vector<vector<vector<int>>> filasRound;
                for (auto turn : spliter(res.back(), "}")) {
                    if (turn.size() > 0) {
                        turns = spliter(turn, "]");
                        //Cartas
                        vector<string> temporal = spliter(turns.front(), ",");
                        Card card = Card(stoi(temporal.front()), stoi(temporal.back()));
                        roundCards.push_back(card);
                        //Filas
                        vector<vector<int>> filas;
                        temporal = spliter(turns.back(), "_");
                        for (auto fila : temporal) {
                            if (fila.size() > 0) {
                                vector<int> filaRes;
                                for (auto num : spliter(fila, ",")) {
                                    filaRes.push_back(stoi(num));
                                }
                                filas.push_back(filaRes);
                            }
                        }
                        filasRound.push_back(filas);
                    }
                }
                filasPlayed.push_back(filasRound);
                cardPlayed.push_back(roundCards);
            }  
        }
        filasPlayedGames.push_back(filasPlayed);
        cardPlayedGames.push_back(cardPlayed);
        handsGames.push_back(hands);
        RoundWinners.push_back(winners);
    }
    MyReadFile.close();
}

int main()
{
    srand(time(NULL));
    //tuto 
    printTuto();
        
    int it = 0;
    if (test) {
        for (int  i = 0; i < NUM_PLAYERS; i++)
        {
            winnerCount[i] = 0;
            winnerRoundCount[i] = 0;
            vector<int> particiones;
            for (int i = 0; i < NUM_PARTITIONS; i++)
            {
                particiones.push_back(0);
            }
            for (int i = 0; i < NUM_CARTAS; i++)
            {
                qTable.push_back(particiones);
            }
        }
    }
    do {
        if (test) {
            for (int i = 0; i < IAPLAYERS.size(); i++)
            {
                Player player = Player();
                string name = "boot";
                player.setPlayerName(name);
                AI* ia = new AI(NUM_PLAYERS, NUM_FILAS, IAPLAYERS[i]);
                player.setAi(ia);
                players[i] = player;
            }
        }
        else {
            for (int i = 0; i < NUM_PLAYERS; i++)
            {
                Player player = Player();
                cout << "Jugador " << i + 1 << endl;
                bool IA = askAi();
                string name = "Jorge";
                //Cargar nombres
                if (IA) {
                    name = "boot";
                    int type = askTypeIa();
                    AI* ia = new AI(NUM_PLAYERS, NUM_FILAS, type);
                    player.setAi(ia);
                }
                else {
                    cout << "Introduce tu nombre Jugador " << i + 1 << " : ";
                    cin >> name;
                    cout << endl;
                }
                player.setPlayerName(name);
                players[i] = player;
            }
        }
        allPlayedCards.clear();
        endGame = false;
        deck = Deck();
        cardsPlayed.clear();
        repartirCartas();
        if (savingData)
            saveCardPlayers();
        //inizializar filas 
        setTable();
        if(!test)
            system("CLS");
        //bucle game 
        display();
        while (!endGame) {
            playCard();
            nextPlayer();
        }
        int winner = checkWinner();
        if (test) {
            winnerCount[winner]++;
            system("CLS");
            cout << it << endl;
            if (saveTrain) {
                vector<vector<int>> table = players[1].Ai->getQtable();
                for (int i = 0; i < NUM_CARTAS; i++)
                {
                    for (int y = 0; y < NUM_PARTITIONS; y++)
                    {
                        qTable[i][y] += table[i][y];
                    }
                }
            }
        }
        else {
            cout << "The winner is ";
            if (players[winner].isAi())
                cout << "the player " << winner + 1;
            else
                cout << players[winner].getPlayerName();
            cout << endl;
        }
        it++;
        
    } while (test && it<num_games);
    outputFile.close();
    if (test) {
        system("CLS");
        AI temp = AI();
        cout << "From " << num_games <<" games" << endl;
        for (int i = 0; i < NUM_PLAYERS; i++)
        {
            cout << "The player " << i<< " with Ai ";
            int num = IAPLAYERS[i];
            cout << temp.listaT[num-1];
            cout <<endl<< "    Won " << winnerRoundCount[i] <<" rounds" << endl;
            cout<< "    Won :" << winnerCount[i] << " Games" << endl;
        }
        if (saveTrain) {
            std::ofstream qtable("data/QTable.txt");
            if (qtable.is_open()) {
                for (int i = 0; i < NUM_CARTAS; i++)
                {
                    for (int y = 0; y < NUM_PARTITIONS; y++)
                    {
                        qtable  << qTable[i][y] << ",";
                    }
                    qtable << "]";
                }
            }
            else {
                std::cerr << "Error opening file\n";
            }
        }
    }
    if (savingData) {
        read();
    }
    
}