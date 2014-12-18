#include<vector>
#include<algorithm>
#include "Card.h"

class Deck
{
	std::vector<Card> mDeck;
	std::vector<Card> mDiscardPile;
	unsigned int mDiscardPileSize;
	unsigned int mDeckSize;
	int withJoker;
public:
	Deck(int joker = 0);
	void reset();
	void formDeck();
	void shuffleDeck();
	std::vector<Card> giveHandtoPlayer(const unsigned int &size);
	Card drawCard();
	void discardCard(const Card &c);
	Card drawFromDiscardPile();
	Card peakIntoDiscardPile();
	void copyFromDiscardToStock();
	unsigned int deckSize();
	unsigned int discardPileSize();
	void showDeck();
};