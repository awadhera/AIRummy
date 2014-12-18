#include "Deck.h"
#include<iostream>
#include<algorithm>
#include<iterator>

Deck::Deck(int joker) : withJoker(joker)
{
	formDeck();
}

void Deck::reset()
{
	mDeck.clear();
	mDiscardPile.clear();
	formDeck();
}

void Deck::formDeck()
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 13; ++j)
		{
			Card c(i, j);
			mDeck.push_back(c);
		}
	}
	if (withJoker != 0)
	{
		Card j1(4, 13);
		mDeck.push_back(j1);
		mDeck.push_back(j1);
	}
}

void Deck::shuffleDeck()
{
	std::random_shuffle(mDeck.begin(), mDeck.end());
}

std::vector<Card> Deck::giveHandtoPlayer(const unsigned int &size)
{
	std::vector<Card> hand(size);
	std::copy(mDeck.end() - size, mDeck.end(), hand.begin());
	mDeck.erase(mDeck.end() - size, mDeck.end());
	return hand;
}

Card Deck::drawCard()
{
	Card topCard = mDeck.back();
	mDeck.pop_back();
	return topCard;
}

unsigned int Deck::deckSize()
{
	return mDeck.size();
}

void Deck::discardCard(const Card &c)
{
	mDiscardPile.push_back(c);
}

Card Deck::peakIntoDiscardPile()
{
	return mDiscardPile.back();
}

Card Deck::drawFromDiscardPile()
{
	Card topCard = mDiscardPile.back();
	mDiscardPile.pop_back();
	return topCard;
}

void Deck::copyFromDiscardToStock()
{
	Card topCard = mDiscardPile.back();
	std::random_shuffle(mDiscardPile.begin(), mDiscardPile.end()-1);
	std::copy(mDiscardPile.begin(), mDiscardPile.end() - 1, back_inserter(mDeck));
	mDiscardPile.clear();
	mDiscardPile.push_back(topCard);
}

unsigned int Deck::discardPileSize()
{
	return mDiscardPile.size();
}

void Deck::showDeck()
{
	std::cout << "Deck is \n";
	for (std::vector<Card>::iterator it = mDeck.begin(); it != mDeck.end(); ++it)
	{
		(*it).displayCard();
	}
}

