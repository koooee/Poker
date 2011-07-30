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

} PLAYER;

/* TODO: Player Init function */
/* Each player should get their own hand and BINS */


