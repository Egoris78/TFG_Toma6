#pragma once
#include <list>
#include "Hand.hpp"
class AI{
private:
	int numFilas;
	int numPlayers;
public:
	AI() {};
	AI(int NUM_PLAYERS, int NUM_FILAS) { numPlayers = NUM_PLAYERS; numFilas = NUM_FILAS; }
	int swapFila(list<Card> filas[4]);
	int smallestRowValue(list<Card> filas[4]);
	int playSmallest(Hand myHand, list<Card> filas[4]);
	int playCard(Hand, list<Card> filas[4]);

};