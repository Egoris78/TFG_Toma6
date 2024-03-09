#include <iostream>
#include "Deck.hpp"
#include "Player.hpp"
#include <Windows.h>
#include <chrono>
#include <iostream>
/*
Right Click on title bar of your running console application
Select Properties
Select Layout
Then set the window size.
*/
using namespace std::chrono;

static const int NUM_PLAYERS = 2;
static const int NUM_FILAS = 4;


Deck deck;
Player players[NUM_PLAYERS];
list<Card> filas[NUM_FILAS];
int playersTurn = 0;
bool endGame = false;
list<Card> cardsPlayed;
int rounds = 0;

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
    system("CLS");
    displayTable();
    displayPlayersHand();
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
            int fila = askFila(player);
            players[player].addPoints(getPointsFila(fila));
            changeFila(fila);
        }
        else {
            if (filas[pos].size() >= 5) {
                players[cardsPlayed.front().getPlayerId()].addPoints(getPointsFila(pos));
                changeFila(pos);
            }
            else {
                filas[pos].push_back(cardsPlayed.front());
            }
        }
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
    int winner = -1, wiinnerPoints= players[0].getPoints();
    for (int i = 1; i < NUM_PLAYERS; i++)
    {
        if (players[i].getPoints() < wiinnerPoints) {
            winner = i;
            wiinnerPoints = players[i].getPoints();
        }
    }
    return winner;
}

void nextPlayer() {
    if (playersTurn + 1 >= NUM_PLAYERS) {
        playersTurn = 0;
        setCards();
        endCond();
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
        playedCard = askCardToPlay();

        Card carta;
        carta = players[playersTurn].getCard(playedCard);

        players[playersTurn].playCard(playedCard);
        carta.setPlayerId(playersTurn);
        cardsPlayed.push_back(carta);
    }
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
        filas[i].push_back(deck.robarCarta());
    }
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

int main()
{
    srand(time(NULL));
    //tuto 
    printTuto();
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        Player player = Player();
        cout << "Introduce tu nombre Jugador " << i << " : ";
        string name ="Jorge";
        //Cargar nombres
        cin >> name;
        cout << endl;
        player.setPlayerName(name);
        players[i] = player;
    }
    repartirCartas();
    //inizializar filas 
    setTable();
    system("CLS");
    //bucle game 
    display();
    while (!endGame ) { 
        if (players[0].getPlayersHand().getHand().empty()) {
            deck = Deck();
            repartirCartas();
            setTable();
            cardsPlayed.clear();
        }
        playCard();
        nextPlayer();
    }

    cout << "The winner is " << players[checkWinner()].getPlayerName() << endl;
}