#pragma once
#include "Card.hpp"
#include <cstdlib>
#include <list>


class Hand {
private:
	list<Card> hand;
public:
	Hand() {};
	void addCard(Card card) { hand.push_back(card); }
	void playCard(int num);
	list<Card> getHand() { return hand; }
	Card getCard(int cardID);
 };