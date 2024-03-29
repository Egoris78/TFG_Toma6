#pragma once
#include <list>
#include <vector>
#include "Hand.hpp"
class AI{
private:
	int numFilas;
	int numPlayers;
	int type;
public:
	vector<string> listaT{ "Miedica","Lanzada","MonteCarlo"};
	AI() {};
	AI(int NUM_PLAYERS, int NUM_FILAS, int t) { numPlayers = NUM_PLAYERS; numFilas = NUM_FILAS; type = t; }
	int swapFila(list<Card> filas[4]);
	int smallestRowValue(list<Card> filas[4]);
	int playSmallest(Hand myHand, list<Card> filas[4]);
	int playNearest(Hand myHand, list<Card> filas[4]);
	int playMontecarlo(Hand myHand, list<Card> filas[4], vector<int>);
	int playCard(Hand, list<Card> filas[4],vector<int>);

};