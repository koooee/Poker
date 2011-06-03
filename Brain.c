/*
Brain.c
The purpose of Brain.c is to give the players intelligence/functionality.  Player.c will include a Brain.c which contains functions that will be able to answer common Player questions when analyzing their poker hand
such as... What is My current hand? What Am I drawing to? How many outs do I have? It will also contain various helperfunctions for
Programmer: Nick Kolegraff
Date: 5/18/2011
*/

#include "Deck.c"
#include "Bins.h"
#include "math.h"

#define GUNSHOT 'G'
#define OPENENDED 'O'

void Swap(CARD *c, CARD *c2)
{/* Helper for SortHand function */
  CARD temp;
  temp = *c;
  *c = *c2;
  *c2 = temp;
}
void Sort_Hand(CARD *hand)
{
  /* bubble sort */ 
  /* should probably make this faster...but, N is going to be very limited. */
  int i,j;
  for(i = 0; i < MAX_HAND_SIZE; i++)
    for(j = i; j < MAX_HAND_SIZE; j++)
      {
	if(hand[i].rank < hand[j].rank)
	  {
	    Swap(&hand[i], &hand[j]);
	  }
      }
}

char Gunshot(CARD *hand)
{/* Check if our straights are gunshot or openended; G - Gunshot; O - Openened */
  return 'G';
}

void Drawing_To(BIN *bin)
{/* check which hands we are one card away from */
  
}

char Player_Card_Makes_Hand(CARD *hand, int hand_len)
{
  int i;
  for(i = 0; i < hand_len; i++)
    {
      if(hand[i].whos_card == 'P')
	return TRUE;
    }
  return FALSE;
}
int Distance(CARD c, CARD c2)
{
  return abs(c.rank - c2.rank);
}

double Rank_Hand(CARD *hand, BIN *bin)
{/* Rank the hand while keeping State in the BINS */
  Sort_Hand(hand);
  int i;

  bin->HC = hand[0]; /* this will always be the case since we are sorting */
  bin->is_bin_full[0] = TRUE;

  for(i = 0; i < MAX_HAND_SIZE-1; i++)
    {
      printf("Checking Distance\n");
      int distance = Distance(hand[i], hand[i+1]);
      if(distance == 0)
	{/* Cards Match */
	  /* Fill Coresponding Bins */
	  printf("MATCH!!\n");
	}
      if(distance == 1)
	{
	  printf("Straight Potential\n");
	}
      
	  

    }
 /* Special Cases */
 /* Check for an Ace in the front and 2 in the back...that way we know there is a potential wheel */
  

  
  return 0.0;
}


