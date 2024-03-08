#include "Card.hpp"

string Card::getSpriteCarta()
{
	char* sprite2;
	string sprite;
	sprite.append( " ------ \n");
	sprite.append("| ");
	sprite.append((num % 10 == 0) ? "0" : "");
	sprite.append(to_string(num) + "   |\n");
	return sprite;
}
