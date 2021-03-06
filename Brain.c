/*
Brain.c

such as... What is My current hand? What Am I drawing to? How many outs do I have? It will also contain various helper functions that you would consider part of someones 'brain'
Programmer: Nick Kolegraff
Date: 5/18/2011
*/

#include "Deck.c"
#include "Bins.h"
#include "math.h"
#include "Payouts.h"

#define GUNSHOT 'G'
#define OPENENDED 'O'

void swap(CARD *c, CARD *c2)
{/* Helper for SortHand function */
  CARD temp;
  temp = *c;
  *c = *c2;
  *c2 = temp;
}

void sort_hand(CARD *hand, int hand_size)
{
  /* bubble sort */ 
  /* should probably make this faster...but, N is going to be very limited. */
  int i,j;
  for(i = 0; i < hand_size; i++)
    for(j = i; j < hand_size; j++)
      {
	if(hand[i].rank < hand[j].rank)
	  {
	    swap(&hand[i], &hand[j]);
	  }
	else if(hand[i].rank == hand[j].rank)
	  {
	    if(hand[i].suit < hand[j].suit)
	      swap(&hand[i], &hand[j]);
	  }
	else ;
      }
}

int sum_of_distances(CARD *hand, int len)
{
  int i,d, total=0;
  for(i = 0; i < len-1; i++)
    {
      
      d = distance(hand[i], hand[i+1]);
      if(d == 0)
	d = 100;
      total += d;
    }
  return total;
}

char Contains(CARD *hand, CARD c, int size)
{
  int i;
  for(i = 0; i < size; i++)
    {
      if(hand[i].rank == c.rank && hand[i].suit == c.suit)
	return TRUE;
    }
  return FALSE;
}

char check_for_s(CARD *hand, int len)
{/* the sum of the differences should be length of the hand - 1...assuming only the cards that make the hand are in the hand */
  int i, total = 0;
  for(i = 0; i < len-1; i++)
    {
      total += distance(hand[i], hand[i+1]);
    }
  if((total == 4 && len == 5) || (total >= 15 && len == 5))
    {
      printf("Total: %d\n", total);
      printh(hand,len);

      return TRUE;
      
    }
  return FALSE;
}

long long factorial(long long n)
{/* Naive approach */
  long long i;
  long long r;
  r = 1LL;
  for(i = n; i > 0LL; i--){
    r = i*r;
  }
  return r;
}

long long choose(long long n, long long r)
{/* we only need to compute the multilpe until we reach difference of n-r */
  /* Example: nCr(52, 2) = 52! / 50! * 2! = 52*51*2! */

  long long i;
  long long o;
  o = 1LL;
  for(i = n; i > (n-r); i--){
    o = i*o;
  }
  o = o/factorial(r);
  return o;
}

long long largestv(long long n, long long r, long long c)
{/* Function is a helper for combinadic */
  /* See: http://msdn.microsoft.com/en-us/library/aa289166%28VS.71%29.aspx */
  long long v = n - 1LL;
  long long result = choose(v, r);
  while(c < result){
    result = choose(--v, r);
  }
  return v;
}

int combinadic(long long  n, long long r, long long mth,  long long *result)
{/* Function will return a representation of the combination integer that can be mapped to a combination element.
    Have a go at this for more details: http://msdn.microsoft.com/en-us/library/aa289166%28VS.71%29.aspx */

  long long dual = (choose(n, r) - 1LL) - mth;

  long long i;
  long long tempn;
  long long tempr;
  tempn = n;
  tempr = r;
  for(i = 0LL; i < r; ++i)
    {
      result[i] = largestv(tempn,tempr,dual);
      //printf("i:%lld\tn:%lld\ttempr:%lld\tdual:%lld\tresult[%lld]=%lld\n", i, tempn, tempr, dual,i,result[i]);
      dual = dual - choose(result[i], tempr);
      --tempr;
    }
  for(i = 0LL; i < r; ++i)
    {
      result[i] = (n-1LL) - result[i];
    }
}

int distance(CARD c, CARD c2)
{
  if((c.rank == 12 || c2.rank == 12) && (c.rank == 3 || c2.rank == 3))
    return 1;
    
  return abs(c.rank - c2.rank);
}

