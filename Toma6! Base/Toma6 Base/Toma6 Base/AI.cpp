#include "AI.hpp"
#include <algorithm>

int AI::playSmallest(Hand myHand, list<Card> filas[])
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

int AI::playNearest(Hand myHand, list<Card> filas[])
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

float AI::calculaProb(int actual, int ref ,vector<int> playedCards) {
    int cantidad= 0,jugadas = 0 ;
    float prob = 0;
    //Calculamos la cantidad de numeros que hay entre la de la fila y el margen.
    if (ref > actual) {
        cantidad = actual - 1; //{1,2,3 ... actual}
        for (int i = 1; i < cantidad; i++)
        {
            jugadas += (find(playedCards.begin(), playedCards.end(), actual - i) != playedCards.end()) ? 1 : 0;
        }
    }
    else {
        cantidad = actual - ref; // {ref ... actual}
        for (int i = 1; i < cantidad; i++)
        {
            jugadas += (find(playedCards.begin(), playedCards.end(), i + ref) != playedCards.end()) ? 1 : 0;
        }
    }
    //Las cartas ya jugadas que esten en esta franja
    prob = cantidad - jugadas - 1;
    float res = prob / NUM_CARTAS;
    return res;
}

int getRowValue(list<Card> list) {
    int points = 0;
    for (auto carta : list) {
        points += carta.getValue();
    }
    return points;
}

//Revisar si hacer Tree serch o algun tipo de probavilistico

/*
    Ramificacion de Cartas en la mano* numero de filas.
    Por cada fila calculamos la prob de que el resultado sea positivo 
    Primero se tiene en cuenta el numero de cartas de la Fila,
    tambien el numero de Jugadores, en caso de que el numero de jugadores 
    sea igual al numero de cartas restantes para llenar la fila
    se calcula la probavilidad de la mejor carta para jugar en esa fila 
    En el caso de que no se cumpla la primera condicion , esa fila sera 
    segura para jugar.

    En el caso en que tengamos una carta menor a cualquiera de las de las filas.
    Se tendra que guardar la carta hasta que haya un valor total de la fila menor al, de otras jugadas.

*/

int AI::playMontecarlo(Hand myHand, list<Card> filas[], vector<int> playedCards)
{
    //ponderations
    float playSafe = 0.2, discardSmallest = 0.1;
    //
    int card = 1 ,bestCard = 0;
    float actual = 100 ,bestActual = 100, best = 100;
    list<Card> hand = myHand.getHand();
    for (auto cartaPlayer : hand) {
        for (int i = 0; i < numFilas; i++) {
            int filaNum = filas[i].back().getNum();
            float prob = calculaProb(cartaPlayer.getNum(), filaNum, playedCards) * (numPlayers-1);
            if (cartaPlayer.getNum() < filaNum) {
                actual = prob * getRowValue(filas[i]) * discardSmallest;
            }
            else {
                if (filas[i].size() + numPlayers >= 6) {
                    actual = prob * getRowValue(filas[i]);
                }
                else
                {
                    actual = prob * getRowValue(filas[i]) *playSafe;
                }
            }
            if (actual < bestActual) {
                bestActual = actual;
            }
        }
        if (bestActual < best) {
            best = bestActual;
            bestCard = card;
        }
        card++;
    }
    return bestCard;
}

int AI::playRand(Hand myHand) {
    int num = rand() % myHand.getHand().size();
    return num +1;
}

int AI::smallestRowValue(list<Card> filas[])
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

int AI::swapFila(list<Card> filas[])
{
    return smallestRowValue(filas);
}

int getReward(int actualPuntuation,int lastPuntuation) {
    int putuacion = 0;
    putuacion += (lastPuntuation == actualPuntuation) ? 1 : 0;
    putuacion += ((actualPuntuation - lastPuntuation) <= 2) ? 1 : 0;
    return putuacion;
}

int AI::TrainQ(Hand myHand, list<Card> filas[], vector<int> playedCards,int actualPuntuation) {
    if (lastState != -1) {
        reward = getReward(actualPuntuation, lastPuntuation);
        qTable[lastState-1][lastPos] += reward;
    }
    int media = 0;
    for (int i = 0; i < 4; i++)
    {
        media += filas[i].back().getNum();
    }
    media = media / 4;
    lastPos = media / (NUM_CARTAS / NUM_PARTITIONS);
    int playedCard = playRand(myHand);
    lastState = myHand.getCard(playedCard).getNum();
    return playedCard;
}

int AI::playQTable(Hand myHand, list<Card> filas[])
{
    int media = 0;
    for (int i = 0; i < 4; i++)
    {
        media += filas[i].back().getNum();
    }
    media = media / 4;
    int pos = media / (NUM_CARTAS / NUM_PARTITIONS);
    list<Card> hand = myHand.getHand();
    int bestCarta = 1,actualCarta = 1,bestPuntuation = 0;

    for (auto cartaPlayer : hand) {
        int puntuacion = TrainedqTable[cartaPlayer.getNum() - 1][pos];
        if (bestPuntuation < puntuacion) {
            bestCarta = actualCarta;
            bestPuntuation = puntuacion;
        }
        actualCarta++;
    }
    return bestCarta;
}

