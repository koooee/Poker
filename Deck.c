#include "Deck.h"

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
}
void PrintDeck()
{ /* For Debugging */
  int i;
  for(i = 0; i < MAX_DECK_SIZE; i++)
    {
      printf("%c%c - %c\n", ranks[deck[i].rank], suits[deck[i].suit], deck[i].whos_card);
    }
}
void Append(CARD c, CARD *d)
{
  
}

CARD *Draw(void)
{
}

void Shuffle(void)
{
  CARD temp[MAX_DECK_SIZE];
  int i;
  for(i =0; i < MAX_DECK_SIZE; i++)
    {
      temp[i] = deck[i];
    }
}
