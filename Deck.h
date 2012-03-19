/*
Deck.h
-------
This is for the Deck data structure.  Decks are similar to stacks. The only difference is we need to shuffle them. And they must contain distinct elements. Hands are very similiar to Decks in the context of this code.  Thus, you will find hands defined in this file as well....Probably not the best way to organize that.
-------
Programmer: Nick Kolegraff
Date: 5/18/2011
*/

#include "Card.h"

/* Change these for other variations of poker that may require something different */
#define MAX_DECK_SIZE 52
#define MAX_HAND_SIZE 7

int current_deck_size = 0;
CARD deck[MAX_DECK_SIZE];

void InitDeck();
void ClearDeck();
void PrintDeck();
CARD pop();
void push(CARD c);
CARD peek();
void shuffle(CARD *d);
