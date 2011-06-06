/*
Bins.h
-------
The purpose of this file is to house the data structure for the ranking algorithm.  We want to dynamically be able to query
this structure at any time and get a sense of where our current hand is (ie. drawing to a flush, etc...)
-------
Programmer: Nick Kolegraff
Date: 5/18/2011
*/

#include "Hands.h"

#define MAX_HAND_RANKS 9 /* Pair, Two Pair, etc... */
#define MADE_HAND 'M'
#define DRAWING 'D'
#define NOTHING 'Z'
#define MAX_SIZE_P 2
#define MAX_SIZE_TP 4
#define MAX_SIZE_TK 3
#define MAX_SIZE_S 5
#define MAX_SIZE_FH 5
#define MAX_SIZE_FK 4

typedef struct BINS {
  /* I toyed around with not doing it like this ie the 'free' concept.  But, decided to go with it.  I'm sure there is a better way to represent this type of structure...will give it some more thought. */
  /* New Comment 5/31/2011 This is kind of like anarchy. FIXME*/

  CARD HC; /* High Card */
  STD_BIN P;
  STD_BIN TP;
  STD_BIN TK;
  STD_BIN S;
  MULTI_BIN F;
  STD_BIN FH;
  STD_BIN FK;
  MULTI_BIN SF;

  /* be used to flag the max hand.  If the bin is full, then the player has that hand */
  char is_full[MAX_HAND_RANKS];
  char drawing[MAX_HAND_RANKS]; /* M - Made hand; D - 1 card to hand; Z - greater than 1 card away */
  
} BIN;

void init_bin(BIN *bin)
{
  int card_size = sizeof(CARD);
  int i;

  bin->P.b = malloc(MAX_SIZE_P * card_size);
  bin->P.b_free = &bin->P.b[0];
  bin->P.b_max = MAX_SIZE_P;
  bin->P.b_count = 0;

  bin->TP.b = malloc(MAX_SIZE_TP * card_size);
  bin->TP.b_free = &bin->TP.b[0];
  bin->TP.b_max = MAX_SIZE_TP;
  bin->TP.b_count = 0;

  bin->TK.b = malloc(MAX_SIZE_TK * card_size);
  bin->TK.b_free = &bin->TK.b[0];
  bin->TK.b_max = MAX_SIZE_TK;
  bin->TK.b_count = 0;

  bin->S.b = malloc(MAX_SIZE_S * card_size);
  bin->S.b_free = &bin->S.b[0];
  bin->S.b_max = MAX_SIZE_S;
  bin->S.b_count = 0;

  bin->FH.b = malloc(MAX_SIZE_FH * card_size);
  bin->FH.b_free = &bin->FH.b[0];
  bin->FH.b_max = MAX_SIZE_FH;
  bin->FH.b_count = 0;

  bin->FK.b = malloc(MAX_SIZE_FK * card_size);
  bin->FK.b_free = &bin->FK.b[0];
  bin->FK.b_max = MAX_SIZE_FK;
  bin->FK.b_count = 0;

  bin->F.b_max = 5;
  bin->SF.b_max = 5;

  for(i = 0; i < MAX_NUM_SUITS; i++)
    {
      bin->F.b_free[i] = &bin->F.b[i][0];
      bin->F.b_count[i] = 0;
      bin->SF.b_free[i] = &bin->SF.b[i][0];
      bin->SF.b_count[i] = 0;      
    }
  for(i = 0; i < MAX_HAND_RANKS; i++)
    {
      bin->is_full[i] = FALSE;
      bin->drawing[i] = 'Z';
    }
}

void clearbin(CARD *b, CARD *free, int *count)
{
  int i;
  CARD empty;
  for(i = 0; i < *count; i++)
    {
      b[i] = empty;
    }
  *free = b[0];
  *count = 0;
  
}


void reset_bin(BIN *bin)
{
  free(bin->FK.b);
  free(bin->FH.b);
  free(bin->S.b);
  free(bin->TK.b);
  free(bin->TP.b);
  free(bin->P.b); 
}

void Add(CARD c, CARD **free, int max, int *count)
{/* Yes, I know, this interface seems a bit retarded.  FIXME */
  if(*count == max)
    {
      printf("Your bin is full.\n");
      exit(EXIT_FAILURE);
    }
  else
    {
      **free = c;
      ++*free;
      ++*count;
    }
  
}

void Remove(CARD **free, int *count)
{
  if(*count != 0)
    {
      CARD blank;
      **free = blank;
      --*free;
      --*count;  
    }
}

