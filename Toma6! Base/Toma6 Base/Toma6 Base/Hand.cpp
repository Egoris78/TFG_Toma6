#include "Hand.hpp"

Card Hand::getCard(int cardID)
{
	auto l_front = hand.begin();
	advance(l_front, cardID-1);
	return *l_front;
}

void Hand::playCard(int num)
{
	auto card = hand.begin();
	advance(card, num-1);
	hand.erase(card);
}
