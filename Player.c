/*
Player.c
The purpose of this file is to hold a structure that represents a player.  Players contain information about their current game state.  Dealers are also players.
Programmer: Nick Kolegraff
Date: 5/18/2011
*/

#include "Brain.c"

typedef struct player {
  CARD hand[MAX_HAND_SIZE];
  BIN bin;
  int num_outs;
  double best_drawing_hand;
  double all_drawing_hands[9];
  double current_best_hand;
  /* TODO: There are many more attributes we will need for analyzing players current game status to make effective decisions */
} PLAYER;

/* TODO: Player Init function */
/* Each player should get their own hand and BINS */

/*Debugging PrintHand*/
void PrintHand(CARD *h, int hand_size)
{/* Debugging */

  int i; 
  for(i =0; i < hand_size; i++)
    {
      printf("%c%c  ", ranks[h[i].rank], suits[h[i].suit]);
    }
  printf("\n");
}

