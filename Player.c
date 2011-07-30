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

void InitHand(PLAYER *p)
{
  int i;
  for(i = 0; i < MAX_HAND_SIZE; i++)
    {
      CARD temp;
      temp.rank = -1;
      temp.suit = -1;
      p->hand[i] = temp;
    }
}

void InitPlayer(PLAYER *p)
{
  init_bin(&p->bin);
  InitHand(p);
}
