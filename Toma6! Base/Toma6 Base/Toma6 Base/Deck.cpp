#include "Deck.hpp"
#include <time.h>

Deck::Deck()
{
	for (int i = 0; i < NUM_CARTAS; i++) {
		int value = CalculateValueCard(i);
        deck[i]=Card(i+1, value);
	}
}

int Deck::CalculateValueCard(int i)
{
    int value = 1;
    value += (i % 10 == 5) ? 1 : 0;
    value += (i % 10 == 0) ? 2 : 0;
    value += (i % 11 == 0) ? 4 : 0;
    value += (i == 55) ? 1 : 0;
    return value;
}

Card Deck::robarCarta()
{
    Card carta = deck[rand() % NUM_CARTAS];
    while (carta.getUsed())
        carta = deck[rand() % NUM_CARTAS];
    deck[carta.getNum()-1].setUsed(true);
    return carta;
}
