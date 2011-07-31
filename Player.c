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


// Helper for BinContainsWhosCard function
char isKey(CARD *hand, char key, int size)
{
  int i;
  for(i = 0; i < size; i++)
    {
      if(hand[i].whos_card == key)
	{
	  return TRUE;
	}
    }
  return FALSE;
}

// Helper for QueryBinArray function
char BinContainsWhosCard (BIN *bin, int hand_rank, char key) /* Key is whos card you are looking for ie. PLAYERS, DEALERS, NO_ONES */
{// function returns true or false if it finds the key...this function is a direct result of my stupidity.  See comments in Bin.h Struct definition
  int i;
  char t = FALSE;
  switch(hand_rank)
    {
    case 0:
      if(bin->HC.whos_card == key)
    	t = TRUE;
      break;
    case 1:
      t = isKey(bin->P.b, key, MAX_SIZE_P);
      break;
    case 2:
      t =  isKey(bin->TP.b, key, MAX_SIZE_TP);
      break;
    case 3:
      t =  isKey(bin->TK.b, key, MAX_SIZE_TK);
      break;
    case 4:
      t =  isKey(bin->S.b, key, MAX_SIZE_S);
      break;
    case 5:
      for(i = 0; i < MAX_NUM_SUITS; i++)
    	{
    	  if(isKey(bin->F.b[i], key, bin->F.b_count[i]) == TRUE)
    	    t =  TRUE;
    	}
      break;
    case 6:
      t =  isKey(bin->FH.b, key, MAX_SIZE_FH);
      break;
    case 7:
      t =  isKey(bin->FK.b, key, MAX_SIZE_FK);
      break;
    case 8:
      for(i = 0; i < MAX_NUM_SUITS; i++)
    	{
    	  if(isKey(bin->SF.b[i], key, bin->SF.b_count[i]) == TRUE)
    	    t =  TRUE;
    	}
      break;
    default:
      printf("Rank %d doesn't exist as a poker hand rank\n", hand_rank);
      exit(EXIT_FAILURE);
      break;
    }
  return t;
}

char drawingto(CARD *hand, char key, char *state, int len)
{// key is whos card to look for.  match is a T/F if it finds key.
  // any contigious subset sum == 4 is a gunshot
  // any contigious subset sum == 3 is an openended
  // only because we are sorting the hand does this algo work.
  // if you try to pass a non sorted hand to this function
  // it will fail.
  CARD temp[4];
  CARD thand[7];
  int offset = len - 3;
  /* printf("offset: %d\n", offset); */
  int i,j,k,l,sum=0, count=0;
  for(i = 0; i < len; i++)
    thand[i] = hand[i];
  // first check for dup cards...cuz that will f shit up.
  // this should be it's own function...if we need it in other places..refactor
  // since we could be removing our card that also makes the hand we wll need to check
  // for that further down in the function.
  for(i = 1; i < len; i++)
    {
      if(thand[i].rank == thand[i-1].rank)
  	{
  	  count++;
  	  for(j = i+1; j<len; j++)
  	    {// this will remove i and shift the bottom half up one
  	      thand[j-1] = thand[j];
  	    }

  	}
	  
    }

  // not sure why we don't need to do this?
  // len = len - count;

  // contigious subset sums algo (on sorted data)
  for(i = 0; i < offset; i++)
    {
      for(j = i; j <= len - (offset - i); j++)
	temp[j-i] = thand[j];

      sum = sum_of_distances(temp, 4);

      // Gunshot Draw
      if(sum == 4)
	{
	  state[1] = HPBB;
	  // in the case of a pair, we might miss our card...so this is a final check
	  for(l = 0; l < 4; l++)
	    {
	      for(k = 0; k < len; k++)
		{
		  if(hand[k].rank == temp[l].rank && hand[k].suit != temp[l].suit)
		    {
		      if(hand[k].whos_card == NO_ONES || temp[l].whos_card == NO_ONES)
			state[1] = HPBB;
		      else
			state[1] = YCMH;
		    }
		}
	    }

	  return GUNSHOT;
	}

      // Openended Draw
      if(sum == 3)
	{
	      state[1] = HPBB;
	      //in the case of a pair, we might miss our card...so this is a final check
	      for(l = 0; l < 4; l++)
		{
		  for(k = 0; k < len; k++)
		    {
		      if(hand[k].rank == temp[l].rank && hand[k].suit != temp[l].suit)
			{
			  if(hand[k].whos_card == NO_ONES || temp[l].whos_card == NO_ONES)
			    state[1] = HPBB;
			  else
			    state[1] = YCMH;
			}
		    }
		}
	  return OPENENDED;
	}
    }
  return NOTHING;
}