int get_max_rank(BIN *bin)
{
  int final_rank;
  for(final_rank = 8; final_rank >= 0; final_rank--)
    {
      if(bin->is_full[final_rank] == TRUE)
	{
	  return final_rank;
	}
    }
  return -1;

}


int rank_hand(CARD *hand, BIN *bin, int size_of_hand)
{/* Rank the hand while keeping State in the BINS */
  /* hand size must be at least 2 */
  if(size_of_hand < 2)
    {
      printf("Hand size must be >= 2 to rank\n");
      exit(EXIT_FAILURE);
    }
  sort_hand(hand, size_of_hand);
  int i;

  bin->HC = hand[0]; /* this will always be the case since we are sorting */
  bin->is_full[0] = TRUE;

  for(i = 0; i < size_of_hand-1; i++)
    {
      CARD c = hand[i];
      CARD c2 = hand[i+1];

      /* 
	 Flushes:
	         regardless, we add every card to the flush bin.  it is a multi array where matching suits
		 get added to their own row

	 Straight Flushes: Check the last card in the flush bin with the current card to be added
	                   if they differ by one. Add current to the SF bin.  If the SF bin is empty
			   add both cards.
      */
      
      /* Straight Flushes */
      if(bin->is_full[8] == FALSE)
	{
	  if(bin->SF.b_count[c.suit] == 0) 
	    {/* if it is the first card with that suit, we want it in the bin */
	      Add(c, &bin->SF.b[c.suit], bin->SF.b_max, &bin->SF.b_count[c.suit], bin, 8);
	    }
	  if(bin->SF.b_count[c2.suit] == 0)
	    Add(c2, &bin->SF.b[c2.suit], bin->SF.b_max, &bin->SF.b_count[c2.suit], bin, 8);

	  if(bin->SF.b_count[c.suit] > 0 && bin->SF.b_count[c2.suit] > 0)
	    {/* we already have cards in the bins */
	      int current_count = bin->SF.b_count[c2.suit];
	      CARD last;
	      if(current_count > 0)
		last = bin->SF.b[c2.suit][current_count - 1];
	      if(distance(last, c2) == 1)
		{/* check if the last card in the SF bin and Current card are worthy of SF bin */

		  if(bin->is_full[8] == FALSE && current_count > 0)
		    Add(c2, &bin->SF.b[c2.suit], bin->SF.b_max, &bin->SF.b_count[c2.suit], bin, 8);
		}
	      else
		{/* clear the bin because we have ruined any chance of a straight flush */
		    bin->SF.b[c2.suit] = clearbin(&bin->SF.b[c2.suit], &bin->SF.b_count[c2.suit], bin->SF.b_max);
		}
	    }
	  if(i == size_of_hand-2)
	    {/* special case for the last cards: since aces are high and low, also could have SF with Pair of aces..etc */
	      	  int current_count = bin->SF.b_count[c2.suit];
		  CARD last;
		  if(current_count > 0)
		    {
		      last = bin->SF.b[c2.suit][current_count - 1];

		      if(distance(last, c2) == 1 && bin->is_full[8] == FALSE)
			{/* Check for SF bin conditions */
			  Add(c2, &bin->SF.b[c2.suit], bin->SF.b_max, &bin->SF.b_count[c2.suit], bin, 8);
			}
				      
		      if(last.rank == 0 && hand[0].rank == 12)
			{/*last card in bin is a two...need to check for an Ace in the front to make the wheel*/
			  int cnt = 0;
			  while(hand[cnt].rank == 12)
			    {/* do this since we could have multiple aces to check the suit for */
			      if(hand[cnt].suit == last.suit && bin->is_full[8] == FALSE)
				{
				  Add(hand[cnt], &bin->SF.b[hand[cnt].suit], bin->SF.b_max, &bin->SF.b_count[hand[cnt].suit], bin, 8);
				}
			      cnt++;
			    }
			}
		    }
	    }
	}

      /* /\* Flushes *\/ */
      if(bin->is_full[5] == FALSE)
      	{
      	  if(i % 2 == 0)
      	    {/* add on the evens otherwise we would count every card twice */

      	      /* Regardless, we add cards to the Flush bin */
      	      Add(c, &bin->F.b[c.suit], bin->F.b_max, &bin->F.b_count[c.suit], bin, 5);
      	      if(bin->F.b_count[c2.suit] < bin->F.b_max)
      		{/* we can add second card */
      		  Add(c2, &bin->F.b[c2.suit], bin->F.b_max, &bin->F.b_count[c2.suit], bin, 5);
      		}
      	    }/* END if(i % 2 == 0) */
      	  else
      	    {/* check for the last card */
      	      if(i == size_of_hand-2)
      		{
      		  if(bin->is_full[5] == FALSE)
      		    {
      		      Add(c2, &bin->F.b[c2.suit], bin->F.b_max, &bin->F.b_count[c2.suit], bin, 5);
      		    }
      		}
      	    }
      	}

      int delta = distance(c, c2);
      if(delta == 0)
	{/* Cards Match */

	  /* Fill Coresponding Bins */

	  /* Fill the pair bin and mark it as full */
	  if(bin->is_full[1] == FALSE) {
	    Add(c, &bin->P.b, bin->P.b_max, &bin->P.b_count, bin, 1);
	    Add(c2, &bin->P.b, bin->P.b_max, &bin->P.b_count, bin, 1);
	    bin->is_full[1] = TRUE;
	  }
	  
	  /* Two Pair Bin */
	  /* Check to see if the ranks differ...if not....four of a kind would be considered two pair */
	  if(bin->is_full[2] == FALSE)
	    {
	      if(bin->TP.b_count != 0)
		{/* There are already cards in the bin */
		  if(c.rank != bin->TP.b[0].rank)
		    {
		      Add(c, &bin->TP.b, bin->TP.b_max, &bin->TP.b_count, bin, 2);
		      Add(c2, &bin->TP.b, bin->TP.b_max, &bin->TP.b_count, bin, 2);
		      bin->is_full[2] = TRUE;
		    }
		}
	      else
		{/* These are the first cards in the bin */
		  Add(c, &bin->TP.b, bin->TP.b_max, &bin->TP.b_count, bin, 2);
		  Add(c2, &bin->TP.b, bin->TP.b_max, &bin->TP.b_count, bin, 2);
		}
	    }	  

	  /* Three of a Kind bin */
	  /* Because of the nature of the algorithm, on any given iteration, the bin will either be full or empty */
	  if(bin->is_full[3] == FALSE)
	    {
	      if(size_of_hand - i >= 3)
		{/* make sure we have enough space for i+2 */
		  if(c.rank == hand[i+2].rank)
		    {/* We have three of a kind */
		      Add(c, &bin->TK.b, bin->TK.b_max, &bin->TK.b_count, bin, 3);
		      Add(c2, &bin->TK.b, bin->TK.b_max, &bin->TK.b_count, bin, 3);
		      Add(hand[i+2], &bin->TK.b, bin->TK.b_max, &bin->TK.b_count, bin, 3);
		      bin->is_full[3] = TRUE;
		    }
		}
	    }
	  
	  /* Fill Full House */
	  /* You'd think we would be checking this one here...but, actually, if you have a full TP and TK bin */
	  /* you have a full house...we will make this check at the end */
	  

	  /* Four of a Kind Bin */
	  /* Same heuristics as TK bin */
	  if(bin->is_full[7] == FALSE)
	    {
	      if(size_of_hand - i >= 4)
		{/* make sure we have enough space */
		  if(c.rank == hand[i+2].rank && c.rank == hand[i+3].rank)
		    {
		      Add(c, &bin->FK.b, bin->FK.b_max, &bin->FK.b_count, bin, 7);
		      Add(c2, &bin->FK.b, bin->FK.b_max, &bin->FK.b_count, bin, 7);
		      Add(hand[i+2], &bin->FK.b, bin->FK.b_max, &bin->FK.b_count, bin, 7);
		      Add(hand[i+3], &bin->FK.b, bin->FK.b_max, &bin->FK.b_count, bin, 7);
		      bin->is_full[7] = TRUE;
		    }
		}
	    }
	  
	} /* End cards match */
      if(delta == 1)
	{/* Cards differ by one */

	  /* Fill Coresponding Bins */

	  /* Straights */
	  if(bin->is_full[4] == FALSE)
	    {
	      /* suits dont match.  Still good for a straight */
	      /* Straights */
	      if(bin->S.b_count <= 0)
	  	{/* These are the first two cards in the bin */
	  	  Add(c, &bin->S.b, bin->S.b_max, &bin->S.b_count, bin, 4);
	  	  Add(c2, &bin->S.b, bin->S.b_max, &bin->S.b_count, bin, 4);
	  	}
	      else
	  	{/* Just add the second otherwise we will get dups */
	  	  Add(c2, &bin->S.b, bin->S.b_max, &bin->S.b_count, bin, 4);
	  	}
	    }
		

	}/* END IF delta == 1 */

      if(delta > 1 && delta < 12)
	{/* any potential straight is ruined, clear the straight bin */
	  bin->S.b = clearbin(&bin->S.b, &bin->S.b_count, bin->S.b_max);
	}

    } /*END for(i = 0; i < size_of_hand; i++)

/* Final Checks for made hands */
  /*Case for Full House */
  /* If we have two pair and three of a kind bins full...we have a full house */
  if(bin->is_full[2] == TRUE && bin->is_full[3] == TRUE)
    {
      int t;
      for(t = 0; t < 3; t++)
	{ /* add all the three kind cards */
	  Add(bin->TK.b[t], &bin->FH.b, bin->FH.b_max, &bin->FH.b_count, bin, 6);
	}
      for(t = 0; t < 4; t++)
	{/* check TP bin for the highest rank that doesnt match the TK bin */
	  if(bin->TP.b[t].rank != bin->TK.b[0].rank)
	    {/* This is our pair, since we are sorted the first cards will be the largest */
	      Add(bin->TP.b[t], &bin->FH.b, bin->FH.b_max, &bin->FH.b_count, bin, 6);
	      Add(bin->TP.b[t+1], &bin->FH.b, bin->FH.b_max, &bin->FH.b_count, bin, 6);
	      bin->is_full[6] = TRUE;
	      break;
	    }
	}
    }

  /* Some Vars for Special Cases */
  CARD temp;
  CARD temp2;
  int counter = 0;
  int counter2 = size_of_hand - 1;
  temp = hand[counter];
  temp2 = hand[counter2];
  int d = distance(temp, temp2);
  /* FIXME: need to add this into a better place in the algo...this is a cheap fix */
  while(hand[counter].rank == 12)
    {/* we do this because you could have a pair of aces, or three, or four of them */
      /* Special Case for Straights: Ace in the front 2 in the back */
      counter2 = size_of_hand - 1;
      while(hand[counter2].rank == 0)
	{
	  if(bin->is_full[4] == FALSE && bin->S.b_count > 0 && d == 12)
	    {/* if the bin isn't full AND has something in it and first and last card are Ace and Two */
	      if(bin->S.b[0].rank == 3)
		{/* Highest Card in the Straight bin HAS to be 5...3 is 5 btw...rank starts at 0 which is 2 */
		  Add(temp, &bin->S.b, bin->S.b_max, &bin->S.b_count, bin, 4);
		}
	    }

	  /* Same thing for SF */
	  if(temp.suit == temp2.suit)
	    {
	      if(bin->is_full[8] == FALSE && bin->SF.b_count[temp.suit] > 0 && d == 12)
		{/* if the bin isn't full AND has something in it and first and last card are Ace and Two */

		  if(bin->SF.b[temp.suit][0].rank == 3)
		    {/* Highest Card in the Straight bin HAS to be 5...3 is 5 btw...rank starts at 0 which is 2 */
		      Add(temp, &bin->SF.b[temp.suit], bin->SF.b_max, &bin->SF.b_count[temp.suit], bin, 8);
		    }
		}
	    }

	  temp2 = hand[--counter2];
	  d = distance(temp,temp2);

	}
      temp = hand[++counter];
      d = distance(temp,temp2);
    }

  return get_max_rank(bin);

}

