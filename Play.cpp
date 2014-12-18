#include<iostream>
#include<iterator>
#include<map>
#include "Player.h"

const int HANDSIZE = 7;
const int EXCEED = 101;

extern bool compare1(Card i, Card j);
extern bool compare2(Card i, Card j);
extern bool compare3(Card i, Card j);

/*****************************************************************************************************
analyseHand - This function takes a player hand, and tries to find out if it is a winning hand
consisting of 2 melds required. If not, it continues to find the best possible card to discard
taking into account, the top card in discard pile. It finds the card to discard using the 
following algorithm:

1. Split the hand into 5 groups, consisting of complete sequences, complete groups, partial 
sequences or partial groups

complete sequence examples: AD 2D 3D, 7H 8H 9H 10H
complete group examples: 2S 2D 2H, KC KD KH KS
partial group example : 2S 2D
partial sequence example: AD 2D

2.Find all complete sequences in the hand
3.Find all complete groups from the remaining cards in the hand
4.Find all partial groups. For each partial group starting with the partial group with the 
highest rank, check if a complete sequence exists that uses the rank of the partial group. 
If such a sequence exists, detemine if breaking the sequence will result in a lower value 
of unmatched cards than the value of the two cards in the partial group
5.Find all partial sequences
6.The rest of the cards are deadcards.
7.To determine if the player should draw from the top card in the discard pile or from stock pile:
	7.1. First check if the top card in the discard pile can be added to any partial sequence or 
		 partial group, if so take the card from the discard pile.
	7.2 Else check if the top card in the discard pile can be added to any complete sequence or 
		complete group, if so take the card from the discard pile.
	7.3 Else check if the top card in the discard pile can convert a deadwood card into a partial 
		sequence or a partial group AND there exists more than one deadwood card in the hand, if so
		take the card from the discard pile.
	7.4 Else take the top card from the stock pile.
8.To determine the card to discard:
	8.1 if deadcards exist, select the dead card with the highest face value
	8.2 Else if partial sequences or partial groups exist, select the card from all of the partial 
		sets with the highest face value
	8.3 Else if complete sequences of 4 or more cards exist, select the card from a complete sequence
		with 4 or more cards with the highest face value.
	8.4 Else select a card from a complete group of 4 or more cards. Any one of these cards can be
		chosen.
*******************************************************************************************************/

