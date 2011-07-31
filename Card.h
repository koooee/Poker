/* Card.h
Data structure to hold a card.  There is no functionality for this data structure.
Programmer: Nick Kolegraff
Date: 5/29/2011
*/

#include "stdio.h"
#include "stdlib.h"

#define MAX_NUM_SUITS 4
#define MAX_NUM_RANKS 13
#define START_RANK 2 /* For looping */
#define END_RANK 15 /* For looping */
#define EMPTY_CARD -1
#define DEALERS 'D'
#define PLAYERS 'P'
#define NO_ONES 'N'
#define YCMH 1 // your card makes hand
#define HPBB 2 // hand played by board
#define NH   3 // hand isn't made or draw
#define TRUE 1
#define FALSE 0

char ranks[MAX_NUM_RANKS] = "23456789TJQKA";
char suits[MAX_NUM_SUITS] = "shdc";

typedef struct card {
  int rank;
  int suit;
  char whos_card;
} CARD;

void printc(CARD c)
{/* print card */
  printf("%c%c", ranks[c.rank], suits[c.suit]);
}
