#include "Player.hpp"

void Player::addCardToPlayer(Card card)
{
	playersHand.addCard(card);
}

void Player::playCard(int num)
{
	playersHand.playCard(num);
}

Card Player::getCard(int cardID)
{
	return playersHand.getCard(cardID);
}
