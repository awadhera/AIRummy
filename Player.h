#include<vector>
#include "Deck.h"

const int VALUE[14] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 25 };

class Player
{
public:
	std::vector<Card> hand;
	int totalPoints;
	int numMisses;
	bool miss;
	Player();
	void displayHand();
	std::vector<Card> & returnHand();
	void assignHand(std::vector<Card> &input);
	void evaluatePoints();
	void reset();
};