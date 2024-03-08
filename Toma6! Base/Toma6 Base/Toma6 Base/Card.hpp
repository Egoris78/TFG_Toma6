#pragma once
#include <string>
#include <iostream>
#include <string>
using namespace std;

class Card {
private:
	int num;
	int value;
	bool used = false;
public:
	Card() {};
	Card(int num, int value) { this->num = num; this->value = value; }
	int getNum() { return num; }
	int getValue() { return value; }
	bool getUsed() { return used; }
	void setUsed(bool b) { used = b; }
	string getSpriteCarta();
};