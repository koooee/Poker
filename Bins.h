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

void Add(CARD c, CARD **hand, int max, int *count)
{/* Yes, I know, this interface seems a bit retarded.  FIXME */
  if(*count == max)
    {
      printf("Your bin is full.\n");
      exit(EXIT_FAILURE);
    }
  else 
    {
      *hand + (*count)++;
      **hand = c;
    }
  
}

void init_bin(BIN *bin)
{
  int card_size = sizeof(CARD);
  int i;

  bin->P.b = malloc(MAX_SIZE_P * card_size);
  bin->P.b_max = MAX_SIZE_P;
  bin->P.b_count = 0;

  bin->TP.b = malloc(MAX_SIZE_TP * card_size);
  bin->TP.b_max = MAX_SIZE_TP;
  bin->TP.b_count = 0;

  bin->TK.b = malloc(MAX_SIZE_TK * card_size);
  bin->TK.b_max = MAX_SIZE_TK;
  bin->TK.b_count = 0;

  bin->S.b = malloc(MAX_SIZE_S * card_size);
  bin->S.b_max = MAX_SIZE_S;
  bin->S.b_count = 0;

  bin->FH.b = malloc(MAX_SIZE_FH * card_size);
  bin->FH.b_max = MAX_SIZE_FH;
  bin->FH.b_count = 0;

  bin->FK.b = malloc(MAX_SIZE_FK * card_size);
  bin->FK.b_max = MAX_SIZE_FK;
  bin->FK.b_count = 0;

  bin->F.b_max = 5;
  bin->SF.b_max = 5;
  /* TODO: Enhancement */
  /* We should dynamically malloc in the Add function (Below)...thus, only using memory when needed...but, this 
     might have an effect on preformance since we would be calling malloc frequently..and I am very concerned
     about runtime preformance
  */
  bin->F.b_count = malloc(NUM_CARDS_TO_RANK * sizeof(CARD));
  bin->SF.b_count = malloc(NUM_CARDS_TO_RANK * sizeof(CARD));

  for(i = 0; i < MAX_NUM_SUITS; i++)
    {
      bin->F.b[i] = malloc(NUM_CARDS_TO_RANK * sizeof(CARD));
      bin->SF.b[i] = malloc(NUM_CARDS_TO_RANK * sizeof(CARD));
      bin->F.b_count[i] = 0;
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
  CARD blank;
  int i;
  bin->HC = blank;
  bin->P.b_count = 0;
  bin->TP.b_count = 0;
  bin->TK.b_count = 0;
  bin->S.b_count = 0;
  bin->FH.b_count = 0;
  bin->FK.b_count = 0;
  
  for(i = 0; i < MAX_NUM_SUITS; i++) {
    bin->SF.b_count[i] = 0;
    bin->F.b_count[i] = 0;
    }
  
  for(i = 0; i < MAX_HAND_RANKS; i++) {
    bin->is_full[i] = FALSE;
    bin->drawing[i] = NOTHING;
  }
}

void free_bins(BIN *bin)
{
  int i,j;
  /* Free Standard Bins */
  free(bin->FK.b);
  bin->FK.b = NULL;
  free(bin->FH.b);
  bin->FH.b = NULL;
  free(bin->S.b);
  bin->S.b = NULL;
  free(bin->TK.b);
  bin->TK.b = NULL;
  free(bin->TP.b);
  bin->TP.b = NULL;
  free(bin->P.b); 
  bin->P.b = NULL;

  /* Free Multi Bins */

  free(bin->F.b_count);
  free(bin->SF.b_count);
  for(i = 0; i < MAX_NUM_SUITS; i++)
    {
      free(bin->F.b[i]);
      bin->F.b[i] = NULL;

      free(bin->SF.b[i]);
      bin->SF.b[i] = NULL;
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

void printb(BIN *bin)
{/* Print Bin */
  int i;

  printf("High Card: ");
  printc(bin->HC);
  printf("\n");

  printf("Pair: ");
  for(i = 0; i < bin->P.b_count; i++)
    {
      printc(bin->P.b[i]);
    }
  printf("\n");

  printf("Two Pair: ");
  for(i = 0; i < bin->TP.b_count; i++)
    {
      printc(bin->TP.b[i]);
    }
  printf("\n");

  printf("Straight: ");
  for(i = 0; i < bin->S.b_count; i++)
    {
      printc(bin->S.b[i]);
    }
  printf("\n");

}

