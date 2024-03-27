#include "AI.hpp"

int AI::playSmallest(Hand myHand, list<Card> filas[4])
{
    list<Card> hand = myHand.getHand();
    int actualLowstValue = 130, smallest = -1,i=1;
    for (auto card : hand) {
        if (card.getNum() < actualLowstValue) {
            actualLowstValue = card.getNum();
            smallest = i;
        }
        i++;
    }
    return smallest;
}

int AI::smallestRowValue(list<Card> filas[4])
{
    int smallest = 100,best = -1;
    for (int fila = 0; fila < numFilas; fila++) {
        int points = 0;
        for (auto carta : filas[fila]) {
            points += carta.getValue();
        }
        if (smallest > points) {
            best = fila;
            smallest = points;
        }
    }
	return best;
}

int AI::swapFila(list<Card> filas[4])
{
    return smallestRowValue(filas);
}

int AI::playCard(Hand myHand, list<Card> filas[4])
{
    switch (type)
    {
    case 1:
        return playSmallest(myHand, filas);
        break;
    case 2:
    default:
        break;
    }
	return playSmallest(myHand, filas);
}
