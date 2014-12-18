#include "Player.h"
#include<iostream>

Player::Player()
{
	numMisses = 0;
	miss = false;
	totalPoints = 0;
}

void Player::reset()
{

	hand.clear();
}

void Player::assignHand(std::vector<Card> &input)
{
	hand = input;
}

void Player::displayHand()
{
	for (unsigned int i = 0; i < hand.size(); i++)
	{
		hand[i].displayCard();
		std::cout << " ";
	}
}

std::vector<Card> & Player::returnHand()
{
	return hand;
}

void Player::evaluatePoints()
{
	for (unsigned int i = 0; i < hand.size(); i++)
	{
		totalPoints += VALUE[hand[i].getFace()];
	}
}
