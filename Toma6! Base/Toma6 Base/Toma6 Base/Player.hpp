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
private:
	Hand playersHand;
	int points;
	string playerName;
};