int getRewardCard(int actualPuntuation, int lastPuntuation, bool extra) {
    int putuacion = 0;
    putuacion += (lastPuntuation == actualPuntuation) ? 1 : 0;
    putuacion += (((actualPuntuation - lastPuntuation) <= 2) && extra) ? 2 : 0;
    putuacion += ((actualPuntuation - lastPuntuation) <= 2) ? 1 : 0;
    return putuacion;
}

int AI::TrainQCard(Hand myHand, list<Card> filas[], vector<int> playedCards, int actualPuntuation, int posJugada) {
    for (int i = 0; i < numFilas; i++)
    {
        if (lastState != -1) {
            reward = getRewardCard(actualPuntuation, lastPuntuation, (posJugada == i) ? true : false);
            qTableCard[lastState - 1][lastFilas[i]-1] += reward;
        }
        lastFilas[i] = filas[i].back().getNum();
    }
    int playedCard = playRand(myHand);
    lastState = myHand.getCard(playedCard).getNum();
    return playedCard;
}

int AI::playQTableCard(Hand myHand, list<Card> filas[])
{
    list<Card> hand = myHand.getHand();
    int bestCarta = 1,actualCarta = 1,bestPuntuation = 0;

    for (auto cartaPlayer : hand) {
        /*for (int i = 0; i < numFilas; i++)
        {
            int puntuacion = TrainedqTableCard[cartaPlayer.getNum() - 1][filas[i].back().getNum()-1];
            if (bestPuntuation < puntuacion) {
                bestCarta = actualCarta;
                bestPuntuation = puntuacion;
            }
        }*/
        int media = 0;
        for (int i = 0; i < numFilas; i++)
        {
            media += TrainedqTableCard[cartaPlayer.getNum() - 1][filas[i].back().getNum() - 1];
        }
        media = media / numFilas;
        if (bestPuntuation < media) {
            bestCarta = actualCarta;
            bestPuntuation = media;
        }
        actualCarta++;
    }
    return bestCarta;
}

int getRewardCardCount(int actualPuntuation, int lastPuntuation, bool extra) {
    int putuacion = 0;
    putuacion += (lastPuntuation == actualPuntuation) ? 1 : 0;
    putuacion += (((actualPuntuation - lastPuntuation) <= 2) && extra) ? 2 : 0;
    putuacion += ((actualPuntuation - lastPuntuation) <= 2) ? 1 : 0;
    return putuacion;
}

int AI::TrainQCardCount(Hand myHand, list<Card> filas[], vector<int> playedCards, int actualPuntuation, int posJugada) {
    for (int i = 0; i < numFilas; i++)
    {
        if (lastState != -1) {
            reward = getRewardCardCount(actualPuntuation, lastPuntuation, (posJugada == i) ? true : false);
            qTableCardCount[lastState - 1][lastFilas[i]-1][lastCount[i]-1] += reward;
        }
        lastFilas[i] = filas[i].back().getNum();
        lastCount[i] = filas[i].size();
    }
    int playedCard = playRand(myHand);
    lastState = myHand.getCard(playedCard).getNum();
    return playedCard;
}

int AI::playQTableCardCount(Hand myHand, list<Card> filas[])
{
    list<Card> hand = myHand.getHand();
    int bestCarta = 1,actualCarta = 1,bestPuntuation = 0;

    for (auto cartaPlayer : hand) {
        int media = 0;
        for (int i = 0; i < numFilas; i++)
        {
            media += TrainedqTableCardCount[cartaPlayer.getNum() - 1][filas[i].back().getNum() - 1][filas[i].size()-1];
        }
        media = media / numFilas;
        if (bestPuntuation < media) {
            bestCarta = actualCarta;
            bestPuntuation = media;
        }
        actualCarta++;
    }
    return bestCarta;
}

int AI::playQuimera(Hand myHand, list<Card> filas[])
{
    list<Card> hand = myHand.getHand();
    int bestCarta = 1, actualCarta = 1, bestPuntuation = 0;

    for (auto cartaPlayer : hand) {
        int media = 0;
        for (int i = 0; i < numFilas; i++)
        {
            media += TrainedqTableCardCount[cartaPlayer.getNum() - 1][filas[i].back().getNum() - 1][filas[i].size() - 1] * getRowValue(filas[i]);
        }
        media = media / numFilas;
        if (bestPuntuation < media) {
            bestCarta = actualCarta;
            bestPuntuation = media;
        }
        actualCarta++;
    }
    return bestCarta;
}


int AI::playCard(Hand myHand, list<Card> filas[], vector<int> playedCards, int actualPuntuation, int posJugada)
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
    case 4:
        return playRand(myHand);
        break;
    case 5:
        return TrainQ(myHand, filas, playedCards, actualPuntuation);
        break;
    case 6:
        return playQTable(myHand, filas);
        break;
    case 7:
        return TrainQCard(myHand, filas, playedCards, actualPuntuation,posJugada);
        break;
    case 8:
        return playQTableCard(myHand, filas);
        break;
    case 9:
        return TrainQCardCount(myHand, filas, playedCards, actualPuntuation,posJugada);
        break;
    case 10:
        return playQTableCardCount(myHand, filas);
        break;
    case 11:
        return playQuimera(myHand, filas);
        break;
    default:
        break;
    }
	return playSmallest(myHand, filas);
}
