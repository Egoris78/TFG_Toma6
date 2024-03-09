#pragma once
#include "Hand.hpp"
class Player
{
public:
	Player() { points = 0; };
	~Player() {};
	void addCardToPlayer(Card card);
	Hand getPlayersHand() { return playersHand; }
	string getPlayerName() { return playerName; }
	void setPlayerName(string name) { playerName = name; }
	void playCard(int num);
	void addPoints(int puntos) { points += puntos; }
	Card getCard(int cardID);
	int getPoints() { return points; }
private:
	Hand playersHand;
	int points;
	string playerName;
};