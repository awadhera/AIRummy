#include "Card.h"
#include<iostream>
#include<string>

const std::string SUIT[5] = {"C", "D", "H", "S", "R" };
const std::string FACE[14] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "R" };


Card::Card()
{
}

Card::Card(const int &s, const int &f) : suit(s), face(f)
{
}

void Card::displayCard()
{
	std::string cardValue = FACE[face] + SUIT[suit];
	std::cout << cardValue;
}

bool Card::operator == (const Card& c)
{
	if (suit == c.suit && face == c.face)
		return true;
	else
		return false;

}

int Card::getSuit()
{
	return suit;
}

int Card::getFace()
{
	return face;
}

bool compare1(Card i, Card j)
{
	if (i.suit == j.suit)
		return(i.face < j.face);
	else
		return(i.suit < j.suit);
}

bool compare2(Card i, Card j)
{
	if (i.face == j.face)
		return(i.suit < j.suit);
	else
		return(i.face < j.face);
}

bool compare3(Card i, Card j)
{
	return(i.face == j.face && i.suit && j.suit);
}

