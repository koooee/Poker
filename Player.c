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

//Helper function to determine winner
int stateToIndex(char *state)
{
  if(state[0] == 1 && state[1] == 1)
    return 0;
  if(state[0] == 1 && state[1] == 2)
    return 1;
  if(state[0] == 1 && state[1] == 3)
    return 2;
  if(state[0] == 2 && state[1] == 1)
    return 3;
  if(state[0] == 2 && state[1] == 2)
    return 4;
  if(state[0] == 2 && state[1] == 3)
    return 5;
  if(state[0] == 3 && state[1] == 1)
    return 6;
  if(state[0] == 3 && state[1] == 2)
    return 7;
  if(state[0] == 3 && state[1] == 3)
    return 8;
  return -1;
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
char BinContainsWhosCard (BIN *bin, int hand_rank, char key) /* Key is whos card you are looking for ie. PLAYERS, DEALERS, BOARDS */
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
          if(bin->F.b_count[i] == 4)
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
          if(bin->SF.b_count[i] == 4)
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

CARD *GetBinByRank(PLAYER *p, int hand_rank, int *size)
{// This function is a direct result of my stupidity.  See comments in Bins.h.
  int i;
  switch(hand_rank)
    {
    case 0:
      *size = 1;
      return &p->bin.HC;
      break;
    case 1:
      *size = p->bin.P.b_max;
      return p->bin.P.b;
      break;
    case 2:
      *size = p->bin.TP.b_max;
      return p->bin.TP.b;
      break;
    case 3:
      *size = p->bin.TK.b_max;
      return p->bin.TK.b;
      break;
    case 4:
      *size = p->bin.S.b_max;
      return p->bin.S.b;
      break;
    case 5:
      for(i = 0; i < MAX_NUM_SUITS; i++)
        {
          if(p->bin.F.b_count[i] == 5) // yes we only return the full one since this is used in the Winner function
            {
              *size = p->bin.F.b_max;
              return p->bin.F.b[i];
            }
        }
      break;
    case 6:
      *size = p->bin.FH.b_max;
      return p->bin.FH.b;
      break;
    case 7:
      *size = p->bin.FK.b_max;
      return p->bin.FK.b;
      break;
    case 8:
      for(i = 0; i < MAX_NUM_SUITS; i++)
        {
          if(p->bin.SF.b_count[i] == 5)
            {
              *size = p->bin.SF.b_max;
              return p->bin.SF.b[i];
            }
        }
      break;
    default:
      printf("Rank %d doesn't exist as a poker hand rank\n", hand_rank);
      exit(EXIT_FAILURE);
      break;
    }
  *size = -1;
  return NULL;
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
                      if(hand[k].whos_card == BOARDS || temp[l].whos_card == BOARDS)
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
                          if(hand[k].whos_card == BOARDS || temp[l].whos_card == BOARDS)
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
    if(p->bin.is_full[i] == TRUE && i > 0)
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

  // check Flush
  for(i = 0; i < MAX_NUM_SUITS; i++)
    {     
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
  
  // Since my rank algo is flawed we need to check for drawing hands
  // as the state isn't kept quite correctly in the BINS data structure.
  // Would be a good thing to fix...
  // Straight Flush and Straights will get checked in this function

  char draw;
  draw = drawingto(p->hand, who_is_this, state, size_of_hand);
  return;
}

/* // Same a above function only returns number of outs to a drawing hand. */
int QueryBinOuts(PLAYER *p)
{/* TODO: not implemented yet */
  int outs;
  return outs;
}

// Helper for Winner Function
int min(int x, int y)
{/* return the largest value, if they are = just return one of them */
  if(x < y)
    return x;
  else
    return y;
}

int Winner(PLAYER *p, PLAYER *d, int p_size, int d_size, int *p_rank, int *d_rank) // p - player; d - dealer;
{// 0 is left as winner; 1 is right as winner; 2 tie

  *p_rank = rank_hand(p->hand, &p->bin, p_size);
  *d_rank = rank_hand(d->hand, &d->bin, d_size);

  CARD p_kickers[6];
  CARD d_kickers[6]; // we only need 6 since 7 card hands...this all needs to get refactored so I can port it to Omaha etc..
  int i, p_num_kickers, d_num_kickers;

  /* this is pretty easy, clear cut winner */
  if(*p_rank > *d_rank)
    return 0;
  if(*d_rank > *p_rank)
    return 1;

  /* this is not so easy, hand rank is the same, now we need to defer to kickers and/or high card values */
  if(*p_rank == *d_rank)
    {
      CARD *p_bin, *d_bin;
      int *p_bin_size, *d_bin_size;

      /* really need to try and not malloc this shit..for GPU purposes */
      p_bin_size = malloc(sizeof(int *));
      d_bin_size = malloc(sizeof(int *));

      /* get the bin coresponding to the hand rank */
      p_bin = GetBinByRank(p, *p_rank, p_bin_size);

      /* get the number of avaliable kickers after the hand is scored */
      /* example: if the highest hand is a Pair, (2 cards) the leftover size will be 3 (for 5 card hand ranks)*/
      p_num_kickers = MAX_CARDS_TO_RANK - *p_bin_size;
      int count=0;
      for(i = 0; i < MAX_HAND_SIZE; i++)
      {
        /* remove any cards that are in the bin and we have our kickers */
        /* this will be useful later on if we need to compare kickers */
        /* if they end up having the same hand */
        if(Contains(p_bin, p->hand[i], *p_bin_size) == FALSE){
          p_kickers[count++] = p->hand[i];
        }
      }
      
      /* See comments above about the player. dealer is the same. */
      d_bin = GetBinByRank(d, *d_rank, d_bin_size);
      d_num_kickers = MAX_CARDS_TO_RANK - *d_bin_size;
      count=0;
      for(i = 0; i < MAX_HAND_SIZE; i++)
      {
        if(Contains(d_bin, d->hand[i], *d_bin_size) == FALSE){
          d_kickers[count++] = d->hand[i];
        }
      }
      /* printf("Dealer Kickers%d: ",*d_bin_size);printh(d_kickers, d_num_kickers); */
      /* printf("Player Kickers%d: ",*p_bin_size);printh(p_kickers, p_num_kickers); */
      /* sort the hands in the bin as a sanity check */
      /* they should already be sorted, but I can't gaurentee that */
      sort_hand(p_bin, *p_bin_size);
      sort_hand(d_bin, *d_bin_size);

      /* if the rank is the same, lets check if the card values are the same */
      /* in this case we can return if the player has the higher cards */
      /* the only exception is a full-house: you want the highest cards in the TK bin */
      if(*p_rank != 6) {
	for(i = 0; i < *d_bin_size; i++)
	  {
	    /* this is pretty easy, any hand with a higher rank will be the winner (minus the FH)n */
	    if(p_bin[i].rank > d_bin[i].rank){
	      free(p_bin_size);
	      free(d_bin_size);
	      return 0;
	    }


	    if(d_bin[i].rank > p_bin[i].rank){
	      free(p_bin_size);
	      free(d_bin_size);
	      return 1;
	    }
	  }
      }
      /* this is the special case of a full house */
      if(*p_rank == 6){
	/* don't need to check d_rank here since they have to be equal */
	p_bin = GetBinByRank(p, 4, p_bin_size);
	d_bin = GetBinByRank(d, 4, d_bin_size);
	printh(p_bin, *p_bin_size);
	printh(d_bin, *d_bin_size);
	if(p_bin[0].rank > d_bin[0].rank){
	  free(p_bin_size);
	  free(d_bin_size);
	  return 0;
	}
	else if(d_bin[0].rank > p_bin[0].rank){
	  free(p_bin_size);
	  free(d_bin_size);
	  return 1;
	}
	else
	  {/* they are equal, need to check two pair bins  */
	    p_bin = GetBinByRank(p, 3, p_bin_size);
	    d_bin = GetBinByRank(d, 3, d_bin_size);

	    /* we want to make sure the ranks aren't equal to the TK */
	    /* if so, proceed with finding the max rank */
	    if(p_bin[0].rank != p->bin.TK.b[0].rank && d_bin[0].rank != d->bin.TK.b[0].rank){
	      if(p_bin[0].rank > d_bin[0].rank){
		free(p_bin_size);
		free(d_bin_size);
		return 0;
	      }
	      if(d_bin[0].rank > p_bin[0].rank){
		free(p_bin_size);
		free(d_bin_size);
		return 1;
	      }

	      /* do the same for the second pair in the TP bin */
	      if(p_bin[2].rank != p->bin.TK.b[0].rank && d_bin[2].rank != d->bin.TK.b[0].rank){
		if(p_bin[2].rank > d_bin[2].rank){
		  free(p_bin_size);
		  free(d_bin_size);
		  return 0;               
		}
		if(d_bin[2].rank > p_bin[2].rank){
		  free(p_bin_size);
		  free(d_bin_size);
		  return 1;
		}

	      }
	    }
	  }
      } /* END checking FH  Special Case*/

      // Random Comment: Shouldn't all this shit really be in functions?....anyone...anyone...buler...

      // If we get here they have the same ranks with the same cards...so we need to check the kickers
      printf("MAX: %d\n", min(p_num_kickers, d_num_kickers));
      for(i = 0; i < min(p_num_kickers, d_num_kickers); i++)
	{
	  if(p_kickers[i].rank > d_kickers[i].rank)
	    {
	      free(p_bin_size);
	      free(d_bin_size);
	      return 0;
	    }
	  if(d_kickers[i].rank > p_kickers[i].rank)
	    {
	      free(p_bin_size);
	      free(d_bin_size);
	      return 1;
	    }
	}
      
      // Free up some memory
      free(p_bin_size);
      free(d_bin_size);


    }/* END checking same ranks, if we haven't returned by now, then we need to check kickers, Ranked hands are equal */

  // if we didnt return they have the same cards for their hand need to check kickers
  // only need to traverse the smallest hand

  // if we didn't return, we have a tie
  return 2;

}

// somewhere some how payouts.h got deleted....so have fun making that again.
/* float Pays(PLAYER *p, PLAYER *d) */
/* { */
/*   float pay; */
/*   pay = 0.0; */
/*   int *prank, *drank; */
/*   Winner(p, d, prank, drank); */
/*   if(drank > 0){ */
/*     // Dealer Qualifies */
/*   } */
/*   else{ */
/*     // Dealer DOES NOT Qualify */
/*   } */
/*   // Does dealer qualify? */
/*   // if so, Ante Pay */
/*   // Trips pay regardless */
/*   // Blind pays only if you beat dealer, pays regardless on straight or better */
/*   // Else, Ante goes back, all other pay as above */
/*   // ties push, trips still live. */
  
/*   return pay; */
/* } */
