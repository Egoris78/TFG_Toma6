#pragma once
#include "Card.hpp"
#include <list>
using namespace std;

class Deck {
private:
	static const int NUM_CARTAS = 104;
	Card deck [NUM_CARTAS] ;
public:
	Deck();
	int CalculateValueCard(int i);
	Card robarCarta();

};