int analyseHand(std::vector<Card> &hand, Deck &deck, int &winningCond, int joker)
{
	std::vector<Card> newHand;
	std::copy(hand.begin(), hand.end(), back_inserter(newHand));
	std::sort(newHand.begin(), newHand.end(), compare1);
	std::vector<std::vector<Card>> completeSeq;
	std::vector<std::vector<Card>> completeGroup;
	std::vector<std::vector<Card>> partialGroup;
	std::vector<std::vector<Card>> partialSeq;
	std::vector<Card> deadCards;
	std::vector<Card> potential;
	int temp = 0;
	bool jokerFound = false;
	Card cardToDiscard;
	int retJ = 0;
	if (joker == 1)
	{
		for (std::vector<Card>::iterator it = hand.begin(); it != hand.end(); ++it)
		{
			if ((*it).suit == 4 && (*it).face == 13)
			{
				cardToDiscard = *it;
				jokerFound = true;
				retJ = 2;
				break;
			}
		}
	}	
	if (!jokerFound)
	{
		for (std::vector<Card>::iterator it = newHand.begin(); it != newHand.end() - 1; ++it)
		{
			Card curr = *it;
			Card next = *(it + 1);
			if (temp == 0)
				potential.push_back(curr);
			if (curr.suit == next.suit && (curr.face + 1) == next.face)
			{
				potential.push_back(next);
				temp = 1;
			}
			else
			{
				if (potential.size() >= 3)
					completeSeq.push_back(potential);
				potential.clear();
				temp = 0;
			}
		}
		if (potential.size() >= 3 && temp == 1)
		{
			completeSeq.push_back(potential);
			potential.clear();
		}
		temp = 0;
		std::vector<Card> newHand_1;
		std::copy(newHand.begin(), newHand.end(), back_inserter(newHand_1));
		for (std::vector<std::vector<Card>>::iterator it = completeSeq.begin(); it != completeSeq.end(); ++it)
		{
			for (std::vector<Card>::iterator it1 = (*it).begin(); it1 != (*it).end(); ++it1)
			{
				std::vector<Card>::iterator itt = std::find(newHand_1.begin(), newHand_1.end(), *it1);
				if (itt != newHand_1.end())
					newHand_1.erase(itt);
			}
		}
		std::sort(newHand_1.begin(), newHand_1.end(), compare2);
		potential.clear();
		temp = 0;
		if (newHand_1.size() > 1)
		{
			for (std::vector<Card>::iterator it = newHand_1.begin(); it != newHand_1.end() - 1; ++it)
			{
				Card curr = *it;
				Card next = *(it + 1);
				if (temp == 0)
					potential.push_back(curr);
				if (curr.face == next.face)
				{
					potential.push_back(next);
					temp = 1;
				}
				else
				{
					if (potential.size() >= 3)
						completeGroup.push_back(potential);
					else if (potential.size() >= 2 && potential.size() < 3)
						partialGroup.push_back(potential);
					potential.clear();
					temp = 0;
				}
			}
			if (potential.size() >= 3 && temp == 1)
			{
				completeGroup.push_back(potential);
				potential.clear();
			}
			else if (potential.size() >= 2 && potential.size() < 3 && temp == 1)
			{
				partialGroup.push_back(potential);
				potential.clear();
			}
		}
		temp = 0;
		if ((completeSeq.size() == 1 && completeSeq[0].size() == 4 && completeGroup.size() == 1 && completeGroup[0].size() == 3) ||
			(completeGroup.size() == 1 && completeGroup[0].size() == 4 && completeSeq.size() == 1 && completeSeq[0].size() == 3) ||
			(completeSeq.size() == 1 && completeSeq[0].size() == 7) || (completeGroup.size() == 1 && completeGroup[0].size() == 7) ||
			(completeSeq.size() == 2 && completeSeq[0].size() == 3 && completeSeq[1].size() == 4) ||
			(completeSeq.size() == 2 && completeSeq[0].size() == 4 && completeSeq[1].size() == 3) ||
			(completeGroup.size() == 2 && completeGroup[0].size() == 3 && completeGroup[1].size() == 4) ||
			(completeGroup.size() == 2 && completeGroup[0].size() == 4 && completeGroup[1].size() == 3))
		{
			winningCond = 1;
			return 0;
		}
		else
			winningCond = 0;
		std::vector<std::vector<Card>> tempSeq;
		std::reverse(partialGroup.begin(), partialGroup.end());
		std::vector<std::vector<Card>>::iterator it = partialGroup.begin();
		while (it != partialGroup.end())
		{
			int partialGroupInc = 0;
			std::vector<std::vector<Card>>::iterator it1 = completeSeq.begin();
			while (it1 != completeSeq.end())
			{
				int completeSeqInc = 0;
				int i = 0;
				int bssum;
				int pgsum;
				pgsum = VALUE[(*it)[0].face] + VALUE[(*it)[1].face];
				for (std::vector<Card>::iterator it2 = (*it1).begin(); it2 != (*it1).end(); it2++)
				{
					if ((*it)[1].face == (*it2).face)
					{
						if ((*it1).size() == 3)
						{
							if (i == 0)
							{
								bssum = VALUE[(*it1)[1].face] + VALUE[(*it1)[2].face];
								if (bssum < pgsum)
								{
									(*it).push_back((*it1)[0]);
									completeGroup.push_back(*it);
									it = partialGroup.erase(it);
									partialGroupInc = 1;
									it1 = completeSeq.erase(it1);
									completeSeqInc = 1;
								}
							}
							else if (i == 1)
							{
								bssum = VALUE[(*it1)[0].face] + VALUE[(*it1)[2].face];
								if (bssum < pgsum)
								{
									(*it).push_back((*it1)[1]);
									completeGroup.push_back(*it);
									it = partialGroup.erase(it);
									partialGroupInc = 1;
									it1 = completeSeq.erase(it1);
									completeSeqInc = 1;
								}
							}
							else if (i == 2)
							{
								bssum = VALUE[(*it1)[0].face] + VALUE[(*it1)[1].face];
								if (bssum < pgsum)
								{
									(*it).push_back((*it1)[2]);
									completeGroup.push_back(*it);
									it = partialGroup.erase(it);
									partialGroupInc = 1;
									it1 = completeSeq.erase(it1);
									completeSeqInc = 1;
								}
							}
						}
						else if ((*it1).size() == 4)
						{
							if (i == 0)
							{
								bssum = 0;
								(*it).push_back((*it1)[0]);
								completeGroup.push_back(*it);
								it = partialGroup.erase(it);
								partialGroupInc = 1;
								std::vector<Card> newCompleteSeq;
								newCompleteSeq.push_back((*it1)[1]);
								newCompleteSeq.push_back((*it1)[2]);
								newCompleteSeq.push_back((*it1)[3]);
								it1 = completeSeq.erase(it1);
								tempSeq.push_back(newCompleteSeq);
								completeSeqInc = 1;
							}
							else if (i == 3)
							{
								bssum = 0;
								(*it).push_back((*it1)[3]);
								completeGroup.push_back(*it);
								it = partialGroup.erase(it);
								partialGroupInc = 1;
								std::vector<Card> newCompleteSeq;
								newCompleteSeq.push_back((*it1)[0]);
								newCompleteSeq.push_back((*it1)[1]);
								newCompleteSeq.push_back((*it1)[2]);
								it1 = completeSeq.erase(it1);
								tempSeq.push_back(newCompleteSeq);
								completeSeqInc = 1;
							}
							else if (i == 1)
							{
								bssum = VALUE[(*it1)[0].face] + VALUE[(*it1)[2].face] + VALUE[(*it1)[3].face];
								if (bssum < pgsum)
								{
									(*it).push_back((*it1)[1]);
									completeGroup.push_back(*it);
									it = partialGroup.erase(it);
									partialGroupInc = 1;
									it1 = completeSeq.erase(it1);
									completeSeqInc = 1;
								}
							}
							else if (i == 2)
							{
								bssum = VALUE[(*it1)[0].face] + VALUE[(*it1)[1].face] + VALUE[(*it1)[3].face];
								if (bssum < pgsum)
								{
									(*it).push_back((*it1)[2]);
									completeGroup.push_back(*it);
									it = partialGroup.erase(it);
									partialGroupInc = 1;
									it1 = completeSeq.erase(it1);
									completeSeqInc = 1;
								}
							}
						}
						else if ((*it1).size() == 5)
						{
							if (i == 0)
							{
								bssum = 0;
								(*it).push_back((*it1)[0]);
								completeGroup.push_back(*it);
								it = partialGroup.erase(it);
								partialGroupInc = 1;
								std::vector<Card> newCompleteSeq;
								newCompleteSeq.push_back((*it1)[1]);
								newCompleteSeq.push_back((*it1)[2]);
								newCompleteSeq.push_back((*it1)[3]);
								newCompleteSeq.push_back((*it1)[4]);
								it1 = completeSeq.erase(it1);
								tempSeq.push_back(newCompleteSeq);
								completeSeqInc = 1;
							}
							else if (i == 4)
							{
								bssum = 0;
								(*it).push_back((*it1)[4]);
								completeGroup.push_back(*it);
								it = partialGroup.erase(it);
								partialGroupInc = 1;
								std::vector<Card> newCompleteSeq;
								newCompleteSeq.push_back((*it1)[0]);
								newCompleteSeq.push_back((*it1)[1]);
								newCompleteSeq.push_back((*it1)[2]);
								newCompleteSeq.push_back((*it1)[3]);
								it1 = completeSeq.erase(it1);
								tempSeq.push_back(newCompleteSeq);
								completeSeqInc = 1;
							}
							else if (i == 1)
							{
								bssum = VALUE[(*it1)[0].face];
								if (bssum < pgsum)
								{
									(*it).push_back((*it1)[1]);
									completeGroup.push_back(*it);
									it = partialGroup.erase(it);
									partialGroupInc = 1;
									std::vector<Card> newCompleteSeq;
									newCompleteSeq.push_back((*it1)[2]);
									newCompleteSeq.push_back((*it1)[3]);
									newCompleteSeq.push_back((*it1)[4]);
									it1 = completeSeq.erase(it1);
									tempSeq.push_back(newCompleteSeq);
									completeSeqInc = 1;
								}
							}
							else if (i == 2)
							{
								bssum = VALUE[(*it1)[0].face] + VALUE[(*it1)[1].face] + VALUE[(*it1)[3].face] + VALUE[(*it1)[4].face];
								if (bssum < pgsum)
								{
									(*it).push_back((*it1)[2]);
									completeGroup.push_back(*it);
									it = partialGroup.erase(it);
									partialGroupInc = 1;
									it1 = completeSeq.erase(it1);
									completeSeqInc = 1;
								}
							}
							else if (i == 3)
							{
								bssum = VALUE[(*it1)[4].face];
								if (bssum < pgsum)
								{
									(*it).push_back((*it1)[3]);
									completeGroup.push_back(*it);
									it = partialGroup.erase(it);
									partialGroupInc = 1;
									std::vector<Card> newCompleteSeq;
									newCompleteSeq.push_back((*it1)[0]);
									newCompleteSeq.push_back((*it1)[1]);
									newCompleteSeq.push_back((*it1)[2]);
									it1 = completeSeq.erase(it1);
									tempSeq.push_back(newCompleteSeq);
									completeSeqInc = 1;
								}
							}
						}
						break;
					}
					i++;
				}
				if (completeSeqInc != 1)
					it1++;
			}
			if (partialGroupInc != 1)
				it++;
		}
		if (tempSeq.size() != 0)
		{
			for (std::vector<std::vector<Card>>::iterator it = tempSeq.begin(); it != tempSeq.end(); ++it)
			{
				completeSeq.push_back((*it));
			}
		}
		for (std::vector<std::vector<Card>>::iterator it = completeSeq.begin(); it != completeSeq.end(); ++it)
		{
			for (std::vector<Card>::iterator it1 = (*it).begin(); it1 != (*it).end(); ++it1)
			{
				std::vector<Card>::iterator itt = std::find(newHand.begin(), newHand.end(), *it1);
				if (itt != newHand.end())
					newHand.erase(itt);
			}
		}
		for (std::vector<std::vector<Card>>::iterator it = completeGroup.begin(); it != completeGroup.end(); ++it)
		{
			for (std::vector<Card>::iterator it1 = (*it).begin(); it1 != (*it).end(); ++it1)
			{
				std::vector<Card>::iterator itt = std::find(newHand.begin(), newHand.end(), *it1);
				if (itt != newHand.end())
					newHand.erase(itt);
			}
		}
		for (std::vector<std::vector<Card>>::iterator it = partialGroup.begin(); it != partialGroup.end(); ++it)
		{
			for (std::vector<Card>::iterator it1 = (*it).begin(); it1 != (*it).end(); ++it1)
			{
				std::vector<Card>::iterator itt = std::find(newHand.begin(), newHand.end(), *it1);
				if (itt != newHand.end())
					newHand.erase(itt);
			}
		}
		std::sort(newHand.begin(), newHand.end(), compare1);
		temp = 0;
		if (newHand.size() > 1)
		{
			for (std::vector<Card>::iterator it = newHand.begin(); it != newHand.end() - 1; ++it)
			{
				Card curr = *it;
				Card next = *(it + 1);
				if (temp == 0)
					potential.push_back(curr);
				if (curr.suit == next.suit && (curr.face + 1) == next.face)
				{
					potential.push_back(next);
					temp = 1;
				}
				else
				{
					if (potential.size() == 2)
						partialSeq.push_back(potential);
					potential.clear();
					temp = 0;
				}
			}
			if (potential.size() >= 2 && potential.size() < 3 && temp == 1)
			{
				partialSeq.push_back(potential);
				potential.clear();
			}
		}
		temp = 0;
		for (std::vector<std::vector<Card>>::iterator it = partialSeq.begin(); it != partialSeq.end(); ++it)
		{
			for (std::vector<Card>::iterator it1 = (*it).begin(); it1 != (*it).end(); ++it1)
			{
				std::vector<Card>::iterator itt = std::find(newHand.begin(), newHand.end(), *it1);
				if (itt != newHand.end())
					newHand.erase(itt);
			}
		}
		std::copy(newHand.begin(), newHand.end(), back_inserter(deadCards));
	}
	int fromDiscard = 0;
	int fromStock = 0;
	int fromDead_1 = 0;
	int fromDead_2 = 0;
	Card discardTop;
	if (deck.discardPileSize() > 0)
	{
		discardTop = deck.peakIntoDiscardPile();
		if (discardTop.suit != 4)
		{
			for (std::vector<std::vector<Card>>::iterator it = partialSeq.begin(); it != partialSeq.end(); ++it)
			{
				if ((*it)[0].suit == discardTop.suit)
				{
					if ((*it)[0].face == discardTop.face + 1 || (*it)[(*it).size() - 1].face == discardTop.face - 1)
					{
						fromDiscard = 1;
						break;
					}
				}
			}
			if (fromDiscard != 1)
			{
				for (std::vector<std::vector<Card>>::iterator it = partialGroup.begin(); it != partialGroup.end(); ++it)
				{
					if ((*it)[0].face == discardTop.face)
					{
						fromDiscard = 1;
						break;
					}
				}
			}
			if (fromDiscard != 1)
			{
				for (std::vector<std::vector<Card>>::iterator it = completeSeq.begin(); it != completeSeq.end(); ++it)
				{
					if ((*it)[0].suit == discardTop.suit)
					{
						if ((*it)[0].face == discardTop.face + 1 || (*it)[(*it).size() - 1].face == discardTop.face - 1)
						{
							fromDiscard = 1;
							break;
						}
					}
				}
			}
			if (fromDiscard != 1)
			{
				for (std::vector<std::vector<Card>>::iterator it = completeGroup.begin(); it != completeGroup.end(); ++it)
				{
					if ((*it)[0].face == discardTop.face)
					{
						fromDiscard = 1;
						break;
					}
				}
			}
			if (fromDiscard != 1)
			{
				if (deadCards.size() > 1)
				{
					for (std::vector<Card>::iterator it = deadCards.begin(); it != deadCards.end(); ++it)
					{
						if ((*it).face == discardTop.face)
						{
							fromDead_1 = 1;
							fromDiscard = 1;
							break;
						}
						else if ((*it).suit == discardTop.suit && (((*it).face == discardTop.face + 1) || (*it).face == discardTop.face - 1))
						{
							fromDead_2 = 1;
							fromDiscard = 1;
							break;
						}
					}
				}
			}
			if (fromDiscard != 1)
				fromStock = 1;
		}
		else
			fromStock = 1;
	}
	else
		fromStock = 1;
	
	if (!jokerFound)
	{
		if (deadCards.size() > 0)
		{
			int highest = -2;
			for (std::vector<Card>::iterator it = deadCards.begin(); it != deadCards.end(); ++it)
			{
				if ((*it).face > highest)
				{
					if (fromDiscard == 1 && fromDead_1 == 1 &&((*it).face == discardTop.face))
					{					
					}
					else if (fromDiscard == 1 && fromDead_2 == 1 && ((*it).suit == discardTop.suit && (((*it).face == discardTop.face + 1) || (*it).face == discardTop.face - 1)))
					{					
					}
					else
					{					
						cardToDiscard = *it;
						highest = (*it).face;
					}
				}
			}
		}
		else if (partialSeq.size() > 0 || partialGroup.size() > 0)
		{
			int highest = -2;
			if (partialSeq.size() > 0)
			{
				for (std::vector<std::vector<Card>>::iterator it = partialSeq.begin(); it != partialSeq.end(); ++it)
				{
					for (std::vector<Card>::iterator it1 = (*it).begin(); it1 != (*it).end(); ++it1)
					{
						if ((*it1).face > highest)
						{
							cardToDiscard = *it1;
							highest = (*it1).face;
						}
					}
				}
			}
			if (partialGroup.size() > 0)
			{
				for (std::vector<std::vector<Card>>::iterator it = partialGroup.begin(); it != partialGroup.end(); ++it)
				{
					for (std::vector<Card>::iterator it1 = (*it).begin(); it1 != (*it).end(); ++it1)
					{
						if ((*it1).face > highest)
						{
							cardToDiscard = *it1;
							highest = (*it1).face;
						}
					}
				}
			}
		}
		else if (completeSeq.size() > 0)
		{
			int highest = -2;
			for (std::vector<std::vector<Card>>::iterator it = completeSeq.begin(); it != completeSeq.end(); ++it)
			{
				if ((*it).size() >= 4 && (*it)[(*it).size() - 1].face > highest)
				{
					cardToDiscard = (*it)[(*it).size() - 1];
					highest = (*it)[(*it).size() - 1].face;
				}
			}
		}
		else if (completeGroup.size() > 0)
		{
			for (std::vector<std::vector<Card>>::iterator it = completeGroup.begin(); it != completeGroup.end(); ++it)
			{
				if ((*it).size() >= 4)
					cardToDiscard = (*it)[0];
			}
		}
	}
	std::vector<Card>::iterator itt = std::find(hand.begin(), hand.end(), cardToDiscard);
	Card topDeck;
	if (itt != hand.end())
	{
		if (fromDiscard == 1)
			topDeck = deck.drawFromDiscardPile();
		deck.discardCard(*itt);
		hand.erase(itt);
	}
	if (fromStock == 1)
	{
		if (deck.deckSize() > 0)
			topDeck = deck.drawCard();
		hand.push_back(topDeck);
		if (deck.deckSize() == 0)
			deck.copyFromDiscardToStock();
	}
	else if (fromDiscard == 1)
	{
		hand.push_back(topDeck);
	}
	if (!jokerFound)
		return 0;
	else
		return 1;
}

