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

int AI::playNearest(Hand myHand, list<Card> filas[4])
{
    int nearest = 1, diff = 100,actual = 1;
    list<Card> hand = myHand.getHand();
    for (auto cartaPlayer : hand) {
        for (int i = 0; i < numFilas; i++)
        {
            int actualDif = cartaPlayer.getNum() - filas[i].back().getNum();
            //Comprueva que el numero sea mas grande que la primera carta y se queda con el de mayor diferencia
            //Tambien chekea el caso en que haya 5 cartas para no robar la fila.
            if (actualDif > 0 && actualDif < diff && filas[i].size()<5) {
                diff = actualDif; 
                nearest = actual;
            }
        }
        actual++;
    }
    return nearest;
}

int AI::playMontecarlo(Hand myHand, list<Card> filas[4], vector<int> playedCards)
{

    return 1;
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

int AI::playCard(Hand myHand, list<Card> filas[4], vector<int> playedCards)
{
    switch (type)
    {
    case 1:
        return playSmallest(myHand, filas);
        break;
    case 2:
        return playNearest(myHand, filas);
        break;
    case 3:
        return playMontecarlo(myHand, filas, playedCards);
        break;
    default:
        break;
    }
	return playSmallest(myHand, filas);
}