// Function to Query the players bins and return an array of the 'state'
// 'state' is defined as an abstraction of the current event -- ie, Drawing Hand, Made Hand.
// the function will return a char array of two elements:
//  0th index: made a hand
//      1 -- your card makes the hand
//      2 -- hand is played by the board
//      3 -- you don't have a made hand
//  1th index: drawing to a hand
//      1 -- your card makes the draw
//      2 -- draw is played by the board
//      3 -- you don't have a draw
// we define a drawing hand as one card to any Straight, Flush, or Straight Flush.
// pretty crude at the moment but we will try and write it with enough abstraction to add in better functionality
// as in drawing to a Straight with x outs to y hands

void QueryBinArray(PLAYER *p, char *state, char *match, char who_is_this, int size_of_hand)
{// TODO: add ability to distinguish between Gunshot and Openended draws
  // check if any bin is full
  // if the bin is full, check whos card
  // if the any bin is not full
  // check if it is drawing to anything
  // if it is: check whos card
  state[0] = NH;
  state[1] = NH;
  int i, j, max_rank;
  max_rank = get_max_rank(&p->bin);
  for(i = max_rank; i >= 0; i--)
    {
    if(p->bin.is_full[i] == TRUE)
      {// we have a made hand
	// now check Does the highest ranking hand contain my card?
	if(BinContainsWhosCard(&p->bin, i, who_is_this) == TRUE)
	  {
	    state[0] = YCMH; // your card makes hand
	    /* we break because this is our best made hand and we now
	       want to check to see if we are also drawing to something
	    */
	    break;
	  }
	else
	  {
	    state[0] = HPBB; // hand played by board
	    break;
	  }
      }
    }

  // check for a drawing hand

  // check SF and F
  for(i = 0; i < MAX_NUM_SUITS; i++)
    {	  
      /* if(p->bin.SF.b_count[i] == 4) */
      /* 	{// check for your card */

      /* 	  if(BinContainsWhosCard(&p->bin, 8, who_is_this) == TRUE) */
      /* 	    { */
      /* 	      state[1] = YCMH; */
      /* 	      return; */
      /* 	    } */
      /* 	  else */
      /* 	    { */
      /* 	      state[1] = HPBB; */
      /* 	      return; */
      /* 	    } */
      /* 	} */
      if(p->bin.F.b_count[i] == 4)
	{
	  if(BinContainsWhosCard(&p->bin, 5, who_is_this) == TRUE)
	    {
	      state[1] = YCMH;
	      return;
	    }
	  else
	    {
	      state[1] = HPBB;
	      return;
	    }
	}
    }
  
  // check Straight
  /* if(p->bin.S.b_count == 4) */
  /*   { */
  /*     if(BinContainsWhosCard(&p->bin, 4, who_is_this) == TRUE) */
  /* 	{ */
  /* 	  state[1] = YCMH; */
  /* 	  return; */
  /* 	} */
  /*     else */
  /* 	{ */
  /* 	  state[1] = HPBB; */
  /* 	  return; */
  /* 	} */
  /*   } */

  // Since my rank algo is flawed we need to check for drawing hands
  // as the state isn't kept quite correctly in the BINS data structure.
  // Would be a good thing to fix...

  char draw;
  draw = drawingto(p->hand, who_is_this, state, size_of_hand);
  return;
}

/* // Same a above function only returns number of outs to a drawing hand. */
int QueryBinOuts(PLAYER *p)
{
  int outs;
  return outs;
}

int Winner(PLAYER *p, PLAYER *d) // p - player; d - dealer;
{// 0 is left as winner; 1 is right as winner
  return -1;
}

float Pays(PLAYER *p, PLAYER *d)
{
  // Does dealer qualify?
  float pay;
  pay = 0.0;
  return pay;
}
