#include "Deck.h"
// TODO: need to test the deck functionality
// TODO: implement shuffle -- get a good PRNG for this..not rand
void InitDeck(void)
{
  int i,j;
  for(i = 0; i < MAX_NUM_RANKS; i++)
    for(j = 0; j < MAX_NUM_SUITS; j++)
      {
	int di = (i * (MAX_NUM_SUITS-1)) + i+j; /* Deck Index */
	deck[di].rank = i;
	deck[di].suit = j;
	deck[di].whos_card = 'N';
      }
  current_deck_size = 52; 
}

void ClearDeck()
{
  int i;
  for(i = 0; i < MAX_DECK_SIZE; i++)
    {
      deck[i].rank = EMPTY_CARD;
      deck[i].suit = EMPTY_CARD;
      deck[i].whos_card = 'N';
    }
  current_deck_size = 0;
}
void PrintDeck()
{ /* For Debugging */
  int i;
  for(i = 0; i < MAX_DECK_SIZE; i++)
    {
      printf("%c%c - %c\n", ranks[deck[i].rank], suits[deck[i].suit], deck[i].whos_card);
    }
}

int isEmpty()
{
  if(current_deck_size <= 0)
    return FALSE;
  else
    return TRUE;
  return -1;
}

CARD draw()
{
  deck[MAX_DECK_SIZE-current_deck_size].rank = EMPTY_CARD;
  deck[MAX_DECK_SIZE-current_deck_size].suit = EMPTY_CARD;
  deck[MAX_DECK_SIZE-current_deck_size].whos_card = 'N';
  current_deck_size--;
  return deck[MAX_DECK_SIZE-(current_deck_size--)];
  
}

CARD peek()
{
  return deck[MAX_DECK_SIZE-current_deck_size];
}

void shuffle(CARD *d)
{
  
}
