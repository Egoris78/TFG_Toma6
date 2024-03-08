#include <iostream>
#include "Deck.hpp"
#include "Player.hpp"

/*
Right Click on title bar of your running console application
Select Properties
Select Layout
Then set the window size.
*/
using namespace std;

static const int NUM_PLAYERS = 4;
static const int NUM_FILAS = 4;


Deck deck;
Player players[NUM_PLAYERS];
list<Card> filas[NUM_FILAS];

void pritnListOfCards(list<Card> cards) {
    string Display;
    for (int i = 0; i < cards.size(); i++)
    {
        //top part
        Display.append(" ------ ");
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
        Display.append("   |");
        Display.append(" ");
    }Display.append("\n");
    for (int i = 0; i < cards.size(); i++)
    {
        //top part
        Display.append("|      |");
        Display.append(" ");
    }Display.append("\n");
    for (int i = 0; i < cards.size(); i++)
    {
        //top part
        Display.append("|      |");
        Display.append(" ");
    }Display.append("\n");
    for (auto card : cards)
    {
        Display.append("|     ");
        Display.append(to_string(card.getValue()));
        Display.append("|");
        Display.append(" ");
    }Display.append("\n");
    for (int i = 0; i < cards.size(); i++)
    {
        //top part
        Display.append(" ------ ");
        Display.append(" ");
    }Display.append("\n");
    cout << Display << endl;
}

void printHand(Hand hand) {
    list<Card> cards = hand.getHand();
    pritnListOfCards(cards);
    cout << endl;
}
void printFilas() {
    for (int i = 0; i < NUM_FILAS; i++)
    {
        pritnListOfCards(filas[i]);
    }
    
}

int main()
{
    srand(time(NULL));
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        Player player = Player();
        //repartir Cartas 
        for (int y = 0; y < 10; y++)
        {
            player.addCardToPlayer(deck.robarCarta());
        }
        cout << "Introduce tu nombre Jugador " << i << " : ";
        string name ="Jorge";
        //Cargar nombres
        //cin >> name;
        cout << endl;
        player.setPlayerName(name);
        players[i] = player;
    }
    //inizializar filas 
    for (int i = 0; i < NUM_FILAS; i++)
    {
        filas[i].push_back(deck.robarCarta());
    }
    system("CLS");
    //Hand hand = players[1].getPlayersHand();
    cout << "Players turn :" << players[1].getPlayerName() << endl;
    cout << "##########################################--Game--##########################################" << endl;
    printFilas();
    cout << "##########################################--"<< players[1].getPlayerName() <<" cards--##########################################" << endl;
    printHand(players[1].getPlayersHand());

    //cout << deck.robarCarta().getNum();

}



// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
