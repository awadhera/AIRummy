#include<vector>

class Deck;
class Card
{
	int suit;
	int face;
public:
	Card();
	Card(const int &s, const int &f);
	void displayCard();
	friend bool compare1(Card i, Card j);
	friend bool compare2(Card i, Card j);
	friend bool compare3(Card i, Card j);
	bool operator == (const Card& c);
	int getSuit();
	int getFace();
	friend int analyseHand(std::vector<Card> &hand, Deck &deck, int &winningCond, int joker = 0);
};