/********************************************************
main() 

Implements all the 3 variants.
*********************************************************/
int main()
{
	char playAgain;
	do
	{
		int variant;
		std::cout << "Choose the variant of the game(1-3):\n";
		std::cout << "1. Base\n";
		std::cout << "2. Variant 1 (Miss with joker)\n";
		std::cout << "3. Variant 2 (Add meld values and find loser)\n";
		std::cin >> variant;
		if (!(variant >= 1 && variant <= 3))
		{
			std::cout << "Wrong variant number. Choose between 1-3\n";
			return 1;
		}
		switch (variant)
		{
			case 1:
			{
				  int numPlayers;
				  std::cout << "\nPlaying Rummy\n";
				  std::cout << "Enter number of players(2-4):";
				  std::cin >> numPlayers;
				  if (!(numPlayers > 1 && numPlayers <= 4))
				  {
					  std::cout << "Number of Players should be between 2-4. Exiting\n";
					  return 1;
				  }
				  Deck d(0);
				  d.shuffleDeck();
				  Player *players = new Player[numPlayers];
				  for (int i = 0; i < numPlayers; ++i)
				  {
					  players[i].assignHand(d.giveHandtoPlayer(HANDSIZE));
				  }
				  unsigned long k = 1;
				  int winningCond = 0;
				  int winningPlayer = -1;
				  while (winningCond != 1)
				  {
					  if (k == 1)
						  std::cout << "Starting Hands:\n";
					  for (int i = 0; i < numPlayers; i++)
					  {
						  std::cout << "Player ";
						  std::cout << i + 1;
						  std::cout << " Hand: ";
						  players[i].displayHand();
						  std::cout << "\n";
					  }
					  for (int i = 0; i < numPlayers; ++i)
					  {
						  analyseHand(players[i].hand, d, winningCond, 0);
						  if (winningCond == 1)
						  {
							  winningPlayer = i + 1;
							  break;
						  }
					  }
					  if (winningCond == 1)
						  break;
					  else
						  std::cout << "At end of turn " << k << ":\n";
					  k++;
				  }
				  std::cout << "\nWinning Player is: Player " << winningPlayer << "\n";
				  break;
			}
			case 2:
			{
				  int numPlayers;
				  std::cout << "\nPlaying Rummy\n";
				  std::cout << "Enter number of players(2-4):";
				  std::cin >> numPlayers;	
				  if (!(numPlayers > 1 && numPlayers <= 4))
				  {
					  std::cout << "Number of Players should be between 2-4. Exiting\n";
					  return 1;
 				  }
				  Deck d(1);
				  d.shuffleDeck();
				  Player *players = new Player[numPlayers];
				  for (int i = 0; i < numPlayers; ++i)
				  {
					  players[i].assignHand(d.giveHandtoPlayer(HANDSIZE));
				  }
				  unsigned long k = 1;
				  int winningCond = 0;
				  int winningPlayer = -1;
				  while (winningCond != 1)
				  {
					  if (k == 1)
						  std::cout << "Starting Hands:\n";
					  for (int i = 0; i < numPlayers; i++)
					  {
						  std::cout << "Player ";
						  std::cout << i + 1;
						  std::cout << " Hand: ";
						  players[i].displayHand();
						  std::cout << "\n";
					  }
					  for (int i = 0; i < numPlayers; ++i)
					  {
						  if (!players[i].miss)
						  {
							  int joker = analyseHand(players[i].hand, d, winningCond, 1);
							  if (joker == 1)
							  {
								  players[i].miss = true;
							  }
							  if (winningCond == 1)
							  {
								  winningPlayer = i + 1;
								  break;
							  }
						  }
						  else
						  {
							  players[i].numMisses++;
							  if (players[i].numMisses == 2)
							  {
								  players[i].miss = false;
								  players[i].numMisses = 0;
							  }
						  }
					  }
					  if (winningCond == 1)
						  break;
					  else
						  std::cout << "At end of turn " << k << ":\n";
					  k++;
				  }
				  std::cout << "\nWinning Player is: Player " << winningPlayer << "\n";
				  break;
			}
			case 3:
			{
				  int numPlayers;
				  std::cout << "\nPlaying Rummy\n";
				  std::cout << "Enter number of players(2-4):";
				  std::cin >> numPlayers;
				  if (!(numPlayers > 1 && numPlayers <= 4))
				  {
					  std::cout << "Number of Players should be between 2-4. Exiting\n";
					  return 1;
				  }
				  Deck d(1);
				  d.shuffleDeck();
				  Player *players = new Player[numPlayers];
				  for (int i = 0; i < numPlayers; ++i)
				  {
					  players[i].assignHand(d.giveHandtoPlayer(HANDSIZE));
				  }
				  unsigned long k = 1;
				  int winningCond = 0;
				  int winningPlayer = -1;
				  int *playerPoints = new int[numPlayers];
				  bool exceedFlag = false;
				  int losingPlayer = -1;
				  for (int j = 0; j < numPlayers; j++)
					  playerPoints[j] = 0;
				  unsigned long x = 0;
				  do
				  {
					  if (x != 0)
					  {
						  d.reset();
						  d.shuffleDeck();
						  for (int i = 0; i < numPlayers; i++)
						  {
							  players[i].reset();
							  players[i].assignHand(d.giveHandtoPlayer(HANDSIZE));
						  }
						  winningCond = 0;
						  winningPlayer = -1;
						  k = 1;
						  std::cout << "Play reset\n";
					  }
					  while (winningCond != 1)
					  {
						  if (k == 1)
							  std::cout << "Starting Hands:\n";
						  for (int i = 0; i < numPlayers; i++)
						  {
							  std::cout << "Player ";
							  std::cout << i + 1;
							  std::cout << " Hand: ";
							  players[i].displayHand();
							  std::cout << "\n";
						  }
						  for (int i = 0; i < numPlayers; ++i)
						  {
							  analyseHand(players[i].hand, d, winningCond, 1);
							  if (winningCond == 1)
							  {
								  winningPlayer = i + 1;
								  break;
							  }
						  }
						  if (winningCond == 1)
							  break;
						  else
							  std::cout << "At end of turn " << k << ":\n";
						  k++;
					  }
					  for (int j = 0; j < numPlayers && j != winningPlayer - 1; j++)
					  {
						  players[j].evaluatePoints();
						  playerPoints[j] += players[j].totalPoints;
						  if (playerPoints[j] > EXCEED)
						  {
							  exceedFlag = true;
							  losingPlayer = j + 1;
							  break;
						  }
					  }
					  x++;
				  } while (!exceedFlag);
				  std::cout << "\nLoser is: Player " << losingPlayer << "\n";
				  break;
			}
		}
		std::cout << "Play again?(y/n)";
		std::cin >> playAgain;
	}while (playAgain == 'y' || playAgain == 'Y');
	return 0;
}