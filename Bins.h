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
